max_len = 1
cur_len = 1
prev = 0
while a := int(input()):
    if a >= prev and prev != 0:
        cur_len += 1
    else:
        if cur_len > max_len:
            max_len = cur_len
        cur_len = 1
    prev = a
    
print(max(max_len, cur_len))
