s = input()
dict_one = {}
dict_two = {}

flag_one = True
flag_two = True
last_word = ''

while True:
    a = input()
    if not a:
        break

    ind_end = len(a)-1
    while a[ind_end].isspace():
        ind_end -= 1

    if a[ind_end] == s[0]:
        cur_flag = True

    else:
        cur_flag = False

    if not(flag_one):
        a = s[0] + a
        
    firsts = a.split(s[0])[1:]

    if cur_flag:
        firsts = firsts[:-1]

    for i in firsts:
        if not i:
            continue
        first_word = i.split()[0]
        if first_word[0] == s[1]:
            dict_one.setdefault(first_word, 0)
            dict_one[first_word] += 1

    flag_one = not cur_flag

    lasts = a.split(s[3])[:-1]
    
    ind_beg = 0
    while a[ind_beg].isspace():
        ind_beg += 1
    if a[ind_beg] == s[3] and last_word:
        dict_two.setdefault(last_word, 0)
        dict_two[last_word] += 1
        lasts = lasts[1:]

    for i in lasts:
        if not i:
            continue
        last_word = i.split()[-1]
        if last_word[0] == s[2]:
            dict_two.setdefault(last_word, 0)
            dict_two[last_word] += 1
            
    j = ind_end
    while j >= 0 and (not a[j].isspace()):
        j -= 1
    if a[j+1] == s[2]:
        last_word = a[j+1:ind_end+1]
    else:
        last_word = ''
        
    
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
##print(dict_one, dict_two)
