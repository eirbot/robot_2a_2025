import os
import sys

GOTO_ANSWER_PREFIX = "GoToPosition"
CANETTE_ACTION_ANSWER_PREFIX = "Cannette"

fifo1 = '/tmp/fifo_in'
fifo2 = '/tmp/fifo_out'

# Function to create a FIFO if it doesn't exist
def create_fifo(fifo_name):
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
    print(f"> {msg}")
    with open("/tmp/fifo_in", "w") as f:
        f.write(msg + "\n")

def callback(line: str):
    if (line.startswith("G")):
        write(GOTO_ANSWER_PREFIX)
    if (line.startswith("C")):
        write(CANETTE_ACTION_ANSWER_PREFIX)

def read_fifo_loop():
    with open("/tmp/fifo_out", "r") as f:
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
    create_fifo(fifo1)
    create_fifo(fifo2)
    print("Named Pipe mock communicator. Ready !\n")
    read_fifo_loop()
