def fix(n):
    def dec(fun):
        def newfun(*args, **kw):
            args = list(args)
            for i in range(len(args)):
                if isinstance(args[i], float):
                    args[i] = round(args[i], n)
            for i in kw:
                if isinstance(kw[i], float):
                    kw[i] = round(kw[i], n)
            a = fun(*args, **kw)
            if isinstance(a, float):
                a = round(a,n)
            return a
        return newfun
    return dec
