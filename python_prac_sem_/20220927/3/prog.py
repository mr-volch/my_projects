l_len = 0
m_1 = []
m_2 = []
i = 0
while l := input():
    l = list(eval(l))
    if not(l_len):
        l_len = len(l)
    if i < l_len:
        m_1.append(l)
    else:
        m_2.append(l)
    i += 1

##print(m_1)
##print('---------------')
##print(m_2)
##print('---------------')
m_3 = []

for i in range(l_len):
    m_3.append([])
    for j in range(l_len):
        cur = 0
        for u in range(l_len):
            cur += m_1[i][u]*m_2[u][j]
        m_3[i].append(cur)

for i in m_3:
    for j in range(l_len):
        print(i[j], end = '')
        if j == (l_len - 1):
            print(end = '\n')
        else:
            print(end = ',')

        
