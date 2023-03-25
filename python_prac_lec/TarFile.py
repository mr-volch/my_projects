import sys
from tarfile import *
import io

a = sys.stdin.read()
a = a.replace(' ','')
a = a.replace('\n','')
b = io.BytesIO(bytes.fromhex(a))
size = 0
amount = 0
with TarFile.open(mode="r|*", fileobj=b) as f:
    for file in f.getmembers():
        size += file.size
        if file.isfile():
            amount += 1
    
print(size, amount)
