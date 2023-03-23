import sys

a = sys.stdin.buffer.read()
print(a.decode('utf-8', errors = 'replace').encode('latin1', errors = 'replace').decode('CP1251', errors = 'replace'))
