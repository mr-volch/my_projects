#!/usr/bin/env python3
import asyncio
import cowsay
import shlex

clients = {}
available_cows = cowsay.list_cows()                

async def chat(reader, writer):
    usr_queue = asyncio.Queue()
    name = None
    send = asyncio.create_task(reader.readline())
    receive = asyncio.create_task(usr_queue.get())
    while not reader.at_eof():
        done, pending = await asyncio.wait([send, receive], return_when=asyncio.FIRST_COMPLETED)
        for q in done:
            if q is send:
                send = asyncio.create_task(reader.readline())
                cur_rcv = q.result()
                parsed_cmd = shlex.split(cur_rcv.decode())
                msg = """Unkown command, list of available commands: \n    who\n    cows\n    login <nickname>
    say <usr_name> <message>\n    yield <message>\n    quit"""
                
                if name != None and len(parsed_cmd) > 0:
                    match parsed_cmd[0]:
                        case 'login':
                            msg = "You are allready logged in, if you want to log in with anither name, type 'quit' firstly \n"
                            
                        case 'who':
                            msg = "Here are names of logged in users: \n"
                            for client in clients.keys():
                                msg += (client + '\n')
                                
                        case 'cows':
                            msg = "Here are available nicknames for you: "
                            for cow in available_cows:
                                msg += f' {cow}' 

                        case 'say':
                            try:
                                target_usr = parsed_cmd[1]
                                if target_usr not in clients:
                                    msg = f"There is no client with name {target_usr}\n"
                                else:
                                    await clients[target_usr].put(cowsay.cowsay(parsed_cmd[2], name))
                                    msg = None
                            except IndexError:
                                msg = "Wrong command format, it should be: 'say <usr_name> <message>'\n"

                        case 'yield':
                            try:
                                for out in clients.values():
                                    if out is not usr_queue:
                                        await out.put(cowsay.cowsay(parsed_cmd[1], cow = name))
                                msg = None
                            except IndexError:
                                msg = "Wrong command format, it should be: 'yield <message>'\n"

                        case 'quit':
                            clients.pop(name)
                            available_cows.append(name)
                            break

                else:
                    if len(parsed_cmd) > 0:
                        match parsed_cmd[0]:
                            case 'login':
                                if parsed_cmd[1] in clients:
                                    msg = "This nickname is allready taken by another user\n"
                                elif parsed_cmd[1] not in available_cows:
                                    msg = "This nickname is not valid, to see available nicknames, type 'cows'"
                                else:
                                    clients[parsed_cmd[1]] = usr_queue
                                    name = parsed_cmd[1]
                                    available_cows.pop(available_cows.index(parsed_cmd[1]))
                                    msg = f"hello, {parsed_cmd[1]}"

                            case 'who':
                                msg = "Here are names of logged in users: \n"
                                for client in clients.keys():
                                    msg += (client + '\n')
                            
                            case 'cows':
                                msg = "Here are available nicknames for you: "
                                for cow in available_cows:
                                    msg += f' {cow}' 

                            case 'say':
                                msg = "You can`t send messages until you log in"

                            case 'yield':
                                msg = "You can`t send messages until you log in"

                            case 'quit':
                                break
                if msg:
                    if name:
                        await usr_queue.put(cowsay.cowsay(msg, name))
                    else:
                        await(usr_queue.put(msg))
##                for out in clients.values():
##                    if out is not usr_queue:
##                        await out.put(f"NO {me} {q.result().decode().strip()}")
            elif q is receive:
                receive = asyncio.create_task(usr_queue.get())
                cur_rcv = q.result()
                print(cur_rcv)
                writer.write((f"{q.result()}\n").encode())
                await writer.drain()

        else:
            continue
        break       
    send.cancel()
    receive.cancel()
    print(name, "DONE")
    del usr_queue
    writer.close()
    await writer.wait_closed()


async def main():
    server = await asyncio.start_server(chat, '0.0.0.0', 1337)
    async with server:
        await server.serve_forever()

asyncio.run(main())
