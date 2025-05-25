import asyncio
from async_pyserial import SerialPortOptions, SerialPort, set_async_worker
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Async Serial Port Communication Tool')
    parser.add_argument('port', type=str, help='The COM port to use (e.g., COM3 or /dev/ttyUSB0)')
    parser.add_argument('--read-mode', type=str, default="from-event", choices=["from-event", "from-read"], help='read mode (default: from-event)')
    parser.add_argument('--internal-read-bufsize', type=int, default=0, help='internal read bufsize (default: 1024)')
    parser.add_argument('--baudrate', type=int, default=9600, help='Baud rate (default: 9600)')
    parser.add_argument('--bytesize', type=int, choices=[5, 6, 7, 8], default=8, help='Number of data bits (default: 8)')
    parser.add_argument('--stopbits', type=int, choices=[1, 1.5, 2], default=1, help='Number of stop bits (default: 1)')
    parser.add_argument('--parity', type=int, choices=[0, 1, 2, 3, 4], default=0, help='Parity (0: None, 1: Odd, 2: Even, 3: Mark, 4: Space, default: 0)')
    
    args = parser.parse_args()
    
    def on_receieved(data):
        
        print(len(data), data)
        
    async def async_run():
        while True:
            req = serial.read()
            buf = ""
            if req is not None:
               buf = await req
            
            print(f'receieved: {buf}')
            
    
    options = SerialPortOptions()
    options.baudrate = args.baudrate
    options.bytesize = args.bytesize
    options.stopbits = args.stopbits
    options.parity = args.parity
    
    if args.async_worker != 'none':
        set_async_worker("asyncio")
        
    if args.internal_read_bufsize > 0:
        options.read_bufsize = args.internal_read_bufsize

    serial = SerialPort(args.port, options)
    
    print(f'read mode: {args.read_mode}')
        
    serial.open()
            
    
    asyncio.run(async_run())
