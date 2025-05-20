from abc import ABC, abstractmethod
import asyncio
from queue import SimpleQueue
from typing import Mapping, Optional, Tuple, cast

ReplyPrefix = str

AwaitingString = list[Optional[str]]

class Communication(ABC):
    """
    - read_yield_frequency: this is a duration in milliseconds. After each
      reading try on the communication channel, the reading thread let the other
      tasks run during this duration 
    """
    def __init__(self, anticipatedAnswerPrefixes: Tuple[ReplyPrefix, ...], read_yield_frequency: int) -> None:
        super().__init__()
        self._queue: Mapping[
            ReplyPrefix, SimpleQueue[Tuple[asyncio.Event, AwaitingString]]
        ] = {
            anticipatedAnswerPrefix: SimpleQueue()
            for anticipatedAnswerPrefix in anticipatedAnswerPrefixes
        }
        self._reading_thread_sleep_time_after_reading_try = read_yield_frequency

    @classmethod
    def __create_empty_awaiting_string(cls) -> AwaitingString:
        return [None]

    @classmethod
    def __set_awaiting_string(cls, awaiting_string: AwaitingString, val: str):
        awaiting_string[0] = val

    @classmethod
    def __get_awaiting_string_value(cls, awaiting_string: AwaitingString):
        return awaiting_string[0]

    """This must be a coroutine which try to read on the communication channel
    each `read_yield_frequency` milliseconds, meaning that after a reading try,
    if a line is not read yet, it yields the execution loop to the other tasks
    during an async sleeping with a duration of `read_yield_frequency`.
    """
    @abstractmethod
    async def _frequently_yielding_read(self) -> str:
        pass

    @abstractmethod
    async def _blocking_write(self, message: str) -> None:
        pass

    def _split(self, msg: str) -> Tuple[ReplyPrefix, str]:
        splitted = msg.split(" ")
        return splitted[0], " ".join(splitted[1:])

    async def read_loop(self):
        msg: str = ""
        while True:
            new_msg: list[str] = (await self._frequently_yielding_read()).split("\n")
            while len(new_msg) > 1:
                msg += new_msg.pop(0)
                prefix, return_message = self._split(msg)
                if prefix in self._queue:
                    q = self._queue[prefix]
                    if not q.empty():
                        waiting_event, waiting_answer = self._queue[prefix].get_nowait()
                        # Returning the given string
                        Communication.__set_awaiting_string(waiting_answer,
                                                   return_message)
                        # And setting the waiting event has done
                        # The next yielding read will yield to the process
                        waiting_event.set()
                msg = ""
            msg += new_msg[0]

    async def send(self, msg: str, expected_reply_prefix: ReplyPrefix) -> str:
        wait_reception_event = asyncio.Event()
        expectedAnswer = Communication.__create_empty_awaiting_string()
        self._queue[expected_reply_prefix].put((wait_reception_event,
                                                expectedAnswer))
        await self._blocking_write(msg + "\n")
        print("Sent:", msg)
        print("Waiting:", expected_reply_prefix)
        await wait_reception_event.wait() # automatically yielding to the read task
        return cast(str, Communication.__get_awaiting_string_value(expectedAnswer))

