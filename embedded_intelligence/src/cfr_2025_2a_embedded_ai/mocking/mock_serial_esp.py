import asyncio
from typing import Callable, Coroutine

from ..communication.com import TerminateReadLoop
from ..communication.serial_com import SerialCom
from ..debug_log import print_debug_log

async def connect_and_run(com: SerialCom, write_loop_func: Callable[[], Coroutine]):
    async def run_loop_and_terminate_group():
        await write_loop_func()
        raise TerminateReadLoop()

    print_debug_log("Starting in concurrence communication writing loop (S) and communication reading loop (R)")
    try:
        # run robot loop and reading in concurrency
        async with asyncio.TaskGroup() as tg:
            tg.create_task(run_loop_and_terminate_group())
            tg.create_task(com.read_loop())
    except* TerminateReadLoop:
        print_debug_log("Communication writing loop and communication reading loop finished!")

async def mock_write_communication(communicator: SerialCom):
    async def try_or_fail_sending(msg: str, prefix: str):
        try:
            print_debug_log(f"> {msg}", in_strategy_loop=True)
            res = await communicator.send(msg, prefix)
            print_debug_log(f"< {prefix} {res}", in_strategy_loop=True)
        except asyncio.TimeoutError:
            print_debug_log(f"Timeout in awaiting {prefix}",
                            in_strategy_loop=True)
    await try_or_fail_sending("Hello", "G")
    await try_or_fail_sending("GoToPosition", "G")
    await try_or_fail_sending("GoToPosition", "C")
    await try_or_fail_sending("Cannette", "UnavoidableTimeout")

async def mock_communication(communicator: SerialCom):
    await connect_and_run(communicator, lambda: mock_write_communication(communicator))

def start():
    communicator = SerialCom(("G", "C", "UnavoidableTimeout"), 100, "/dev/pts/4")
    asyncio.run(mock_communication(communicator))
