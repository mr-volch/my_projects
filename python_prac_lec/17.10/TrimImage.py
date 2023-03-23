def read_pattern():
    drawer = []
    while 1:
        to_draw = (input()).split()
        if len(to_draw) == 0:
            break
        for i in range(4):
            to_draw[i] = int(to_draw[i])
        if to_draw[2] != 0 and to_draw[3] != 0:
            drawer.append(to_draw)
    return drawer

def count_blanc(drawer):
    x_min = drawer[0][0]
    x_max = drawer[0][0]
    y_min = drawer[0][1]
    y_max = drawer[0][1]
    
    for i in drawer:
        x_min = min(x_min, i[0], i[0]+i[2])
        x_max = max(x_max, i[0], i[0]+i[2])
        y_min = min(y_min, i[1], i[1]+i[3])
        y_max = max(y_max, i[1], i[1]+i[3])

    return [x_min, x_max, y_min, y_max]

def draw_blanc(c):
    return([['.' for i in range(c[0], c[1])] for i in range(c[2], c[3])])

def fill_canv(drawer, canv_size, canv):
    for mem in drawer:
        left_i = min(mem[0] - canv_size[0], mem[0] + mem[2]- canv_size[0])
        right_i = max(mem[0] - canv_size[0], mem[0] + mem[2]- canv_size[0])
        left_j = min(mem[1]-canv_size[2], mem[1] + mem[3] - canv_size[2])
        right_j = max(mem[1] - canv_size[2] , mem[1] + mem[3] - canv_size[2])
        for i in range(left_i, right_i):
            for j in range(left_j, right_j):
                canv[j][i] = mem[4]
    return canv

def view_canv(canv):
    for i in canv:
        print("".join(i))

drawer = read_pattern()
canv_size = count_blanc(drawer)
canv = draw_blanc(canv_size)
canv = fill_canv(drawer, canv_size, canv)
view_canv(canv)
