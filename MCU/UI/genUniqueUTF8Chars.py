import os

S = set()
with open('CNCharactors.txt', 'r', encoding='utf8') as f:
    for line in f:
        S.update(line.strip())

with open('UniqueUTF8Chars.txt', 'w', encoding='utf8') as f:
    for s in S:
        f.write(s)
print(S)
