#!/usr/bin/python3 -P
import collections
import dataclasses
import enum
import heapq
import math
import os
import random
import subprocess
import sys
import typing


class Component(list[int]):
    __slots__ = ()

    def __eq__(self, other: object) -> bool:
        assert False

    def __ne__(self, other: object) -> bool:
        assert False

    def __lt__(self, other: object) -> bool:
        assert type(other) is Component
        if (n1 := len(self)) != (n2 := len(other)):
            return n1 > n2
        return id(self) < id(other)

    def __le__(self, other: object) -> bool:
        assert type(other) is Component
        if (n1 := len(self)) != (n2 := len(other)):
            return n1 >= n2
        return id(self) <= id(other)

    def __gt__(self, other: object) -> bool:
        assert type(other) is Component
        if (n1 := len(self)) != (n2 := len(other)):
            return n1 < n2
        return id(self) > id(other)

    def __ge__(self, other: object) -> bool:
        assert type(other) is Component
        if (n1 := len(self)) != (n2 := len(other)):
            return n1 <= n2
        return id(self) >= id(other)


class ComponentSet:
    """Implements a list-based disjoint set."""
    __slots__ = ('components',)
    components: list[Component]

    def __init__(self, /, n: int) -> None:
        cs = self.components = [Component() for _ in range(n)]
        cs.sort()
        for (i, c) in enumerate(cs):
            c.append(i)

    def pop_max(self, /) -> Component:
        """Pop the largest component."""
        return heapq.heappop(self.components)

    def pop_random(self, /) -> Component:
        """Pop a random component, uniform on components."""
        i = random.randrange(len(cs := self.components))
        top = heapq.heappop(cs)
        if i == 0:
            return top
        r = cs[i := i-1]
        while i:
            cs[i] = cs[j := (i - 1) // 2]
            i = j
        cs[0] = top
        return r
    
    def push(self, /, cs: list[Component]) -> None:
        """Push a list of components as a single component."""
        cs.sort()
        c0 = cs[0]
        for ci in cs[1:]:
            c0.extend(ci)
        heapq.heappush(self.components, c0)


class UnionFind:
    """Implements a union-find-based disjoint set."""
    __slots__ = ('parent',)
    parent: list[int]

    def __init__(self, n: int, /) -> None:
        self.parent = list(range(n))

    def find(self, /, i: int) -> int:
        j = i
        while True:
            if (k := self.parent[j]) == j:
                break
            j = k
        while i != j:
            k = self.parent[i]
            self.parent[i] = j
            i = k
        return j

    def union(self, /, i: int, j: int) -> bool:
        i = self.find(i)
        j = self.find(j)
        if i == j:
            return False
        self.parent[j] = i
        return True


class VertexOrder(enum.Enum):
    Random = 0
    Sorted = 1
    Reversed = 2


@dataclasses.dataclass(kw_only=True, slots=True, eq=False, match_args=False)
class Generator:
    n: int
    m: int
    r: int

    prob_max_first: float = 0.9
    prob_max_rest: float = 0.1

    prob_vert_first: float = 0.0
    prob_vert_last: float = 0.0

    prob_sub_bipartite: float = 0.45
    prob_sub_cycle: float = 0.45

    prob_comp_tree: float = 0.0

    edge_count_ok: float = 0.99

    vert_min: float
    vert_extra: float = 3.0
    vert_scale: float = 0.5

    vert_order: VertexOrder = VertexOrder.Random

    def pick_edge_count(self, /) -> int:
        return self.r - (random.randrange(100) == 0)

    def pick_comp_count(self, /, edge_count: int) -> int:
        if self.prob_comp_tree > 0 and random.random() < self.prob_comp_tree:
            return edge_count + 1
        extra = random.gammavariate(self.vert_extra, self.vert_scale)
        return round(self.vert_min + extra)

    def pick_comp(self, /, graph: ComponentSet, i: int) -> Component:
        prob_max = self.prob_max_first if i == 0 else self.prob_max_rest
        if random.random() < prob_max:
            return graph.pop_max()
        return graph.pop_random()

    def pick_vert(self, /, component: Component) -> int:
        rnd = random.random()
        if rnd < self.prob_vert_first:
            return component[0]
        if rnd < self.prob_vert_first + self.prob_vert_last:
            return component[-1]
        return random.choice(component)

    def pick_sub(self, /, n: int, m: int) -> typing.Iterable[tuple[int, int]]:
        rnd = random.random()
        if rnd < self.prob_sub_bipartite:
            # For a limited number of edges, bipartite graphs tend to have the most
            # spanning trees.
            l = []
            for i in range(n // 2):
                for j in range(n // 2, n):
                    l.append((i, j))
            random.shuffle(l)
            yield from l
        elif rnd < self.prob_sub_bipartite + self.prob_sub_cycle:
            # Start with a cycle, then a cycle with a larger step, and so on.
            vert = list(range(n))
            for k in range(1, n):
                if k > 1:
                    if math.gcd(k, n) != 1:
                        continue
                    random.shuffle(vert)
                for i in vert:
                    yield (i, (i+k)%n)
        for i in range(1000):
            a = random.randrange(n)
            if random.randrange(10):
                b = random.randrange(n - 1)
                if b >= a:
                    b += 1
            else:
                # With low probabiltiy, generate loops.
                b = random.randrange(n)
            yield (a, b)

    def try_generate(self, /) -> list[list[tuple[int, int]]] | None:
        graph = ComponentSet(self.n)
        edges: set[tuple[int, int]] = set()
        levels: list[list[tuple[int, int]]] = []
        while True:
            if len(graph.components) == 1:
                got_m = len(edges)
                if got_m >= self.m * self.edge_count_ok - 1:
                    print(f'Down to one component at {len(edges)} edges; satisfactory', file=sys.stderr)
                    return levels
                if self.vert_scale >= 0.3 or self.vert_min <= 1:
                    print(f'Down to one component at {len(edges)} edges; retrying with smaller vertex set scale', file=sys.stderr)
                    self.vert_scale = round(self.vert_scale / 1.01, 4)
                else:
                    print(f'Down to one component at {len(edges)} edges; retrying with smaller vertex set base', file=sys.stderr)
                    self.vert_scale = 0.5
                    self.vert_min -= 1
                return None

            if len(edges) >= self.m:
                print(f'Down to {len(graph.components)} components; retrying with larger vertex sets', file=sys.stderr)
                self.vert_scale = round(self.vert_scale * 1.01, 4)
                return None

            nedge = self.pick_edge_count()
            nedge = min(max(nedge, 1), self.r, self.m - len(edges))

            ncomp = min(max(self.pick_comp_count(nedge), 2), len(graph.components))
            selected = [self.pick_comp(graph, i) for i in range(ncomp)]

            uf = UnionFind(len(selected))

            level = []
            for (cid1, cid2) in self.pick_sub(ncomp, nedge):
                u = self.pick_vert(selected[cid1])
                v = self.pick_vert(selected[cid2])
                if u > v:
                    u, v = v, u
                if u == v or (edge := (u, v)) in edges:
                    assert uf.find(cid1) == uf.find(cid2)
                    continue
                uf.union(cid1, cid2)
                edges.add(edge)
                level.append(edge)
                if len(level) == nedge:
                    break

            levels.append(level)

            groups = collections.defaultdict(list)
            for (cid, c) in enumerate(selected):
                groups[uf.find(cid)].append(c)
            for grp in groups.values():
                graph.push(grp)

    def generate(self, /) -> list[tuple[int, int, int]]:
        while (edges := self.try_generate()) is None:
            pass

        # TODO add filler edges
        # (unused costs, loops within components?)

        have_costs = set()
        cmap: list[int] = []
        while len(cmap) < len(edges):
            c = random.randint(1, 10**6)
            if c in have_costs:
                continue
            cmap.append(c)
            have_costs.add(c)
        cmap.sort()

        vmap = list(range(1, self.n+1))
        match self.vert_order:
            case VertexOrder.Random:
                random.shuffle(vmap)
            case VertexOrder.Sorted:
                pass
            case VertexOrder.Reversed:
                vmap.reverse()

        edge_list = []
        for (c, uvs) in zip(cmap, edges):
            for (u, v) in uvs:
                u = vmap[u]
                v = vmap[v]
                if u > v:
                    u, v = v, u
                edge_list.append((u, v, c))
        random.shuffle(edge_list)

        return edge_list


type EdgeList = list[tuple[int, int, int]]


def generate_groups(n: int, m: int, r: int) -> typing.Iterable[Generator]:
    if r < 5:
        # We can't do anything.
        return

    vert_min = n / m * (r - 0.02) - 0.5

    # First try to fiddle with the subgraphs
    for (pb, pc) in [
        (0.45, 0.45),
        (0.9, 0.05),
        (0.05, 0.9),
        (0.0, 0.0),
    ]:
        yield Generator(
            n = n,
            m = m,
            r = r,
            vert_min = vert_min,
            prob_sub_bipartite = pb,
            prob_sub_cycle = pc,
        )

    # Then try fun vertex orders.
    for (pmax1, pmax2, pfirst, plast, vo) in [
        (0.99, 0.0, 0.98, 0.01, VertexOrder.Sorted),
        (0.99, 0.0, 0.01, 0.98, VertexOrder.Sorted),
        (0.99, 0.0, 0.98, 0.01, VertexOrder.Reversed),
        (0.99, 0.0, 0.01, 0.98, VertexOrder.Reversed),
        (0.0, 0.99, 0.98, 0.01, VertexOrder.Random),
        (0.0, 0.99, 0.01, 0.98, VertexOrder.Random),
    ]:
        yield Generator(
            n = n,
            m = m,
            r = r,
            vert_min = vert_min,
            prob_max_first = pmax1,
            prob_max_rest = pmax2,
            prob_vert_first = pfirst,
            prob_vert_last = plast,
            vert_order = vo,
        )

    # Now generate something with some more trees.
    yield Generator(
        n = n,
        m = m,
        r = r,
        vert_min = vert_min,
        edge_count_ok = 0.5,
        prob_comp_tree = 0.5,
    )


def generate_random(n: int, m: int, r: int) -> EdgeList:
    # First generate the edges.
    edges = []

    # Ensure we have a spanning tree.
    for v in range(1, n):
        u = random.randrange(0, v)
        edges.append(p := (u, v))

    # Fill the rest randomly
    edge_set = set(edges)
    while len(edges) < m:
        u = random.randrange(n)
        v = random.randrange(n - 1)
        if v >= u:
            v += 1
        else:
            u, v = v, u
        if (p := (u, v)) in edge_set:
            continue
        edges.append(p)
        edge_set.add(p)

    random.shuffle(edges)

    # Now assign random costs
    costs = set()
    edge_iter = iter(edges)

    vmap = list(range(1, n+1))
    random.shuffle(vmap)

    edge_list = []
    while True:
        c = random.randint(1, 10**6)
        if c in costs:
            continue
        costs.add(c)
        to_add = r
        for (u, v) in edge_iter:
            u = vmap[u]
            v = vmap[v]
            if u > v:
                u, v = v, u
            edge_list.append((u, v, c))
            to_add -= 1
            if to_add == 0:
                break
        else:
            break

    random.shuffle(edge_list)
    return edge_list


def generate_evil(n: int, m: int, r: int) -> EdgeList:
    maxk = 12
    n1 = 1 << maxk
    n2 = n - n1
    assert n2 % 8 == 0

    cost = 0
    edges = []

    vmap = list(range(1, n1 + n2 + 1))
    random.shuffle(vmap)

    # Step 1: build a binary tree out of the first 2^maxk vertices
    for k in range(maxk):
        for i in range(0, n1, 1 << (k + 1)):
            u = vmap[i]
            v = vmap[i + (1 << k)]
            if u > v:
                u, v = v, u
            edges.append((u, v, cost))
            cost += 1

    # Step 2: add the rest of the vertices, in groups of 8, together to the big group.
    for b in range(n1, n1+n2, 8):
        for j in range(5):
            this_i = list(range(3))
            random.shuffle(this_i)
            this_r = r // 5 + (j < r % 5) - 1
            del this_i[this_r:]
            if j < 3 and j not in this_i:
                this_i[0] = j
            this_i.append(3)
            for i in this_i:
                if i == 3:
                    u = random.randrange(n1)
                else:
                    u = b + i
                v = b + j + 3
                u = vmap[u]
                v = vmap[v]
                if u > v:
                    u, v = v, u
                edges.append((u, v, cost))
        cost += 1

    random.shuffle(edges)
    return edges


secret = 'srAd4vYu9FrRZLuLTmX79BFUpz7YY04U'
test_id: int = 0

def write_test(sub: int, kind: str, i: int, n: int, m: int, r: int, make_edges: typing.Callable[[], EdgeList], *, desc: str = '') -> None:
    global test_id
    test_id = (gi := test_id) + 1

    path = f'tests/nmst.{gi:03}.in'
    if os.path.exists(path):
        return

    print(f'Generating {path}') 
    random.seed(f'{secret} sub{sub} {kind} {i}')
    edge_list = make_edges()

    os.makedirs(os.path.dirname(path), exist_ok=True)

    with open(path, 'x') as f:
        f.write(f'{n} {len(edge_list)}\n')
        for (u, v, c) in edge_list:
            f.write(f'{u} {v} {c}\n')

    with open(path, 'rb', buffering=0) as f:
        out = subprocess.check_output(['./bin/check-test'], stdin=f, text=True).splitlines()
        assert out[0].startswith('R=')
        got_r = int(out[0][2:])
        if got_r != r:
            raise RuntimeError(f'Expected R={r}, got {got_r}')

    with open(path[:-3] + '.desc', 'w') as f:
        f.write(f'subtask {sub!s}\nkind {kind}\n{desc!s}')


def generate() -> None:
    for (sub, n, m, r, n_random, n_evil) in [
        (1, 100000, 200000, 1, 10, 0),
        (2, 10, 20, 10, 10, 0),
        (3, 2000, 4000, 10, 2, 0),
        (4, 100000, 200000, 10, 2, 2),
        (5, 100000, 200000, 12, 2, 2),
        (6, 100000, 200000, 14, 2, 2),
        (7, 100000, 200000, 16, 2, 2),
    ]:
        for (i, gen) in enumerate(generate_groups(n, m, r)):
            write_test(sub, 'groups', i, n, m, r, lambda: gen.generate(), desc=f'{gen!r}\n')
        for i in range(n_random):
            scale = 1
            if sub == 1:
                scale = 10 ** ((n_random - i - 1) // 2)
            write_test(sub, 'random', i, n // scale, m // scale, r, lambda: generate_random(n // scale, m // scale, r))
        for i in range(n_evil):
            write_test(sub, 'evil', i, n, m, r, lambda: generate_evil(n, m, r))


generate()
