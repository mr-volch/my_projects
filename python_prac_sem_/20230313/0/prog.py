#!/usr/bin/env python3
import asyncio

async def echo(reader, writer):
    res = writer.get_extra_info('peername')
    print(res)
    while not reader.at_eof():
        data = await reader.readline()
        data = data.decode()
        data = data.split(maxsplit = 1)
        match data[0]:
            case 'print':
                writer.write(data[1].encode())
            case 'info':
                match data[1][:-1]:
                    case 'host':
                        writer.write((res[0] + '\n').encode())
                    case 'port':
                        writer.write((str(res[1]) + '\n').encode())
                    case _:
                        writer.write("unkown cmd\n".encode())
            case _:
                writer.write("unknown cmd\n".encode())
                
    writer.close()
    await writer.wait_closed()

async def main():
    server = await asyncio.start_server(echo, '0.0.0.0', 1337)
    async with server:
        await server.serve_forever()

asyncio.run(main())

