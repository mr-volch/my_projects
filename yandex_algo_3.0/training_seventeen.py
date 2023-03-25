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


def drunken_sailor(first, second):
    first_queue = my_queue()
    second_queue = my_queue()

    for i in first:
        first_queue.push(i)
    for i in second:
        second_queue.push(i)

    counter = 0
    while first_queue.size() != 10 and second_queue.size() != 10:
        if counter == 1000000:
            break
        cur_first, cur_second = first_queue.pop(), second_queue.pop()
        if not(cur_first == 9 and cur_second == 0) and (cur_first > cur_second or (cur_first == 0 and cur_second == 9)):
            first_queue.push(cur_first)
            first_queue.push(cur_second)
        else:
            second_queue.push(cur_first)
            second_queue.push(cur_second)
        counter += 1
    else:
        if first_queue.size() == 0:
            return ("second", counter)
        else:
            return ("first", counter)
    return ("botva", "")

    
first = [int(i) for i in input().split()]
second = [int(i) for i in input().split()]

res = drunken_sailor(first, second)

if res[1] == '':
    print(res[0])
else:
    print(res[0], res[1])
    
