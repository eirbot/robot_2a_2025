import logging
from typing import Optional

COLOR_RESET = "\033[0m"
COLOR_BLUE = "\033[94m"  # Blue
COLOR_GREEN = "\033[92m"  # Green


class ColorFormatter(logging.Formatter):
    level_colors = {
        "<R>": COLOR_BLUE,
        "<S>": COLOR_GREEN,
    }

    def format(self, record):
        level_color = ColorFormatter.level_colors.get(record.msg[:3], COLOR_RESET)
        record.msg = f"{level_color}{record.msg}{COLOR_RESET}"
        return super().format(record)


handler = logging.StreamHandler()
handler.setFormatter(ColorFormatter("%(levelname)s: %(message)s"))

# Configure logger
logger = logging.getLogger(__name__)
# TODO: make a production mode with logging.INFO mode
logger.setLevel(logging.DEBUG)
logger.addHandler(handler)
logger.propagate = False  # Avoid duplicate logs

LOOP_PREFIX = {False: "<R>", True: "<S>"}

def prefix(ob: Optional[bool]) -> str:
    return "" if ob is None else f"{LOOP_PREFIX[ob]} "


def print_log(msg: str, in_strategy_loop: Optional[bool] = None):
    logger.info(prefix(in_strategy_loop) + msg)


def print_debug_log(msg: str, in_strategy_loop: Optional[bool] = None):
    logger.debug(prefix(in_strategy_loop) + msg)
