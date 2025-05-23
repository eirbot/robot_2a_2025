import os
import sys

from ..config import load_config

GOTO_ANSWER_PREFIX = "GoToPosition"
CANETTE_ACTION_ANSWER_PREFIX = "Cannette"

# in the viewpoint of the mock esp
inpt_fifo: str
otpt_fifo: str

# Function to create a FIFO if it doesn't exist
def create_fifo(fifo_name: str):
    try:
        if not os.path.exists(fifo_name):
            os.mkfifo(fifo_name)
            print(f"Created FIFO: {fifo_name}")
        else:
            print(f"FIFO already exists: {fifo_name}")
    except Exception as e:
        print(f"Failed to create FIFO {fifo_name}: {e}")
        sys.exit(1)

def write(msg: str):
    global otpt_fifo
    print(f"> {msg}")
    with open(otpt_fifo, "w") as f:
        f.write(msg + "\n")

def callback(line: str):
    if (line.startswith("G")):
        write(GOTO_ANSWER_PREFIX)
    if (line.startswith("C")):
        write(CANETTE_ACTION_ANSWER_PREFIX)

def read_fifo_loop():
    global inpt_fifo
    with open(inpt_fifo, "r") as f:
        while True:
            try:
                line = f.readline()
                if len(line) > 0:
                    print(f"< {line}", end="")
                    # comment it for debug with timeouts
                    callback(line)
            except* Exception:
                pass

def start():
    global inpt_fifo
    global otpt_fifo
    com_config = load_config()["robot"]["communication"]
    if com_config["protocol"] != "fifo":
        raise Exception("Invalid configuration")
    # inversing the pipe using according to the robot viewpoint
    inpt_fifo = com_config["ports"]["output"]
    otpt_fifo = com_config["ports"]["input"]
    create_fifo(inpt_fifo)
    create_fifo(otpt_fifo)
    print("Named Pipe mock communicator. Ready !\n")
    read_fifo_loop()
