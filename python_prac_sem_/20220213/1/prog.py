import sys
import zlib
import argparse
from glob import iglob
from os.path import basename, dirname
import os


def print_the_tree(tree_id):
    with open('.git/objects' + '/' + tree_id[:2] + '/' + tree_id[2:], 'rb') as filename:
        a = zlib.decompress(filename.read())
        header, _, body = a.partition(b'\x00')
        kind, size = header.split()
        tail = body
        while tail:
            treeobj, _, tail = tail.partition(b'\x00')
            tmode, tname = treeobj.split()
            num, tail = tail[:20], tail[20:]
            SHIFT = "  "
            print(f"{SHIFT}{tname.decode()} {tmode.decode()} {num.hex()}")
    return None

parser = argparse.ArgumentParser(description='monitoring git repo')

parser.add_argument('-B', metavar='branch', type = str)

args = parser.parse_args()

if args.B == None:
    for branch in iglob(".git/refs/heads/*"):
        print(branch.split('/')[-1])
#2
else:
    with open(".git/refs/heads/" + args.B, 'rb') as f:
        d = (f.read())
        commit_id = d[:-1].decode('UTF-8')

    with open(".git/objects/" + commit_id[:2] + '/' + commit_id[2:], 'rb') as commit_file:
        a = zlib.decompress(commit_file.read())
        cur = (b'\n'.join(a.split(b'\n')[1:]))
        tmp = (cur.decode())
        print('tree', a.split()[2].decode())
        print(tmp)

###3
##else:
##    for f in iglob(".git/objects/??/*"):
##        with open(f, 'rb') as filename:
##            a = zlib.decompress(filename.read())
##
##    with open(".git/refs/heads/" + args.B, 'rb') as f:
##        d = (f.read())
##        commit_id = d[:-1].decode('UTF-8')
##
##    with open(".git/objects/" + commit_id[:2] + '/' + commit_id[2:], 'rb') as commit_file:
##        a = zlib.decompress(commit_file.read())
##        tmp = a.split()
##        print_the_tree(tmp[2].decode())

###4                                
##else:
##    for f in iglob(".git/objects/??/*"):
##        with open(f, 'rb') as filename:
##            a = zlib.decompress(filename.read())
##
##    with open(".git/refs/heads/" + args.B, 'rb') as f:
##        d = (f.read())
##        commit_id = d[:-1].decode('UTF-8')
##
##    with open(".git/objects/" + commit_id[:2] + '/' + commit_id[2:], 'rb') as commit_file:
##        a = zlib.decompress(commit_file.read())
##        tmp = a.split()
##        print("TREE for commit", commit_id)
##        print_the_tree(tmp[2].decode())
##        while tmp[3] == b'parent':
##            commit_id = tmp[4].decode()
##            with open(".git/objects/" + commit_id[:2] + '/' + commit_id[2:], 'rb') as commit_file:
##                a = zlib.decompress(commit_file.read())
##                tmp = a.split()
##                print("TREE for commit", commit_id)
##                print_the_tree(tmp[2].decode())                                
