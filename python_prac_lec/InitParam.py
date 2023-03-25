from inspect import signature as sig
from inspect import Parameter
import types


class init(type):

    def modify_func(func, name):
        def newfun(*args, **kw):
            params = sig(func).parameters
            n = 1
            for i in params:
                if n == 1:
                    n += 1
                    continue
                if n > len(args) and params[i].name not in kw:
                    tmp = params[i].annotation
                    if type(tmp) == types.GenericAlias:
                        tmp = tmp.__origin__
                    if params[i].default != Parameter.empty:
                        kw[params[i].name] = params[i].default
                        n += 1
                        continue
                    try:
                        tmp = tmp()
                    except:
                        tmp = None
                    kw[params[i].name] = tmp
                n += 1
            res = func(*args, **kw)
            return res
        return newfun

    @staticmethod
    def __new__(metacls, name, parents, ns, **kwds):
        for i in ns:
            if isinstance(ns[i], type(metacls.modify_func)):
                param = 3
                if len(kwds) > 0:
                    param = kwds['ndigits']
                ns[i] = metacls.modify_func(ns[i], i)

        return super().__new__(metacls, name, parents, ns)
