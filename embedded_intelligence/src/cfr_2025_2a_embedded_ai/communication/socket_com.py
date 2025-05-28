from typing_extensions import override
import zmq
import zmq.asyncio

from typing import Generic, Set

from cfr_2025_2a_embedded_ai.debug_log import print_debug_log

from ..config import RobotSocketCommunicationConfig

from .com import Communication, ReplyPrefix


class SocketCom(Communication[ReplyPrefix], Generic[ReplyPrefix]):

    """do not use a protocol like TCP which handle the \n boundaries.
    IPC is quite good for an imitation of the serial duplex byte communication
    """
    PROTOCOL = "inproc"
    def __init__(
        self,
        anticipatedAnswerPrefixes: Set[ReplyPrefix],
        com_settings: RobotSocketCommunicationConfig,
        is_client=False,
    ) -> None:
        super().__init__(anticipatedAnswerPrefixes, com_settings["delays"])
        self._socket = zmq.asyncio.Context().socket(zmq.PAIR)
         
        tcp_url = f"{SocketCom.PROTOCOL}://{com_settings['ports']}"
        self._is_client = is_client
        if self._is_client:
            self._socket.connect(tcp_url)
        else:
            self._socket.bind(tcp_url)

    @override
    async def _open_channel(self):
        if not self._is_client:
            print_debug_log("Waiting a Hello from the socket-esp", False)
            await self._socket.recv_string()

    @override
    async def _close_channel(self):
        pass

    @override
    async def _blocking_read(self) -> str:
        return await self._socket.recv_string()

    @override
    async def _blocking_write(self, message: str) -> None:
        await self._socket.send_string(message)
