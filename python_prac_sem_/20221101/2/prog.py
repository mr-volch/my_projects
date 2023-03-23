class Triangle:
    def __init__(self,A,B,C):
        self.A = A
        self.B = B
        self.C = C
        cur = [self.A,self.B,self.C]
        res = [((cur[i][0] - cur[j][0])**2 + (cur[i][1] - cur[j][1])**2)**0.5 for i in range(2) for j in range(i+1,3)]
        if res[0] <= res[1] + res[2] and res[1] <= res[0] + res[2] and res[2] <= res[0] + res[1]:

            self.S = abs((B[0]-A[0])*(C[1]-A[1])-(C[0]-A[0])*(B[1]-A[1]))/2
        else:
            self.S = 0

    def __abs__(self):
        return self.S

    def __bool__(self):
        if self.S:
            return True
        return False

    def __lt__(self, other):
        if self.S < other.S:
            return True
        else:
            return False

    def __contains__(self, other):
        #в задаче нет спецификации для случая включения в пустой треугольник или пустого треугольника в другой
        #однако в соответствии в выводом теста, решено сделать следующую спецификацию: пустой треугольник содержится везде
        #в пустом треуголнике не содержится ничего, кроме пустого треугольника
        if other.S == 0:
            return True
        if self.S == 0:
            return False
        A,B,C = self.A,self.B,self.C
        A1,B1,C1 = other.A,other.B,other.C
        for D in (A1,B1,C1):
            n_1 = (A[0]-D[0])*(B[1]-A[1])-(B[0]-A[0])*(A[1]-D[1])
            n_2 = (B[0]-D[0])*(C[1]-B[1])-(C[0]-B[0])*(B[1]-D[1]) 
            n_3 = (C[0]-D[0])*(A[1]-C[1])-(A[0]-C[0])*(C[1]-D[1])
            if n_1*n_2 < 0 or n_2*n_3 < 0:
                return False
        return True

    def __and__(self, other):
        if self and other:
            A,B,C = self.A,self.B,self.C
            A1,B1,C1 = other.A,other.B,other.C
            if self.S > other.S:
                A,B,C,A1,B1,C1 = A1,B1,C1,A,B,C
            for D in (A1,B1,C1):
                n_1 = (A[0]-D[0])*(B[1]-A[1])-(B[0]-A[0])*(A[1]-D[1])
                n_2 = (B[0]-D[0])*(C[1]-B[1])-(C[0]-B[0])*(B[1]-D[1]) 
                n_3 = (C[0]-D[0])*(A[1]-C[1])-(B[0]-A[0])*(C[1]-D[1])
                #print("NS:", n_1, n_2, n_3)
                if n_1*n_2 > 0 or n_2*n_3 > 0:
                    return True
            return False
        return False

    def __matmul__(self, other):
        cur = [self.A[0], self.A[1], self.B[0], self.B[1], self.C[0], self.C[1]]
        #print(cur)
        for i in range(len(cur)):
            if i%2:
                cur[i] = 2*other[1]-cur[i]
            else:
                cur[i] = 2*other[0]-cur[i]
        return Triangle(cur[0],cur[1],cur[2],cur[3],cur[4],cur[5])

    def __str__(self):
        return 'Coords:' + str(self.A) + str(self.B) + str(self.C) + ' S:' + str(self.S)
        
        
import sys
exec(sys.stdin.read())
