import argparse
import cowsay
from cowsay import Option

parser = argparse.ArgumentParser(description='drawing some wonderfull cows')
parser.add_argument('message',metavar='message', nargs = '?', type=str)
parser.add_argument('-T', metavar='tongue', type = str)
parser.add_argument('-e', metavar='eyes', type = str)
parser.add_argument('-W', metavar='width', type = int)
parser.add_argument('-f', metavar='cowfile', type = str)
parser.add_argument('-l', action = 'store_true')
parser.add_argument('-n', action = 'store_true')
parser.add_argument('-b', action = 'store_true')
parser.add_argument('-d', action = 'store_true')
parser.add_argument('-g', action = 'store_true')
parser.add_argument('-p', action = 'store_true')
parser.add_argument('-s', action = 'store_true')
parser.add_argument('-t', action = 'store_true')
parser.add_argument('-w', action = 'store_true')
parser.add_argument('-y', action = 'store_true')

args = parser.parse_args()

if args.l == True:
    print(cowsay.list_cows())

else:
    cur_preset = None
    for i in ['b','d','g','p','s','t','w','y']:
        if getattr(args,i):
            if cur_preset == None:
                cur_preset = i
            else:
                cur_preset += i
    
    if args.W == None:
        args.W = 40

    if args.T == None:
        args.T = Option.tongue

    if args.e == None:
        args.e = Option.eyes

    print(cowsay.cowsay(preset = cur_preset, message = args.message, eyes=args.e, tongue=args.T, width=args.W, wrap_text = not(args.n), cowfile = args.f))
