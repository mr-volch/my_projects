import collections

class DivStr(collections.UserString):
    def __init__(self, s = ''):
        super().__init__(s)        
            
    def __floordiv__(self, other):
        a = len(self)//other
        s = []
        for i in range(other):
            s.append(self[a * i: a * (i+1)])
        return s
        
        

    def __mod__(self,other):
        return self[len(self)-len(self)%other:]


import sys
exec(sys.stdin.read())
