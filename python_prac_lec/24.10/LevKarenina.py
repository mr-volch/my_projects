s = input()

big_str = ''
while True:
    a = input()
    if not a:
        break
    big_str += a + '\n'

firsts = (big_str.split(s[0]))
dict_one = {}

for sen in firsts[1:]:
    if sen and (sen != '\n'):
        cur_first = sen.split()[0]
        if cur_first[0] == s[1]:
            print(sen)
            dict_one.setdefault(cur_first, 0)
            dict_one[cur_first] += 1

if dict_one:
    found = list(sorted(dict_one.items(), key=lambda item: -item[1]))
    aaa = str(found[0][0]) + ' ' + str(found[0][1]) + ' -'
else:
    aaa = '... 0 -'

lasts = (big_str.split(s[3]))
dict_one = {}
for sen in lasts[:len(lasts)-1]:
    if sen:
        cur_last = sen.split()[-1]
        if cur_last[0] == s[2]:
            dict_one.setdefault(cur_last, 0)
            dict_one[cur_last] += 1

if dict_one:
    found = list(sorted(dict_one.items(), key=lambda item: -item[1]))
    bbb = str(found[0][0]) + s[3] + ' ' + str(found[0][1])
else:
    bbb = '... 0'

print(aaa,bbb)
