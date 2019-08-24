import json

reg = json.load(open("data_col.json","r"))
mod = json.load(open("data_mod.json","r"))

regnames = {}

final = [[255,0,255]] * 256

for dat in reg:
    out = ["", 0, 0, 0, 0]
    for match in dat:
        for el, val in match.items():
            if el == "groupNum":
                if val == 1:
                    out[0] = match["content"]
                elif val == 2:
                    out[1] = int(match["content"])
                elif val == 3:
                    out[2] = int(match["content"])
                elif val == 4:
                    out[3] = int(match["content"])
                elif val == 5:
                    out[4] = int(match["content"])
    regnames[out[0]] = out
    final[out[1]] = [out[2], out[3], out[4]]

def lighten(c):
    return max(c+40, 255)

for dat in mod:
    for match in dat:
        for el, val in match.items():
            if el == "groupNum":
                if val == 2:
                    id = regnames[match["content"].rstrip(",")]
                    final[id[1]+128] = [id[2], id[3], id[4]]

for i, col in enumerate(final):
    print("[%3d, %3d, %3d], " % (col[0], col[1], col[2]),end="")
    if i % 8 == 7:
        print()
        