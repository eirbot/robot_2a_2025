from async_pyserial import SerialPort, SerialPortOptions, SerialPortParity, set_async_worker
import asyncio

PORT = '/dev/ttyUSB0'

def on_data(data):
    print(f"Received: {data}")

options = SerialPortOptions()
options.baudrate = 9600
options.bytesize = 8
options.stopbits = 1
options.parity = SerialPortParity.NONE # NONE, ODD, EVEN
set_async_worker("asyncio")

serial_port = SerialPort(PORT, options)

async def run_read():
    while True:
        print("Ready for reading. Awaiting...")
        buf = await serial_port.read() # type: ignore # TODO: make this with stronger typing
        on_data(buf)

async def run_write_loop():
    while True:
        data_to_send = input("Enter data to send (or 'exit' to quit): ")
        if data_to_send.lower() == 'exit':
            return
        await serial_port.write(data_to_send.encode('utf-8')) # type: ignore # TODO: make this with stronger typing

async def async_run():
    _ = run_read()

    try:
        await run_write_loop()
    except KeyboardInterrupt:
        pass
    except Exception as ex:
        print(ex)
    finally:
        serial_port.close()

def run():
    serial_port.open()
    asyncio.run(async_run())

if __name__ == "__main__":
    run()
