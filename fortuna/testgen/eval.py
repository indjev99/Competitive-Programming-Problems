total = 0

for i in range(1, 20 + 1):
    ts = f'{i:2}'.replace(' ', '0')
    with open(f'fortuna.{ts}.sol', 'r') as f:
        target = float(f.readline())
    with open(f'fortuna.{ts}.out', 'r') as f:
        cost = float(f.readline())

    rat = target / cost
    score = 5 * (0.5 * (rat ** 10) + 0.5 * (1 - (1 - rat) ** 0.15))
    total += score

    print(f'{ts}: {score:.2f}')

print(f'{total:.2f}')