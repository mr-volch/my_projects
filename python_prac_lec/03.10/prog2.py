N = int(input())
x = 0

while 4*x**2 < N:
    x += 1

pre_y = x

while(x**2 <= N):
    while (pre_y and 3*(pre_y-1)**2 >= N-x**2):
        pre_y -= 1
    pre_z = y = pre_y
    while y <= x and x**2 + y**2 <= N:
        while (pre_z and 2*(pre_z-1)**2 >= N-x**2-y**2):
            pre_z -= 1
        z = t = pre_z
        while z <= y and x**2+y**2+z**2 <= N:
            while (t**2 > N-x**2-y**2-z**2):
                t -= 1
            if (x**2+y**2+z**2+t**2 == N):
                print(x,y,z,t)
            z += 1
        y += 1
    x += 1
