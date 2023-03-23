import random

def return_dot(y,z,x):
##    print('dot', y,z)
    return (y[1] + (z[1]-y[1]) * (x-y[0])/(z[0]-y[0]))

def find_borders(a,b,c,d,x):
    borders = []
    if min(a[0], d[0]) <= x <= max(a[0],d[0]):
        borders.append(return_dot(a,d,x))
    if min(a[0], c[0]) <= x <= max(a[0],c[0]):
        borders.append(return_dot(a,c,x))

    if min(b[0] ,d[0]) <= x <= max(b[0], d[0]):
        borders.append(return_dot(b,d,x))
    if min(b[0] ,c[0]) <= x <= max(b[0], c[0]):
        borders.append(return_dot(b,c,x))

##    print(x, (min(borders), max(borders)))
    return (min(borders), max(borders))

def randsquare(a,b):
    new_a = (0,0)
    new_b = (b[0]-a[0], b[1]-a[1])


    f_d = (new_b[0]/2, new_b[1]/2)
    s_d = (new_b[1]/2, -new_b[0]/2)
    new_c = (f_d[0] + s_d[0], f_d[1] + s_d[1])
    new_d = (f_d[0] - s_d[0], f_d[1] - s_d[1])
    

    min_x = min(new_a[0],new_b[0],new_c[0],new_d[0])
    max_x = max(new_a[0],new_b[0],new_c[0],new_d[0])

    cur_x = random.uniform(min_x, max_x)

    y_min, y_max = find_borders(new_a,new_b,new_c,new_d,cur_x)

    cur_y = random.uniform(y_min, y_max)
    return (cur_x + a[0],cur_y + a[1])

##for i in range(10000):
##    x, y = randsquare((0,-10.1), (0,10.1))
##    if x**2+y**2 > 100:
##        print(f"Error: {x}:{y}")
