"""
Contains all the utilities method to interact with the robot's firmware
"""

import asyncio
from typing import Callable, Coroutine, Tuple

from .communication.com import Communication, TerminateReadLoop
from .communication.fifo_com import FifoCom
from .communication.serial_com import SerialCom

from .debug_log import print_debug_log, print_log

Position = Tuple[int, int]

READ_YIELDING_TIME = 2

class Robot:
    GOTO_ANSWER_PREFIX = "GoToPosition"
    CANETTE_ACTION_ANSWER_PREFIX = "Cannette"

    def __init__(self, position: Position, debug=False) -> None:
        self._p: Position = position
        self._com: Communication
        if debug:
            self._com = FifoCom(
                (Robot.GOTO_ANSWER_PREFIX, Robot.CANETTE_ACTION_ANSWER_PREFIX),
                READ_YIELDING_TIME*1000
            )
        else:
            self._com = SerialCom(
                (Robot.GOTO_ANSWER_PREFIX, Robot.CANETTE_ACTION_ANSWER_PREFIX),
                READ_YIELDING_TIME*1000,
                '/dev/pts/3'
            )

    async def _run_loop_and_terminate_group(self, robot_loop_func: Callable[[], Coroutine]):
        await robot_loop_func()
        raise TerminateReadLoop()

    async def connect_and_run(self, robot_loop_func: Callable[[], Coroutine]):
        print_log("Starting in concurrence robot's strategy loop (S) and communication reading loop (R)")
        try:
            # run robot loop and reading in concurrency
            async with asyncio.TaskGroup() as tg:
                tg.create_task(self._run_loop_and_terminate_group(robot_loop_func))
                tg.create_task(self._com.read_loop())
        except* TerminateReadLoop:
            print_log("Robot's strategy loop and communication reading loop finished!")

    @property
    def p(self):
        return self._p

    async def goTo(self, x: int, y: int, angle: float) -> None:
        try:
            print_debug_log("-"*5 + f"Sending GoTo with these parametres: {x} {y} {angle}" + "-"*5, in_strategy_loop=True)
            await self._com.send(f"G {x} {y} {angle}", Robot.GOTO_ANSWER_PREFIX)
            self._p = (x, y)
            print_debug_log("-"*5 + f"GoTo done! Robot in position {self._p}" + "-"*5, in_strategy_loop=True)
        except asyncio.TimeoutError:
            print_debug_log("-"*5 + f"Timeout in sending GoTo {x} {y} {angle}" + "-"*5, in_strategy_loop=True)

    async def actionCanette(self) -> None:
        try:
            print_debug_log("-"*5 + "Sending CanetteAction" + "-"*5,
                            in_strategy_loop=True)
            await self._com.send("C", Robot.CANETTE_ACTION_ANSWER_PREFIX)
            print_debug_log("-"*5 + "CanetteAction confirmed by esp" + "-"*5,
                            in_strategy_loop=True)
        except asyncio.TimeoutError:
            print_debug_log(
                "-" * 5 + "Timeout in sending CanetteAction" + "-" * 5,
                in_strategy_loop=True,
            )
