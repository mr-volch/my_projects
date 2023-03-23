class Alpha:
    __slots__ = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']

    def __init__(self, **kw):
        for i in kw:
           setattr(self, i, kw[i])

    def __str__(self):
        res = ''
        for i in self.__slots__:
            try:
                cur = getattr(self,i)
            except AttributeError:
                continue
            res += i + ': ' + str(cur) + ', '
        res = res[:-2]
        return(res)


class AlphaQ:
    declared = {}

    def __init__(self, **kw):
        for i in kw:
           setattr(self, i, kw[i])

    def __str__(self):
        res = ''
        for i in sorted(self.declared):
            try:
                cur = getattr(self,i)
            except AttributeError:
                continue
            res += i + ': ' + str(cur) + ', '
        res = res[:-2]
        return res

    def __setattr__(self, attrname, val):
        if attrname in ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']:
            self.declared[attrname] = val

    def __getattr__(self, attrname):
        if attrname in self.declared:
            return self.declared[attrname]
        else:
            raise AttributeError

            
import sys
exec(sys.stdin.read())
