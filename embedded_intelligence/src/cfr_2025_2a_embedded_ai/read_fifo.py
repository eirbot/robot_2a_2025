def read_fifo_loop():
    with open("/tmp/fifo_out", "r") as f:
        while True:
            try:
                line = f.readline()
                if len(line) > 0:
                    print(line, end="")
            except* Exception:
                pass
