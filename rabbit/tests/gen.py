import random

def genN():
    return random.randint(9500, 10000)

def taketh_away(lst, pred):
    for i in range(len(lst)):
        if pred(lst[i]):
            ans = lst[i]
            del lst[i]
            return ans
    raise Exception("No such element")
    
stuff = []
for i in range(1, 51):
    for j in range(1, 51):
        stuff.append( (i / j, i, j) )

random.shuffle(stuff);

s_wins = 20
c_wins = 15


for t in range(1, 51):
    if t < 10:
        s = "0" + str(t)
    else:
        s = str(t)
        
    tname = "rabbit." + s + ".in"
    
    data = str(genN()) + " "
    
    if t <= 4:
        data += "0 1"
    elif t <= 10:
        data += "1 0"
    elif t <= 14:
        data += "1 1"
    elif t == 30:
        data = f"{genN()} 50 50"
    else:
        if random.randint(0, 1) == 0 and s_wins > 0:
            res = taketh_away(stuff, lambda x: 8.0 > x[0] > 2.5)
        else:
            res = taketh_away(stuff, lambda x: 0.05 < x[0] < 0.3)
        data += f"{res[1]} {res[2]}"
    
    data += "\n"
    
    print(f"Test {t}: {data}\n")
    
    with open(tname, "w") as f:
        f.write(data)
    
    tname = "rabbit." + s + ".sol"
    
    with open(tname, "w") as f:
        pass
