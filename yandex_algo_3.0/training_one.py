from sys import stdin

s = ''

for line in stdin:
  s += line + '\n'

print(s)

dict_amount = {}
symbol_order = set()
max_h = 0

for i in s:
    if i in (' ', '\n'):
        continue
    if i in dict_amount:
        tmp = dict_amount[i]+1
        if tmp > max_h:
            max_h = tmp
        dict_amount[i] = tmp
    else:
        symbol_order.add(i)
        dict_amount.setdefault(i, 1)

symbol_order = sorted(list(symbol_order))

str_len = len(symbol_order)
to_print = [[' ' for j in range(str_len)] for i in range(max_h+1)]

cur_column = 0
for i in symbol_order:
    cur = dict_amount[i]
    to_print[max_h][cur_column] = i
    for j in range(max_h - cur, max_h):
        to_print[j][cur_column] = '#'
    cur_column += 1
    
for i in range(len(to_print)):
    to_print[i] = ' '.join(to_print[i])
    
print('\n'.join(to_print))
