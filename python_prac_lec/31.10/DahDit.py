class morse:

    def __init__(self,s=''):
        self.s = ['di', 'dit', 'dah', '.', ',',' ']
        if ' ' in s:
            s = s.split(' ')

        elif s:
            self.s = ['.','.','-','',' ','']

        if len(s) == 2:
            self.s[0] = s[0]
            self.s[1] = s[0]
            self.s[2] = s[1]
        else:
            for i in range(len(s)):
                self.s[i] = s[i]
        self.p = self.s[3]
        self.is_end = True


    def __neg__(self):
        m = morse()
        m.is_end = False
        m.s = self.s
        m.p = self.s[5] + self.s[2] + self.p
        return m

    def __pos__(self):
        m = morse()
        m.is_end = False
        m.s = self.s
        if self.is_end:
            m.p = self.s[5] + self.s[1] + self.p
        else:
            m.p = self.s[5] + self.s[0] + self.p
        return m

    def __invert__(self):
        m = morse()
        m.s = self.s
        m.p = self.s[4] + self.p
        m.is_end = True
        return m

    def __str__(self):
        if self.p[0] == ' ':
            return self.p[1:]
        else:
            return self.p

        
