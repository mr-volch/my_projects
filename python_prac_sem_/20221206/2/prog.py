import asyncio
import random

async def merge(A, B, start, middle, finish, event_in1, event_in2, event_out):
    await event_in1.wait()
    await event_in2.wait()

    A = A.copy()
    left = 0
    right = 0
    for i in range(start, finish):
        if left+start == middle:
            B[i:finish] = A[middle+right:finish]
            break
        if right+middle == finish:
            B[i:finish] = A[start+left:middle]
            break

        if A[left+start] < A[right+middle]:
            B[i] = A[left+start]
            left += 1
        else:
            B[i] = A[right+middle]
            right += 1
    event_out.set()



def gen(A):
    cur_len = 2
    cur_begin = 0
    while cur_len < len(A):
        for i in range(len(A)//cur_len):
            if cur_begin + cur_len >= len(A):
                if cur_begin + cur_len//2 < len(A):
                    yield (cur_begin, cur_begin + cur_len // 2, len(A), asyncio.Event(), cur_len)
                cur_len *= 2
                cur_begin = 0
            else:
                yield (cur_begin, cur_begin + cur_len // 2, cur_begin + cur_len, asyncio.Event(), cur_len)
                cur_begin += cur_len
    if cur_len // 2 < len(A):
        yield(0,cur_len//2,len(A),asyncio.Event(), cur_len)
                


E_0 = asyncio.Event()

def gen_task(A):
    tuples = list(gen(A))
    awaiting = 0
    for i in range(len(tuples)):
        if i < len(A):
            yield asyncio.create_task(merge(A,A,tuples[i][0],tuples[i][1],tuples[i][2],E_0,E_0,tuples[i][3]))
        else:
            if i < len(tuples)-1 and tuples[i][4] != tuples[i+1][4] and len(A)%tuples[i][4] > tuples[i][4]//2:
                yield asyncio.create_task(merge(A,A,tuples[i][0],tuples[i][1],tuples[i][2],tuples[awaiting][3],tuples[awaiting][3],tuples[i][3]))
                awaiting += 1
            else:
                yield asyncio.create_task(merge(A,A,tuples[i][0],tuples[i][1],tuples[i][2],tuples[awaiting][3],tuples[awaiting+1][3],tuples[i][3]))
                awaiting += 2

async def mtasks(A):
    A1 = A.copy()
    E_0.set()
    cur = list(gen_task(A1))
    return (cur, A1)

import sys
exec(sys.stdin.read())
