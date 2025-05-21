"""
Contains all the utilities method to interact with the robot's firmware
"""

import asyncio
from typing import Callable, Coroutine, Tuple

from .communication.com import Communication, TerminateReadLoop
from .communication.fifo_com import FifoCom
from .communication.serial_com import SerialCom

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
                READ_YIELDING_TIME*1000
            )

    async def _run_loop_and_terminate_group(self, robot_loop_func: Callable[[], Coroutine]):
        await robot_loop_func()
        raise TerminateReadLoop()

    async def connect_and_run(self, robot_loop_func: Callable[[], Coroutine]):
        try:
            # run robot loop and reading in concurrency
            async with asyncio.TaskGroup() as tg:
                tg.create_task(self._run_loop_and_terminate_group(robot_loop_func))
                tg.create_task(self._com.read_loop())
        except* TerminateReadLoop:
            print("Robot's strategy loop and communication reading loop finished!")

    @property
    def p(self):
        return self._p

    async def goTo(self, x: int, y: int, angle: float) -> None:
        try:
            print("S" + "-"*5 + f"Sending GoTo with these paramtres: {x} {y} {angle}" + "-"*5)
            await self._com.send(f"G {x} {y} {angle}", Robot.GOTO_ANSWER_PREFIX)
            self._p = (x, y)
            print("S" + "-"*5 + f"GoTo done! Robot in position {self._p}" + "-"*5)
        except asyncio.TimeoutError:
            print("S" + "-"*5 + f"Timeout in sending GoTo {x} {y} {angle}" + "-"*5)

    async def actionCanette(self) -> None:
        try:
            print("S" + "-"*5 + "Sending CanetteAction" + "-"*5)
            await self._com.send("C", Robot.CANETTE_ACTION_ANSWER_PREFIX)
            print("S" + "-"*5 + "CanetteAction confirmed by esp" + "-"*5)
        except asyncio.TimeoutError:
            print("S" + "-"*5 + "Timeout in sending CanetteAction" + "-"*5)
