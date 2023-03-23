import sys

a = sys.stdin.buffer.read()

if a[-1] != '\n':
    a += bytes('\n'.encode('utf-8'))

N = a[0]
a = a[1:]
parts = []
L = len(a)

cur_pos = 0
for i in range(N):
    parts.append(a[cur_pos:cur_pos+i*L//N])
    cur_pos += i*L//N
    L -= i*L//N

parts = sorted(parts)
print(bytes([N]).decode('utf-8'), end = '')
for i in parts:
    print(i.decode('utf-8'), end = '')
print()

