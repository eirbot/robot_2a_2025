from typing_extensions import override
import aioserial
from typing import Generic, Set

from cfr_2025_2a_embedded_ai.config import RobotSerialCommunicationConfig
from .com import Communication, ReplyPrefix


class SerialCom(Communication[ReplyPrefix], Generic[ReplyPrefix]):
    def __init__(self, anticipatedAnswerPrefixes: Set[ReplyPrefix],
                 com_settings: RobotSerialCommunicationConfig) -> None:
        super().__init__(anticipatedAnswerPrefixes, com_settings["delays"])
        # Do not directly precise the port so the AioSerial is not opened yed
        self._aioserial_instance: aioserial.AioSerial = aioserial.AioSerial(baudrate=9600)
        self._aioserial_instance.port = com_settings["ports"]

    @override
    async def _open_channel(self):
        self._aioserial_instance.open()

    @override
    async def _close_channel(self):
        self._aioserial_instance.close()

    @override
    async def _blocking_read(self) -> str:
        data: bytes = await self._aioserial_instance.read_async()
        return data.decode(errors='ignore')

    @override
    async def _blocking_write(self, message: str) -> None:
        await self._aioserial_instance.write_async(message.encode("utf-8"))
