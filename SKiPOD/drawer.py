import matplotlib
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

series = []
with open(input("введите файл, для которого хотиите просмотреть статистику: "),'r') as f:
    while c:=f.readline():
        series.append(c.split())

series = sorted(series, key = lambda i: int(i[0]))
print(series)
dictik = {}
for i in series:
    dictik.setdefault(int(i[1]), {})
    dictik[int(i[1])].setdefault(int(i[0]), [0,0])
    dictik[int(i[1])][int(i[0])][0] += float(i[3])
    dictik[int(i[1])][int(i[0])][1] += 1

plt.figure(figsize = (170,120))
plt.title(f"Завсисимость времени выполнения программы от количества задействованных нитей/процессов")
plt.ylabel("время(мс)")
plt.xlabel("кол-во нитей/процессов")
plt.grid(True)

counter = -len(dictik)//2+1
colors = []
s = ['green','red','blue','yellow','magenta']
x = []
y = []
for i in dictik:
    for j in dictik[i]:
        a = ''
        if len(str(j))>3:
            a = '  '
        x.append(a + s[counter][0] + str(j) + ' ')
        y.append(1000*dictik[i][j][0]/dictik[i][j][1])
        colors.append(s[counter])
    counter += 1


to_draw = sorted(y)
new_draw = [to_draw[0]]
prev = to_draw[0]
for i in range(1, len(to_draw)-1):
    if to_draw[i]-prev > 0.15*to_draw[-1]:
        new_draw.append(to_draw[i])
        prev = to_draw[i]
new_draw.append(to_draw[-1])
##plt.xticks(sorted(list(set([int(series[i][0]) for i in range(len(series))]))), rotation = 'vertical')
plt.yticks(new_draw, rotation = 'horizontal')
plt.bar(x,y,color = colors, )


handles_list = []
count = 0
prev = 'white'
for i in colors:
    if prev == i:
        prev = i
        continue
    cur_patch = mpatches.Patch(color=i, label=list(dictik.keys())[count])
    handles_list.append(cur_patch)
    count += 1
    prev = i

plt.legend(title = 'кол-во итераций', handles=handles_list)

plt.grid(axis = 'x')
plt.show()
