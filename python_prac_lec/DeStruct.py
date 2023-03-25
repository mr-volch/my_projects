import base64
from struct import *
import sys

a = sys.stdin.read()
b = base64.b85decode(a[:-1])
i = 0
struct_pattern = []
struct_len = 0
while i < len(b) and b[i] != 0:
    if b[i] <= 8:
        cur_num = b[i]
    else:
        cur_num = b[i]-256
    struct_pattern.append(cur_num)
    struct_len += abs(cur_num)
    i += 1
i += struct_len+1
total_summ = 0
while i <= len(b):
    cur_struct = b[i-struct_len:i]
    cur_byte = 0
    for j in struct_pattern:
        cur_field = cur_struct[cur_byte:cur_byte+abs(j)]
        total_summ += int.from_bytes(cur_field, 'big', signed = (j<0))
        cur_byte += abs(j)
    i += struct_len
print(total_summ)
