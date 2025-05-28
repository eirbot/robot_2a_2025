from asyncio import Future
import asyncio
from async_pyserial import SerialPort, SerialPortOptions, SerialPortParity, set_async_worker
from typing import Generic, Set, cast

from cfr_2025_2a_embedded_ai.config import RobotSerialCommunicationConfig
from .com import Communication, ReplyPrefix


class SerialCom(Communication[ReplyPrefix], Generic[ReplyPrefix]):
    def __init__(self, anticipatedAnswerPrefixes: Set[ReplyPrefix],
                 com_settings: RobotSerialCommunicationConfig) -> None:
        super().__init__(anticipatedAnswerPrefixes, com_settings["delays"])
        options = SerialPortOptions()
        options.baudrate = 9600
        options.bytesize = 8
        options.stopbits = 1
        options.parity = SerialPortParity.NONE # NONE, ODD, EVEN
        options.read_bufsize = 8*64
        self._serial_port = SerialPort(com_settings["ports"], options)

    async def _open_channel(self):
        self._serial_port.open()

    async def _close_channel(self):
        self._serial_port.close()

    async def _blocking_read(self) -> str:
        set_async_worker("asyncio", loop=asyncio.get_running_loop())
        while True:
            try:
                buf = (await cast(Future[bytes],
                                self._serial_port.read())).decode('utf-8')
                if not buf:
                    # TODO: if this read is blocking, then add this
                    # to make the coroutine ready to yield sometimes to others
                    await asyncio.sleep(0.05)
                    continue
                else:
                    return buf
            except asyncio.InvalidStateError:
                continue

    async def _blocking_write(self, message: str) -> None:
        await cast(Future[None],
                   self._serial_port.write(message.encode('utf-8'))
                   )
