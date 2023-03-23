s = input()

big_str = ''
while True:
    a = input()
    if not a:
        break
    big_str += a + '\n'

big_str = big_str.split()
m_len = len(big_str)

dict_one = {}
dict_two = {}

for i in range(len(big_str)):
    cur = big_str[i]
    if cur[len(cur)-1] == s[3] and cur[0] == s[2]:
        dict_two.setdefault(cur,0)
        dict_two[cur] += 1

    if cur[len(cur)-1] == s[0] and i < m_len - 1 and big_str[i+1][0] == s[1]:
        dict_one.setdefault(big_str[i+1],0)
        dict_one[big_str[i+1]] += 1
        
if dict_one:
    found = list(sorted(dict_one.items(), key=lambda item: -item[1]))
    aaa = str(found[0][0]) + ' ' + str(found[0][1]) + ' -'
else:
    aaa = '... 0 -'

if dict_two:
    found = list(sorted(dict_two.items(), key=lambda item: -item[1]))
    bbb = str(found[0][0]) + ' ' + str(found[0][1])
else:
    bbb = '... 0'

print(aaa,bbb)
