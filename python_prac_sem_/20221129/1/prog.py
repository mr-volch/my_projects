class dump(type):

    def modify_func(func, name):
        def newfun(*args, **kw):
            print(name, end = ': ')
            print(args[1:], end = ', ')
            print(kw)
            return func(*args, **kw)
        return newfun

    @staticmethod
    def __new__(metacls, name, parents, ns, **kwds):
        for i in ns:
            if callable(ns[i]):
                ns[i] = metacls.modify_func(ns[i], i)

        return super().__new__(metacls, name, parents, ns)

import sys
exec(sys.stdin.read())
