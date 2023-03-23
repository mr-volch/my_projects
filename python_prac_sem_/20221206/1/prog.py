import asyncio
from collections import deque

global_event = asyncio.Event()

async def writer(q1, t1):
    n = 0
    await asyncio.sleep(t1)
    while not global_event.is_set():
        await q1.put(f"{n}_{t1}")
        await asyncio.sleep(t1)
        n += 1
    await q1.put(None)

async def stacker(q1, s1):
    while not global_event.is_set():
        tmp = await q1.get()
        await s1.put(tmp)
        
async def reader(s1, a1, t1):
    await asyncio.sleep(t1)
    for i in range(a1):
        tmp = await s1.get()
        print(tmp)
        await asyncio.sleep(t1)
    global_event.set()

async def main():
    d1, d2, d3, c = map(int, input().split(','))
    queue = asyncio.Queue()
    stack = asyncio.LifoQueue()
    await asyncio.gather(
        writer(queue, d1),
        writer(queue, d2),
        stacker(queue, stack),
        reader(stack, c, d3)
    )

asyncio.run(main())
