a = eval(input())
a.sort()
for i in range(len(a) - 1):
	print(a[i], end = ', ') 
print(a[len(a)-1])
