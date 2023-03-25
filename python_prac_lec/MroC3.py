class BadLin(Exception): pass

def merge(merge_seq):
    beg_with = {}
    for i in range(len(merge_seq)):
        beg_with[merge_seq[i][0]] = 1
    for cl in beg_with:
        for j in merge_seq:
            if cl in j and j[0] != cl:
                break
        else:
            i = 0
            while i < len(merge_seq):
                if cl in merge_seq[i]:
                    if len(merge_seq[i]) == 1:
                        merge_seq.pop(i)
                    else:
                        merge_seq[i] = merge_seq[i][1:]
                        i += 1
                else:
                    i += 1
            return cl
    else:
        raise BadLin
    
            
def mro_analyze():
    class_dict = {}

    while a := input():
        if a.startswith('class '):
            x,y,z = a.find(' '),a.find('('),a.find(')')
            if y == -1:
                y = a.find(':')
                cur_key = a[x+1:y]
                class_dict[cur_key] = []
            else:
                cur_key = a[x+1:y]
                cur_val = a[y+1:z]
                cur_val = cur_val.split()
                cur_val = "".join(cur_val)
                cur_val = cur_val.split(',')
                class_dict[cur_key] = cur_val

    mro_dict = {}
    kk = 0
    for k in class_dict:
        kk += 1
        merge_seq = []
        for v in class_dict[k]:
            if v in mro_dict:
                merge_seq.append(mro_dict[v])
        if class_dict[k]:
            merge_seq.append(class_dict[k])

        lin = [k]
        while(len(merge_seq) > 0):
            try:
                lin.append(merge(merge_seq))
            except BadLin:
                break
        else:
            mro_dict[k] = lin
            continue
        break
    else:
        return True
    return False

if mro_analyze():
    print('Yes')
else:
    print('No')
