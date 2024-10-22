import os
import numpy as np

S = set()
with open('CNCharactors.txt', 'r', encoding='utf8') as f:
    for line in f:
        S.update(line.strip())
l = np.sort(list(S))

with open('UniqueUTF8Chars.txt', 'w', encoding='utf8') as f:
    for s in l:
        f.write(s)
print(l)
print(f'字符数：{len(l)}')
