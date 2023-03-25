
class stack:
    def __init__(self):
        self.data = []
        self.size = 0

    def push(self, num):
        self.data.append(num)
        self.size += 1
        return 'ok'

    def pop(self):
        if self.size == 0:
            return 'error'
        
        cur = self.data.pop(-1)
        self.size -= 1
        return cur

    def back(self):
        if self.size == 0:
            return 'error'
        
        return self.data[self.size - 1]

    def size(self):
        return self.size

    def clear(self):
        self.data = []
        self.size = 0
        return 'ok'

    
def main_loop():
    st = stack()
    cmds = {'push': stack.push, 'pop': stack.pop, 'back': stack.back, 'size': stack.size, 'clear': stack.clear}
    
    while (usr_cmd := input()) != 'exit':
        usr_cmd = usr_cmd.split()

        match len(usr_cmd):
            case 1:
                args = (st,)
            case 2:
                args = (st, int(usr_cmd[1]))

        print(cmds[usr_cmd[0]].__call__(*args))

    print('bye')
            
                
main_loop()  
