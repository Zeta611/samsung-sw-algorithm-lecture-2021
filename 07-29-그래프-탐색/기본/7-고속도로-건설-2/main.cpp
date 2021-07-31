#include <iostream>

constexpr long INF{2147483647};
constexpr long V{50000};
constexpr long E{200000};

struct Edge {
    long vertex;
    long cost;
    Edge *next;
} pool[E * 2];
long ecnt{0};
long vcnt{0};
Edge *vertices[V + 1];
long dist[V + 1];
Edge *alloc(long vertex, long cost, Edge *next = nullptr)
{
    Edge *e{&pool[ecnt++]};
    *e = {vertex, cost, next};
    return e;
}

void init()
{
    ecnt = 0;
    for (long i{1}; i <= vcnt; ++i) {
        vertices[i] = nullptr;
        dist[i] = INF;
    }
}

inline void add_edge(long s, long e, long c)
{
    vertices[s] = alloc(e, c, vertices[s]);
    vertices[e] = alloc(s, c, vertices[e]);
}

class BinaryMinHeap
{
    unsigned _size{0};
    long arr[V];
    long idx[V];

    static inline unsigned parent(unsigned i) { return (i - 1) >> 1; }
    static inline unsigned left(unsigned i) { return (i << 1) + 1; }
    static inline unsigned right(unsigned i) { return (i + 1) << 1; }

    void min_heapify(unsigned i)
    {
        const unsigned l{left(i)};
        const unsigned r{right(i)};
        unsigned smallest{l < _size && dist[arr[l]] < dist[arr[i]] ? l : i};
        if (r < _size && dist[arr[r]] < dist[arr[smallest]]) {
            smallest = r;
        }
        if (smallest != i) {
            const auto tmp = arr[i];
            arr[i] = arr[smallest];
            arr[smallest] = tmp;
            idx[arr[i]] = i;
            idx[arr[smallest]] = smallest;
            min_heapify(smallest);
        }
    }

    void decrease_key(unsigned i, long key)
    {
        if (key > dist[arr[i]]) {
            return; // Error
        }
        dist[arr[i]] = key;
        unsigned p{parent(i)};
        while (i && dist[arr[p]] > dist[arr[i]]) {
            const auto tmp = arr[i];
            arr[i] = arr[p];
            arr[p] = tmp;
            idx[arr[i]] = i;
            idx[arr[p]] = p;
            i = p;
            p = parent(i);
        }
    }

  public:
    inline void init() { _size = 0; }
    inline bool empty() { return _size == 0; }
    inline bool contains(long v) { return idx[v] >= 0; }

    long extract_min()
    {
        if (!_size) {
            return 0;
        }
        const long v{arr[0]};
        arr[0] = arr[--_size];
        idx[arr[0]] = 0;
        idx[v] = -1;
        min_heapify(0);
        return v;
    }

    inline void decrease_key(long v) { decrease_key(idx[v], dist[v]); }

    void insert(long v)
    {
        arr[_size] = v;
        idx[v] = _size;
        const long d{dist[v]};
        dist[v] = INF;
        decrease_key(_size++, d);
    }
} heap;

long min_cost()
{
    long cost{0};
    dist[1] = 0;
    for (long i{1}; i <= vcnt; ++i) {
        heap.insert(i);
    }
    while (!heap.empty()) {
        const long v{heap.extract_min()};
        cost += dist[v];

        for (Edge *e{vertices[v]}; e; e = e->next) {
            const long u{e->vertex};
            const long c{e->cost};
            if (heap.contains(u) && dist[u] > c) {
                dist[u] = c;
                heap.decrease_key(u);
            }
        }
    }
    return cost;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        long ecnt;
        std::cin >> vcnt >> ecnt;
        init();
        for (long i{0}; i < ecnt; ++i) {
            long s, e, c;
            std::cin >> s >> e >> c;
            add_edge(s, e, c);
        }
        std::cout << '#' << test_case << ' ' << min_cost() << '\n';
    }
    return 0;
}
