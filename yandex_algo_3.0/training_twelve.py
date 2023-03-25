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


def check_parentheses(par_seq):
    st = stack()
    couples = {')':'(', ']': '[', '}': '{'}
    closing_par = list(couples.keys())

    for p in par_seq:
        if p not in closing_par:
            st.push(p)
        else:
            if st.pop() != couples[p]:
                return 'no'

    if st.get_size() == 0:
        return 'yes'
    return 'no'


print(check_parentheses(input()))
