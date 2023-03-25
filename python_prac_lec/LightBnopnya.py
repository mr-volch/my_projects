import copy

def check_proc(n,m,codings):
    try:
        tmp = n.decode('koi8-r')
    except:
        return False
##    print(tmp[:4])
##    print(codings)
    if tmp[:4] == 'ПРОЦ':
        for i in tmp:
            if i.isalpha() and i not in ['А','Б','В','Г','Д','Е','Ё','Ж','З','И','Й','К','Л','М','Н','О','П','Р','С','Т','У','Ф','Ч','Ц','Х','Ш','Щ','Ъ','Ы','Ь','Э','Ю','Я']:
                return False
##        print(codings)
        i = 0
        cur_m = copy.copy(m)
        while i < len(codings) and codings[i] != '0':
            if i % 2 == 0:
                try:
                    cur_m = cur_m.encode(codings[i])
                except:
                    return False
            else:
                try:
                    cur_m = cur_m.decode(codings[i])
                except:
                    return False
            i += 1
        try:
            tmp = cur_m.decode('koi8-r')
        except:
            return False
        if 'КНЦ;' not in tmp[-6:]:
            return False
        for i in tmp:
            if i.isalpha() and i not in ['А','Б','В','Г','Д','Е','Ё','Ж','З','И','Й','К','Л','М','Н','О','П','Р','С','Т','У','Ф','Ч','Ц','Х','Ш','Щ','Ъ','Ы','Ь','Э','Ю','Я']:
                return False
        return True
    return False

def main_proc(code_list, n, m):
    for i1 in code_list:
        try:
            n0 = n.encode(i1)
            n0 = n0.decode(i1)
        except:
            continue

        for i2 in code_list:
            try:
                n1 = n0.encode(i2)
            except:
                continue
            codings = [i2,'0','0','0','0']
            if check_proc(n1,m,codings):
                return [n1,codings]
            for i3 in code_list:
                try:
                    n2 = n1.decode(i3)
                except:
                    continue
                for i4 in code_list:
                    try:
                       n3 = n2.encode(i4)
                    except:
                        continue
                    codings = [i2,i3,i4,'0','0']
                    if check_proc(n3,m,codings):
                        return [n3,codings]
            
                    for i5 in code_list:
                        try:
                            n4 = n3.decode(i5)
                        except:
                            continue

                        for i6 in code_list:
                            try:
                               n5 = n4.encode(i6)
                            except:
                                continue
                            codings = [i2,i3,i4,i5,i6]
                            if check_proc(n5,m,codings):
                                return [n5, codings]
##    print("AOPOSPA")
####%%%%####%%%%####%%%%####%%%%####%%%%####%%%%####%%%%####end of proc




code_list = ["cp037", "cp1006", "cp1250", "cp1251", "cp1253", "cp1254", "cp1255", "cp1256", "cp1257", "cp1258", "cp437", "cp720", "cp737", "cp775", "cp850", "cp852", "cp855", "cp864", "cp866", "cp869", "cp874", "cp875", "hp_roman8", "iso8859_10", "iso8859_16", "iso8859_4", "iso8859_5", "koi8_r", "latin_1", "mac_croatian", "mac_greek", "mac_iceland", "mac_latin2"]

import sys
n = sys.stdin.buffer.readline()
##print(n.decode('utf-8').encode('cp1251').decode('iso8859_5').encode('cp1251').decode('cp869').encode('mac_greek').decode('koi8-r'))
m = sys.stdin.buffer.read()
##print(m.decode('utf-8').encode('cp1251').decode('iso8859_5').encode('cp1251').decode('cp869').encode('mac_greek').decode('koi8-r'))
n = n.decode('utf-8')
m = m.decode('utf-8')

cur = main_proc(code_list, n, m)
            
n = cur[0].decode('koi8-r')
i = 0
codings = cur[1]
while i < len(codings) and codings[i] != '0':
    if i % 2 == 0:
        m = m.encode(codings[i])
    else:
        m = m.decode(codings[i])
    i += 1
m = m.decode('koi8-r')
if m[-1] == '%':
    m = m[:-1]
print(n+m)
