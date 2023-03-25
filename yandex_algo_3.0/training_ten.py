s = input()
l = len(s)

letters = sorted(list(set(s)))

dict_amount = {}

for i in letters:
    dict_amount[i] = 0

for i in range(len(s)):
    cur = s[i]
    dict_amount[cur] += (i+1)*(l-i)

for i in dict_amount:
    if dict_amount[i] != 0:
        print(i, ': ', dict_amount[i], sep = '')
