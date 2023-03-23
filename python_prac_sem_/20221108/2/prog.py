class InvalidInput(Exception): pass

class BadTriangle(Exception): pass

def triangleSquare(a):
    try:
        try:
            a = eval(a)
            a = op_ins_2(a)
            if not a:
                raise Exception
        except:
            raise InvalidInput
        else:
            A,B,C = a[0],a[1],a[2]
            cur = [A,B,C]
            res = [((cur[i][0] - cur[j][0])**2 + (cur[i][1] - cur[j][1])**2)**0.5 for i in range(2) for j in range(i+1,3)]
            if res[0] < res[1] + res[2] and res[1] < res[0] + res[2] and res[2] < res[0] + res[1]:
                S = abs((B[0]-A[0])*(C[1]-A[1])-(C[0]-A[0])*(B[1]-A[1]))/2
            else:
                raise BadTriangle
    except InvalidInput:
        print("Invalid input")
    except BadTriangle:
        print("Not a triangle")
    else:
        print('%.2f' %S)

def op_ins_2(a):
    if isinstance(a, tuple):
        if len(a) == 3:
            for i in range(3):
                if not isinstance(a[i], tuple):
                    return ''
                if not(len(a[i]) == 2 and isinstance(a[i][0], float | int) and isinstance(a[i][1], float | int)):
                    return ''
            return a
    return ''

try:
    while a:= input():
        triangleSquare(a)
except EOFError:
    pass
    
