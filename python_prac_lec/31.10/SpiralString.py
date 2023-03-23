class Spiral:

    def __init__(self,s):
        self.di = {}
        for i in s:
            self.di.setdefault(i,0)
            self.di[i] += 1
        le = 0
        n = 0
        while n < len(s):
            le += 1
            n += le
        self.le = le
        self.n = n-len(s)
        tmp = [self.di[i]*i for i in self.di]
        self.d = "".join(tmp)

    def __add__(self, other):
        di = self.di.copy()
        for i in other.di:
            if i in di:
                di[i] += other.di[i]
            else:
                di[i] = other.di[i]
        tmp = [di[i]*i for i in di]
        d = "".join(tmp)

        cur = Spiral(d)
        return cur

    def __sub__(self, other):
        di = self.di.copy()
        for i in other.di:
            if i in di:
                di[i] -= min(other.di[i], di[i])
        tmp = [di[i]*i for i in di]
        d = "".join(tmp)

        cur = Spiral(d)
        return cur

    def __mul__(self, n):
        di = self.di.copy()
        for i in di:
            di[i] *= n
        tmp = [di[i]*i for i in di]
        d = "".join(tmp)

        cur = Spiral(d)
        return cur

    def __iter__(self):
        return self.d.__iter__()

    def __str__(self):
        temp = [[1,0],[0,-1],[-1,0],[0,1]]
        le = self.le

        chet = 1
        mode_u = 0
        mode_d = 0
        mode_r = 0
        mode_l = 0
        sp = [[' ' for i in range(le)] for i in range(le)]
        if le%4 == 1:
            cur = [le-1, le-1-self.n]
            flag = 1
            mode_u = 2
        elif le%4 == 3:
            cur = [0, self.n-1]
            if (le == 3):
                cur = [0, self.n]
                mode_r -= 1
            flag = 3
            mode_d = 2
        elif le%4 == 2:
            cur = [self.n,le-1]
            flag = 0
            mode_d = 1
            mode_r = 1
        else:
            cur = [le-1-self.n, 0]
            flag = 2
            mode_r = -1
            mode_u = 1
            mode_l = 2

        i = len(self.d)
        while i:
            i -= 1
            sp[cur[0]][cur[1]] = self.d[i]

            if flag == 0 and cur[0] == le-1-mode_u:
                mode_u += 2
                flag = 1
            elif flag == 1 and cur[1] == mode_l:
                mode_l += 2
                flag = 2
            elif flag == 2 and cur[0] == mode_d:
                mode_d += 2
                flag = 3
            elif flag == 3 and cur[1] == le-2-mode_r:
                mode_r += 2
                flag = 0

            cur[0] += temp[flag][0]
            cur[1] += temp[flag][1]

        ##уберем все общие пробельные столбцы и пустые строки
        real_sizes = [0,le,0,le]
        for i in range(le):
            for j in range(le):
                if sp[j][i] != ' ':
                    break
            else:
                real_sizes[0] += 1
                continue
            break
        for i in range(le):
            for j in range(le):
                if sp[le-1-j][le-1-i] != ' ':
                    break
            else:
                real_sizes[1] -= 1
                continue
            break
        for i in range(le):
            for j in range(le):
                if sp[i][j] != ' ':
                    break
            else:
                real_sizes[2] += 1
                continue
            break
        for i in range(le):
            for j in range(le):
                if sp[le-1-i][le-1-j] != ' ':
                    break
            else:
                real_sizes[3] -= 1
            break

        sp = sp[real_sizes[2]:real_sizes[3]]
        for i in range(real_sizes[3] - real_sizes[2]):
            sp[i] = "".join(sp[i][real_sizes[0]:real_sizes[1]])
                    
        sp = "\n".join(sp)
        return(sp)
