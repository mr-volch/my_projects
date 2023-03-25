import numbers

class fixed(type):

    def modify_func(func, name, param):
        def newfun(*args, **kw):
            res = func(*args, **kw)
            if isinstance(res, numbers.Real):
                res = round(res, param)
            return res
        return newfun

    @staticmethod
    def __new__(metacls, name, parents, ns, **kwds):
        for i in ns:
            if isinstance(ns[i], type(metacls.modify_func)):
                param = 3
                if len(kwds) > 0:
                    param = kwds['ndigits']
                ns[i] = metacls.modify_func(ns[i], i, param)

        return super().__new__(metacls, name, parents, ns)
