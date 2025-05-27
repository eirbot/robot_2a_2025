from abc import ABC, abstractmethod
import asyncio
from typing import Mapping, Optional, Tuple

from ..config import RobotCommunicationDelays

from ..debug_log import print_debug_log, print_log

class TerminateReadLoop(Exception):
    """Exception raised to ends the reading loop when the robot has
    finished"""

ReplyPrefix = str

AwaitingString = list[Optional[str]]

"""
Usage instruction for a safe channel opening/closing:
Only write messages when the read_loop has started running. There is a
protection that will block any message sending while the channel is not open
"""
class Communication(ABC):
    """
    - read_yield_frequency: this is a duration in milliseconds. After each
      reading try on the communication channel, the reading thread let the other
      tasks run during this duration 
    """
    def __init__(self, anticipatedAnswerPrefixes: Tuple[ReplyPrefix, ...],
                 com_settings: RobotCommunicationDelays) -> None:
        super().__init__()
        self._queue: Mapping[
            ReplyPrefix, asyncio.Queue[str]] = {
            anticipatedAnswerPrefix: asyncio.Queue()
            for anticipatedAnswerPrefix in anticipatedAnswerPrefixes
        }
        self._is_channel_open = asyncio.Event()
        self._reading_thread_sleep_time_after_reading_try = com_settings[
            "read_yielding_delay"
        ]
        self._reading_timeout = com_settings["read_timeout_before_yield"]
        self._response_timeout = com_settings["response_awaiting"]

    @classmethod
    def __create_empty_awaiting_string(cls) -> AwaitingString:
        return [None]

    @classmethod
    def __set_awaiting_string(cls, awaiting_string: AwaitingString, val: str):
        awaiting_string[0] = val

    @classmethod
    def __get_awaiting_string_value(cls, awaiting_string: AwaitingString):
        return awaiting_string[0]

    """This must be a coroutine which tries to read on the communication
    channel. Can be blocking.
    The function is allowed to internally raise an asyncio.TimeoutError
    """
    @abstractmethod
    async def _blocking_read(self) -> str:
        pass

    """This coroutine can be blocking.
    The function is allowed to internally raise an asyncio.TimeoutError
    """
    @abstractmethod
    async def _blocking_write(self, message: str) -> None:
        pass

    @abstractmethod
    async def _open_channel(self) -> None:
        pass

    @abstractmethod
    async def _close_channel(self) -> None:
        pass

    def _split(self, msg: str) -> Tuple[ReplyPrefix, str]:
        splitted = msg.split(" ")
        return splitted[0], " ".join(splitted[1:])

    async def read_loop(self) -> None:
        msg: str = ""
        await self._open_channel()
        print_log("Communication channel open! Starting reading loop...",
                  in_strategy_loop=False)
        self._is_channel_open.set()
        try:
            while True:
                new_msg: list[str]
                try:
                    print_debug_log(f"reading during the next {self._reading_timeout} seconds", in_strategy_loop=False)
                    async with asyncio.timeout(self._reading_timeout):
                        new_msg = (await self._blocking_read()).split("\n")
                except asyncio.TimeoutError:
                    print_debug_log(
                        f"timeout, ready to yield for the next {self._reading_thread_sleep_time_after_reading_try} s",
                        in_strategy_loop=False,
                    )
                    await asyncio.sleep(self._reading_thread_sleep_time_after_reading_try)
                    continue

                print_debug_log(f"have read these messages: {new_msg}",
                                in_strategy_loop=False)

                while len(new_msg) > 1:
                    msg += new_msg.pop(0)
                    prefix, return_message = self._split(msg)
                    if prefix in self._queue:
                        # Piping the given string to the other send task
                        # And setting the waiting event has done
                        # The next yielding read will yield to the process
                        self._queue[prefix].put_nowait(return_message)
                    msg = ""
                msg += new_msg[0]
        except asyncio.CancelledError:
            # the read loop is over, we do not use anymore the instance
            self._is_channel_open.clear()
            await self._close_channel()
            print_log("End of reading loop. Communication channel closed!",
                      in_strategy_loop=False)

    """This coroutine can raise an asyncio.TimeoutError if the sent message has
    not be received by the other side of the channel.
    """
    async def send(self, msg: str, expected_reply_prefix: ReplyPrefix) -> str:
        await self._is_channel_open.wait() # do not write while channel is closed 
        print_debug_log(f"Starting writing in channel the message \"{msg}\"",
                        in_strategy_loop=True)
        await self._blocking_write(msg + "\n")
        print_debug_log(f"Finished writing. Awaiting response during the next {self._response_timeout} seconds", in_strategy_loop=True)
        try:
            response_content: str = ""
            async with asyncio.timeout(self._response_timeout):
                response_content = await self._queue[expected_reply_prefix].get() # automatically yielding to the read task
            print_debug_log(f"Got reponse with the following content: {response_content}", in_strategy_loop=True)
            return response_content
        except asyncio.TimeoutError:
            print_debug_log("Timeout in awaiting response. Raise TimeoutError...", in_strategy_loop=True)
            raise TimeoutError()


