from string import *

a = input().lower()

b = set()
for i in range(0, len(a)-1):
    if a[i].isalpha() and a[i+1].isalpha():
        b.add((a[i],a[i+1]))

print(len(b))
