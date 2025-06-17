import random
import os


def generate_test_case(n, m, yn):
    """
    Generate a test case.
    - n: number of vertices.
    - m: number of edges.
    """
    edges = []
    permutation = list(range(n))
    for c in range(int(m/n)):
        random.shuffle(permutation)
        for i in range(n):
            edges.append((permutation[i-1], permutation[i]))
    random.shuffle(edges)

    if yn == 0:
        edges[0] = (edges[0][1], edges[0][0])
        random.shuffle(edges)

    return n, m, edges


def write_test_case(filename, n, m, edges):
    with open(filename, "w") as f:
        f.write(f"{n}\n{m}\n")
        for u, v in edges:
            f.write(f"{u} {v}\n")


def write_test_case_sol(filename, yn):
    with open(filename, "w") as f:
        f.write(f"{yn}\n")

def generate_test_suite():
    # List of (n, m, yn, cnt_tests) configurations.
    test_configs = [
    ]

    for i in range(3, 9):
        for j in range(2,5):
            if i*j <= 16:
                test_configs.append((i, i*j, 1, 1))
                test_configs.append((i, i*j, 0, 1))
    for i in range(13, 17):
        test_configs.append((i, i, 1, 1))
        test_configs.append((i, i, 0, 1))
    print(len(test_configs))

    for i in range(20):
        n = random.randint(50, 1000)
        d = random.randint(1, 10000//n)
        m = min(n*d, 10000 - 10000%n)
        test_configs.append((n, m, 1, 1))
        test_configs.append((n, m, 0, 1))

    print(len(test_configs))

    for i in range(20):

        n = random.randint(50, 10000)
        d = random.randint(1, 100000//n)
        m = min(n*d, 100000 - 100000%n)
        test_configs.append((n, m, 1, 1))
        test_configs.append((n, m, 0, 1))

    test_num = 1
    for n, m, yn, cnt_tests in test_configs:
        for _ in range(cnt_tests):
            n_val, m_val, edges = generate_test_case(n, m, yn)
            filename = os.path.join(
                os.path.dirname(os.path.abspath(__file__)),
                f"subway.{str(test_num).zfill(2)}.in",
            )
            write_test_case(filename, n_val, m_val, edges)

            filename = os.path.join(
                os.path.dirname(os.path.abspath(__file__)),
                f"subway.{str(test_num).zfill(2)}.sol",
            )
            write_test_case_sol(filename, yn)

            print(f"Generated {filename}")
            test_num += 1


if __name__ == "__main__":
    generate_test_suite()
