import asyncio
from io import TextIOWrapper
import os
import select
from typing import List, Tuple, cast

from .com import Communication, ReplyPrefix, TerminateReadLoop

class FifoCom(Communication):
    def __init__(self, anticipatedAnswerPrefixes: Tuple[ReplyPrefix, ...], read_yield_frequency: int) -> None:
        super().__init__(anticipatedAnswerPrefixes, read_yield_frequency)
        self.input_fifo_path = "/tmp/fifo_in"
        self.output_fifo_path = "/tmp/fifo_out"

    async def _open_channel(self):
        pass

    async def _close_channel(self):
        pass

    async def _blocking_read(self) -> str:
        fifo_in_fd = os.open(self.input_fifo_path, os.O_RDONLY | os.O_NONBLOCK)
        fifo_in = os.fdopen(fifo_in_fd)

        try:
            while True:
                readable, _, _ = cast(
                    Tuple[List[TextIOWrapper], List, List],
                    select.select([fifo_in], [], [], 0.05),
                )
                if len(readable) == 0:
                    yield_time = 0.05
                    print(f"R\tfifo_read: ready to yield during the next {yield_time} seconds")
                    await asyncio.sleep(yield_time)
                    print("R\tfifo_read: retry reading input")
                    continue
                line = readable[0].read()
                print(f"R\tfifo_read: read message \"{line}\"")
                fifo_in.close()
                return line
        except TerminateReadLoop:
            fifo_in.close()
            raise TerminateReadLoop()

    async def _blocking_write(self, message: str) -> None:
        print("S\tfifo_write: opening and writing")
        with open(self.output_fifo_path, 'w') as fifo:
            fifo.write(message)
        print(f"S\tfifo_write: written message \"{message}\"")

def test():
    async def test_for_main():
        com = FifoCom(("Answer", "OtherAnswer"), 100)
        res = await com._blocking_read()
        print("Read:", res)
        print("Writing...")
        await com._blocking_write("OtherAnswer I sent the message")
        print("Ok done!")

    asyncio.run(test_for_main())
