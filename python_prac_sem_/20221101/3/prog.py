class Grange:
    def __init__(self,b0,q,bn):
        self.b0, self.q, self.bn = b0, q, bn
        self.seq = []
        while b0 < bn:
            self.seq.append(b0)
            b0 *= q

    def __len__(self):
        return len(self.seq)

    def __bool__(self):
        return (len(self.seq) > 0)

    def __getitem__(self, i):
        if isinstance(i, int):
            if i >= len(self.seq):
                return self.b0*self.q**i
            return self.seq[i]

        #здесь работаем с секционированием
        if i.step == None:
            return Grange(i.start, self.q, i.stop)

        return Grange(i.start, self.q**i.step, i.stop)

        
    def __iter__(self):
        return self.seq.__iter__()

    def __str__(self):
        return "grange(" + str(self.b0) + ', ' + str(self.q) + ', ' + str(self.bn) + ')'

    def __repr__(self):
        return self.__str__()

    
        
import sys
exec(sys.stdin.read())
