class my_queue:
    def __init__(self):
        self.data = []
        self.length = 0        

    def push(self, n):
        self.data.append(n)
        self.length += 1
        return 'ok'

    def pop(self):
        if self.length == 0:
            return 'error'
        cur = self.data.pop(0)
        self.length -= 1
        return cur

    def front(self):
        if self.length == 0:
            return 'error'
        return self.data[0]

    def size(self):
        return self.length

    def clear(self):
        self.data = []
        self.length = 0
        return 'ok'

def main_loop():
    q = my_queue()
    cmds = {'push': my_queue.push, 'pop': my_queue.pop, 'front': my_queue.front,
            'size': my_queue.size, 'clear': my_queue.clear}
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
    

