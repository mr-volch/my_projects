A = int(input())
if (A % 50 == 0):
    print("A + B -", end = ' ')
elif (A % 25 == 0):
    print("A - B +", end = ' ')
else:
    print("A - B -", end = ' ')

if (A % 8 == 0):
    print("C +")
else:
    print("C -")
    
