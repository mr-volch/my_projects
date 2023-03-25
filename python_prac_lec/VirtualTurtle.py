def turtle(coord, direction):
    coord = list(coord)
    a = yield tuple(coord)

    while True:
        if a == 'f':
            coord[direction%2] += (1 if direction < 2 else -1)
        elif a == 'l':
            direction += 1
            direction = direction%4
        elif a == 'r':
            direction -= 1
            direction = direction%4
        a = yield tuple(coord)
