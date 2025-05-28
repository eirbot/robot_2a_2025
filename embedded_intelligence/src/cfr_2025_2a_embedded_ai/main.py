import asyncio

from .config import load_config
from .world import World
from .firmware import Robot

def strategy(robot: Robot):
    async def robot_loop_func():
        await robot.goTo(2, 1, 10)
        await robot.goTo(3, 3, 10)
        await robot.actionCanette()
    return robot_loop_func

# run one of the functions below with poetry

def start_debug():
    config = load_config()
    robot = Robot((1, 2), config)
    game = World()
    game = game
    asyncio.run(robot.connect_and_run(strategy(robot)))

def start():
    load_config()
    print("Robot loop with real strategy not implemented yet. Leaving...")
