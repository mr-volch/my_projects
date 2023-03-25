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

def migration(size, costs):
    migrants_st = stack()
    result = {i : -1 for i in range(size)}

    for i in range(size):
        while migrants_st.get_size() > 0 and migrants_st.back()[0] > costs[i]:
            cur = migrants_st.pop()
            result[cur[1]] = i

        migrants_st.push((costs[i],i))

    for town in result:
        print(result[town])

    return result

migration(int(input()), [int(i) for i in input().split()])
    
            
        
    
