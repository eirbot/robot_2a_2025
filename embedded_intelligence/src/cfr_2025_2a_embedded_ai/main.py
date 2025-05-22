import asyncio
from .world import World
from .firmware import Robot

def strategy(robot: Robot):
    async def robot_loop_func():
        await robot.goTo(2, 1, 10)
        await robot.goTo(3, 3, 10)
        await robot.actionCanette()
    return robot_loop_func

# run one of the functions below with poetry

def start_debug_fifo():
    robot = Robot((1, 2), debug=True)
    game = World()
    asyncio.run(robot.connect_and_run(strategy(robot)))

def start_debug():
    robot = Robot((1, 2))
    game = World()
    asyncio.run(robot.connect_and_run(strategy(robot)))

def start():
    print("Robot loop with real strategy not implemented yet. Leaving...")
