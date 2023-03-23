s = input()

big_str = ''
while True:
    a = input()
    if not a:
        break
    big_str += a + '\n'

i = 0
dict_one = {}
dict_two = {}

target_len = len(big_str)

while i < target_len:
    if big_str[i] == s[3]:
        j = i-1
        while j >= 0  and big_str[j].isspace():
            j -= 1
        k = j
        while j >= 0 and (not big_str[j].isspace()):
            j -= 1

        if big_str[j+1] == s[2]:
            cur = big_str[j+1:k+1]
            dict_two.setdefault(cur, 0)
            dict_two[cur] += 1
            
    if big_str[i] == s[0]:
        i += 1
        while i < target_len-1 and big_str[i].isspace():
            i += 1
        if big_str[i] == s[1]:
            j = i
            while i < target_len and (not big_str[i].isspace()):
                i += 1
            cur = big_str[j:i]
            dict_one.setdefault(cur, 0)
            dict_one[cur] += 1
            
        
    else:
        i += 1

if dict_one:
    found = list(sorted(dict_one.items(), key=lambda item: -item[1]))
    aaa = str(found[0][0]) + ' ' + str(found[0][1]) + ' -'
else:
    aaa = '... 0 -'

if dict_two:
    found = list(sorted(dict_two.items(), key=lambda item: -item[1]))
    bbb = str(found[0][0]) + s[3] + ' ' + str(found[0][1])
else:
    bbb = '... 0'

print(aaa,bbb)
