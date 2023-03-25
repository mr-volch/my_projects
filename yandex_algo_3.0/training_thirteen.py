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

    def get_size(self):
        return self.size

    def clear(self):
        self.data = []
        self.size = 0
        return 'ok'


def poliz_counter(poliz):
    st = stack()
    ops = ('+', '-', '*')

    for mem in poliz:
        match mem:
            case '+':
                cur = st.pop() + st.pop()
                st.push(cur)
            case '-':
                first = st.pop()
                second = st.pop()
                cur = second - first
                st.push(cur)
            case '*':
                cur = st.pop() * st.pop()
                st.push(cur)
            case _:
                st.push(int(mem))

    return (st.pop())

print(poliz_counter(input().split()))
