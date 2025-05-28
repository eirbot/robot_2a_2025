import argparse
import tomllib
from pathlib import Path
from typing import TypedDict, Literal, Union, cast

RobotCommunicationDelays = TypedDict(
    "RobotCommunicationDelays",
    {
        "read_timeout_before_yield": float,
        "read_yielding_delay": float,
        "response_awaiting": float,
    },
)
RobotSerialCommunicationConfig = TypedDict(
    "RobotSerialCommunicationConfig",
    {"protocol": Literal["serial"], "ports": str, "delays": RobotCommunicationDelays},
)

RobotSocketCommunicationConfig = TypedDict(
    "RobotSocketCommunicationConfig",
    {"protocol": Literal["socket"], "ports": str, "delays": RobotCommunicationDelays},
)
RobotConfigRoot = TypedDict(
    "RobotConfigRoot",
    {
        "log_level": Literal["info", "debug"],
        "communication": Union[
            RobotSerialCommunicationConfig, RobotSocketCommunicationConfig
        ],
    },
)
RobotConfig = TypedDict(
    "RobotConfig",
    {"robot": RobotConfigRoot},
)

def _load_config_from_path(config_path: str) -> RobotConfig:
    with open(Path(config_path).resolve(), "rb") as toml_file:
        return cast(RobotConfig, tomllib.load(toml_file))


def load_config():
    parser = argparse.ArgumentParser(description="Run robot with specific settings (see the README for scheme information).")
    parser.add_argument(
        "--config",
        required=True,
        help="TOML config filename, e.g., './robot_settings/debug.toml'"
    )
    args = parser.parse_args()

    return _load_config_from_path(args.config)

