from decimal import *
getcontext().prec = 10000
init_c = Decimal.sqrt(Decimal(10005))

def PiGen():
    cur_s = Decimal(0)
    zn = init_c * Decimal(426880)
    ch1 = Decimal(13591409)
    ch2 = Decimal(1)
    k = 0
    tmp_1 = 0
    tmp_2 = 0
    while True:
        cur_s += (ch1*ch2)/zn
        yield Decimal(1)/cur_s
        tmp_1 += 6
        tmp_2 += 3
        k += 1
        zn *= Decimal(-262537412640768000*(tmp_2-2)*(tmp_2-1)*tmp_2*k*k*k)
        ch1 += Decimal(545140134)
        ch2 *= Decimal((tmp_1-5)*(tmp_1-4)*(tmp_1-3)*(tmp_1-2)*(tmp_1-1)*(tmp_1))
