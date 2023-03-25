a = eval(input())
dict_main = {}

while (a[0] != 0 or a[1] != 0):
    dict_main.setdefault(a[0], set({}))
    dict_main[a[0]].add(a[1])
    dict_main.setdefault(a[1], set({}))
    dict_main[a[1]].add(a[0])

    a = eval(input())

full_vertices = []
for i in dict_main:
    if len(dict_main[i]) == len(dict_main) - 1:
        full_vertices.append(i)

full_vertices = sorted(full_vertices)

for i in range(len(full_vertices)):
    full_vertices[i] = str(full_vertices[i])
    
print(' '.join(full_vertices))
        
