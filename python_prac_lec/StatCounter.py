def statcounter():
    dict_1 = {}
    def dec(fun):
        def newfun(*args, **kw):
            dict_1[fun] += 1
            return fun(*args, **kw)
        return newfun

    fun = yield(dict_1)
    dict_1[fun] = 0
    while True:
        fun = yield(dec(fun))
        if fun not in dict_1:
            dict_1[fun] = 0
