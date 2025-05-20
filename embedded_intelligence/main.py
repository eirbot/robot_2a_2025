import asyncio
from world import World
from firmware import Robot

def loop():
    pass

def setup():
    pass

def strategy(robot: Robot):
    async def robot_loop_func():
        await robot.goTo(2, 1, 10)
        await robot.goTo(3, 3, 10)
        await robot.actionCanette()
    return robot_loop_func

def start():
    robot = Robot((1, 2))
    game = World()
    asyncio.run(robot.connect_and_run(strategy(robot)))

if __name__ == "__main__":
    start()
