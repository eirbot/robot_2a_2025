import asyncio
import os
from typing import Tuple
from io import TextIOWrapper
from .com import Communication, ReplyPrefix

class FifoCom(Communication):
    def __init__(self, anticipatedAnswerPrefixes: Tuple[ReplyPrefix, ...], read_yield_frequency: int) -> None:
        super().__init__(anticipatedAnswerPrefixes, read_yield_frequency)
        self.input_fifo_path = "/tmp/fifo_in"
        self.output_fifo_path = "/tmp/fifo_out"

        self._input_fifo_fd: int
        self._input_fifo_pipe: TextIOWrapper

        self._writer: TextIOWrapper

    async def _open_channel(self):
        # Open named pipe to be read
        # Open the FIFO in non-blocking mode
        self._input_fifo_fd = os.open(self.input_fifo_path, os.O_RDONLY | os.O_NONBLOCK)
        self._input_fifo_pipe = os.fdopen(self._input_fifo_fd)

        # Open named pipe to be written in
        # TODO: implement with streamwriter for full async/user-auto-yielding
        # support
        while True:
            try:
                self._writer = open(self.output_fifo_path, "w")
                break
            except* Exception:
                pass

    async def _close_channel(self):
        self._input_fifo_pipe.close()
        os.close(self._input_fifo_fd)

        self._writer.close()

    async def _frequently_yielding_read(self) -> str:
        # Wrap in asyncio StreamReader
        loop = asyncio.get_running_loop()
        reader = asyncio.StreamReader()
        protocol = asyncio.StreamReaderProtocol(reader)
        await loop.connect_read_pipe(lambda: protocol, self._input_fifo_pipe)
        while True:
            line = await reader.readline()
            if not line:
                await asyncio.sleep(
                    self._reading_thread_sleep_time_after_reading_try / 1000
                )  # Yield time to other tasks
                continue
            received = line.decode().strip()
            return received + "\n"

    async def _blocking_write(self, message: str) -> None:
        self._writer.write(message)

if __name__ == "__main__":

    async def test_for_main():
        com = FifoCom(("Answer", "OtherAnswer"), 100)
        res = await com._frequently_yielding_read()
        print("Read:", res)
        print("Writing...")
        await com._blocking_write("OtherAnswer I sent the message")
        print("Ok done!")

    asyncio.run(test_for_main())
