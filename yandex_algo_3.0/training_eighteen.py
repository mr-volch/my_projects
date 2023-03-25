class dequeue:
    def __init__(self):
        self.data = []
        self.length = 0

    def push_front(self, elem):
        self.length += 1
        self.data.insert(0, elem)
        return 'ok'

    def push_back(self, elem):
        self.length += 1
        self.data.append(elem)
        return 'ok'

    def pop_front(self):
        if self.length == 0:
            return 'error'
        self.length -= 1
        return self.data.pop(0)

    def pop_back(self):
        if self.length == 0:
            return 'error'
        self.length -= 1
        return self.data.pop(-1)

    def front(self):
        if self.length == 0:
            return 'error'
        return self.data[0]

    def back(self):
        if self.length == 0:
            return 'error'
        return self.data[-1]

    def size(self):
        return self.length

    def clear(self):
        self.data = []
        self.length = 0
        return 'ok'


def main_loop():
    q = dequeue()
    cmds = {'push_front': dequeue.push_front, 'push_back': dequeue.push_back, 'pop_front': dequeue.pop_front,
            'pop_back': dequeue.pop_back, 'front': dequeue.front, 'back' : dequeue.back,
            'size': dequeue.size, 'clear': dequeue.clear}

    while (usr_cmd := input()) != 'exit':
        usr_cmd = usr_cmd.split()

        match len(usr_cmd):
            case 1:
                args = (q,)
            case 2:
                args = (q, int(usr_cmd[1]))

        print(cmds[usr_cmd[0]].__call__(*args))

    print('bye')


main_loop()
