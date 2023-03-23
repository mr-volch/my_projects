def check_proc(knc,proc,n):
    if n[:len(proc)] == proc and n[-len(knc):] == knc:
        return True
    return False

def second_checker(n ,cur_coding):
    m = copy.copy(n)
    i = 5
    while i >= 0:
        if cur_coding[i] != '0':
            if i % 2 == 1:
                m = m.decode(cur_coding[i])
            else:
                m = m.encode(cur_coding[i])
        i -= 1
    tmp = tmp.decode('koi-8r')
    for i in m:
        if isalpha(i) and i not in ['АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦШЩЪЫЬЭЮЯ']:
            return False
    return True
    
        

def main_proc(knc, proc, n, code_list):
    check_proc(knc,proc,n)
    for i1 in code_list:
        cur_coding = [i1,'0','0','0','0','0']
        for i2 in code_list:
            cur_coding[1] = i2
            try:
                if check_proc(knc.decode(i1).encode(i2), proc.decode(i1).encode(i2), n):
                    if second_checker(n, cur_coding):
                        return cur_coding
            except:
                continue
            for i3 in code_list:
                cur_coding[2] = i3
                for i4 in code_list:
                    cur_coding[3] = i4
                    try:
                        if check_proc(knc.decode(i1).encode(i2).decode(i3).encode(i4), proc.decode(i1).encode(i2).decode(i3).encode(i4), n):
                            if second_checker(n, cur_coding):
                                return cur_coding
                    except:
                        continue
                    for i5 in code_list:
                        cur_coding[4] = i5
                        for i6 in code_list:
                            cur_coding[5] = i6
                            try:
                                if check_proc(knc.decode(i1).encode(i2).decode(i3).encode(i4).decode(i5).encode(i6), proc.decode(i1).encode(i2).decode(i3).encode(i4).decode(i5).encode(i6), n):
                                    if second_checker(n, cur_coding):
                                        return cur_coding
                            except:
                                continue
    print("OH MY GOD")
    return ['0','0','0','0','0','0']

import sys
n = sys.stdin.buffer.read()


code_list = ["cp037", "cp1006", "cp1250", "cp1251", "cp1253", "cp1254", "cp1255", "cp1256", "cp1257", "cp1258", "cp437", "cp720", "cp737", "cp775", "cp850", "cp852", "cp855", "cp864", "cp866", "cp869", "cp874", "cp875", "hp_roman8", "iso8859_10", "iso8859_16", "iso8859_4", "iso8859_5", "koi8_r", "latin_1", "mac_croatian", "mac_greek", "mac_iceland", "mac_latin2"]
fit_code_list = []
for j in code_list:
    try:
        n.decode(j)
    except:
        continue
    else:
        fit_code_list.append(j)

encoded_knc = 'КНЦ;\n'.encode('koi8-r')
encoded_proc = 'ПРОЦ'.encode('koi8-r')

print(fit_code_list)
fit_code_list = ['cp1251','cp869','mac_greek','iso8859_5']


a = main_proc(encoded_knc, encoded_proc, n, fit_code_list)

i = 5
while i >= 0:
    if cur_coding[i] != '0':
        if i % 2 == 1:
            n = n.decode(cur_coding[i])
        else:
            n = n.encode(cur_coding[i])
    i -= 1

n = n.decode('koi8-r')
print(n)

                    
                

    
    
