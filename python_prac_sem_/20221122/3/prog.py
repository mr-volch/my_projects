

with open('tests/'+input(), 'rb') as f:
    check_fields = [b'RIFF', b'WAVE', b'fmt ', b'data']
    cur = f.read(44)
    if len(cur) != 44:
        print('NO')
    else:
        for i in check_fields:
            if i not in cur:
                print('NO')
                break
        else:
            s = int.from_bytes(cur[4:8], 'little')
            t = int.from_bytes(cur[20:22], 'little')
            c = int.from_bytes(cur[22:24], 'little')
            r = int.from_bytes(cur[24:28], 'little')
            b = int.from_bytes(cur[34:36], 'little')
            d = int.from_bytes(cur[40:44], 'little')
            print(f"Size={s}, Type={t}, Channels={c}, Rate={r}, Bits={b}, Data size={d}")
