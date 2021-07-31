#include <iostream>

constexpr long INF{100000};
constexpr long N{100000};
constexpr int LGN{16};

struct Edge {
    long n;
    Edge *next;
    Edge *prev;
} pool[N];
Edge *edges[N + 1];
long pred[N + 1][LGN + 1];
long depth[N + 1];
long vcnt{0};
long ecnt{0};
Edge *alloc(long n, Edge *next = nullptr, Edge *prev = nullptr)
{
    Edge *e{&pool[ecnt++]};
    *e = {n, next, prev};
    return e;
}

long queue[N];
long start{0};
long end{0};
inline bool empty() { return start == end; }
inline void enqueue(long n) { queue[end++] = n; }
inline long dequeue() { return queue[start++]; }

void init()
{
    ecnt = start = end = 0;
    for (long v{1}; v <= vcnt; ++v) {
        edges[v] = nullptr;
        depth[v] = INF;
    }
}

inline void put(long child, long parent)
{
    pred[child][0] = parent;
    Edge *&e{edges[parent]};
    if (e) {
        e = alloc(child, e, e->prev);
        e->prev->next = e;
        e->next->prev = e;
    } else {
        e = alloc(child);
        e->next = e->prev = e;
    }
}

long lca(long u, long v)
{
    if (depth[u] < depth[v]) {
        const long tmp{u};
        u = v;
        v = tmp;
    }

    long i{depth[u] - depth[v]};
    for (int j{0}; i; i /= 2, ++j) {
        if (i % 2) {
            u = pred[u][j];
        }
    }

    if (u == v) {
        return u;
    }
    for (int i{LGN}; i >= 0; --i) {
        if (pred[u][i] != pred[v][i]) {
            u = pred[u][i];
            v = pred[v][i];
        }
    }
    return pred[u][0];
}

long bfs_path()
{
    long path{0};
    long prev{1};

    depth[1] = 0;
    enqueue(1);
    while (!empty()) {
        const long v{dequeue()};

        for (int i{1}; i <= LGN; ++i) {
            const long p{pred[v][i - 1]};
            pred[v][i] = p > 0 ? pred[p][i - 1] : 0;
        }

        path += depth[prev] + depth[v] - 2 * depth[lca(prev, v)];
        prev = v;

        if (!edges[v]) {
            continue;
        }

        bool init{true};
        Edge *start{edges[v]->prev};
        for (Edge *e{start}; init || e != start; e = e->prev) {
            init = false;
            if (depth[e->n] == INF) {
                depth[e->n] = depth[v] + 1;
                enqueue(e->n);
            }
        }
    }

    return path;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        std::cin >> vcnt;
        init();
        for (long n{2}; n <= vcnt; ++n) {
            long m;
            std::cin >> m;
            put(n, m);
        }

        std::cout << '#' << test_case << ' ' << bfs_path() << '\n';
    }
    return 0;
}
