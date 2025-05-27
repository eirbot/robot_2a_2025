from typing import Literal, Set

EspReplyPrefix = Literal["GoToPosition", "Cannette"]
GOTO_ANSWER_PREFIX: EspReplyPrefix = "GoToPosition"
CANETTE_ACTION_ANSWER_PREFIX: EspReplyPrefix = "Cannette"
ESP_REPLY_PREFIX_SET: Set[EspReplyPrefix] = { GOTO_ANSWER_PREFIX, CANETTE_ACTION_ANSWER_PREFIX }

EspNotificationsPrefix = Literal["P", "D"] # big lol
POSITION_PREFIX: EspNotificationsPrefix = "P"
DETECTION_PREFIX: EspNotificationsPrefix = "D"
ESP_NOTIF_PREFIX_SET: Set[EspNotificationsPrefix] = {"P", "D"}
