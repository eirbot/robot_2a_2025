import logging

COLOR_RESET = "\033[0m"
COLOR_DEBUG = "\033[94m"   # Blue
COLOR_INFO = "\033[92m"    # Green
COLOR_WARNING = "\033[93m" # Yellow
COLOR_ERROR = "\033[91m"   # Red
COLOR_CRITICAL = "\033[95m" # Magenta
class ColorFormatter(logging.Formatter):
    level_colors = {
        logging.DEBUG: COLOR_DEBUG,
        logging.INFO: COLOR_INFO,
        logging.WARNING: COLOR_WARNING,
        logging.ERROR: COLOR_ERROR,
        logging.CRITICAL: COLOR_CRITICAL
    }
    def format(self, record):
        level_color = ColorFormatter.level_colors.get(record.levelno, COLOR_RESET)
        record.msg = f"{level_color}{record.msg}{COLOR_RESET}"
        return super().format(record)
handler = logging.StreamHandler()
handler.setFormatter(ColorFormatter('%(message)s'))

# Configure logger
logger = logging.getLogger(__name__)
# TODO: make a production mode with logging.INFO mode
logger.setLevel(logging.DEBUG)
logger.addHandler(handler)
logger.propagate = False  # Avoid duplicate logs

def print_log(msg: str):
    logger.info(msg)

def print_debug_log(msg: str):
    logger.debug(msg)

