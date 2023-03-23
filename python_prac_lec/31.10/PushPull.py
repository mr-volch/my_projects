class Pushpull:
    cur = 0
    def __init__(self, a = 0):
        self.__class__.cur = a

    def push(self, r=1):
        self.__class__.cur += r

    def pull(self, l=1):
        self.__class__.cur -= l

    def __iter__(self):
        return iter(tuple([i for i in range(0, self.__class__.cur, self.__class__.cur//abs(self.__class__.cur))]))

    def __str__(self):
        if self.__class__.cur < 0:
            return ('<' + str(-self.__class__.cur) + '<')
        elif self.__class__.cur > 0:
            return ('>' + str(self.__class__.cur) + '>')
        return "<0>"

