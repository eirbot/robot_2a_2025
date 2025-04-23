from async_pyserial import SerialPort, SerialPortOptions, SerialPortParity, set_async_worker
import asyncio
from random import randint

PORT = '/dev/ttyUSB0'

def on_data(data: bytes):
    print(f"Received: {data.decode('utf-8')}")
    pass

options = SerialPortOptions()
options.baudrate = 9600
options.bytesize = 8
options.stopbits = 1
options.parity = SerialPortParity.NONE # NONE, ODD, EVEN
options.read_bufsize = 8*64
set_async_worker("asyncio")

serial_port = SerialPort(PORT, options)

async def run_read_loop():
    try:
        while True:
            print("Ready for reading. Awaiting...")
            # TODO: accumulate the stream and parse when a '\n' is received
            buf: bytes = await serial_port.read() # type: ignore # TODO: make this with stronger typing
            on_data(buf)
    except asyncio.CancelledError:
        print('Reading loop cancelled!')
        raise

async def run_write_loop_io():
    """
    This function is not a good example to show the concurrency,
    as input() is blocking.
    """
    while True:
        data_to_send = input("Enter data to send (or 'exit' to quit): ")
        # add \n for the test program in the stm32
        data_to_send += "\n"
        if data_to_send.lower() == 'exit\n':
            return
        await serial_port.write(data_to_send.encode('utf-8')) # type: ignore # TODO: make this with stronger typing

async def run_write_loop(delay: float):
    """Infinitely write random integers each 'delay' seconds
    """
    try :
        while True:
            nb = randint(156, 343)
            print("writing", nb)
            # add \n for the test program in the stm32
            await asyncio.sleep(delay)
            data_to_send = str(nb) + "\n"
            await serial_port.write(data_to_send.encode('utf-8')) # type: ignore # TODO: make this with stronger typing
    except asyncio.CancelledError:
        print(f"Writing loop with a delay of {delay} cancelled!")
        raise

async def async_run():
    try:
        # run writing and reading in concurrency
        async with asyncio.TaskGroup() as tg:
            read_loop = tg.create_task(run_read_loop())
            write_loop_1 = tg.create_task(run_write_loop(0.3))
            write_loop_2 = tg.create_task(run_write_loop(0.5))
    except asyncio.CancelledError:
        print("All the group is cancelled!")
    except KeyboardInterrupt:
        print("Nope!")
    except Exception as ex:
        print("It happens!")
        print(ex)
    finally:
        serial_port.close()

def run():
    serial_port.open()
    asyncio.run(async_run())

if __name__ == "__main__":
    run()
