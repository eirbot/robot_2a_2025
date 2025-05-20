import asyncio
import os
from typing import Tuple
from com import Communication, ReplyPrefix

class FifoCom(Communication):
    def __init__(self, anticipatedAnswerPrefixes: Tuple[ReplyPrefix, ...], read_yield_frequency: int) -> None:
        super().__init__(anticipatedAnswerPrefixes, read_yield_frequency)
        self.fifo_input_path = "/tmp/fifo_in"
        self.fifo_output_path = "/tmp/fifo_out"


    async def _frequently_yielding_read(self) -> str:
        loop = asyncio.get_running_loop()

        # Open the FIFO in non-blocking mode
        fd = os.open(self.fifo_input_path, os.O_RDONLY | os.O_NONBLOCK)
        pipe = os.fdopen(fd)

        # Wrap in asyncio StreamReader
        reader = asyncio.StreamReader()
        protocol = asyncio.StreamReaderProtocol(reader)
        await loop.connect_read_pipe(lambda: protocol, pipe)

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
        # TODO: implement with streamwriter for full async/user-auto-yielding
        # support
        try:
            with open(self.fifo_output_path, "w") as file:
                file.write(message)
            return
        except* Exception:
            pass

if __name__ == "__main__":

    async def test_for_main():
        com = FifoCom(("Answer", "OtherAnswer"), 100)
        res = await com._frequently_yielding_read()
        print("Read:", res)
        print("Writing...")
        await com._blocking_write("OtherAnswer I sent the message")
        print("Ok done!")

    asyncio.run(test_for_main())
