#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>

constexpr int N{100};
char map[N][N];
int dist[N][N];
inline int &pdist(std::pair<int, int> p) { return dist[p.second][p.first]; }

class MinHeap
{
    int _size{0};
    std::pair<int, int> arr[N * N];
    int idx[N][N];

    inline int &pidx(std::pair<int, int> p) { return idx[p.second][p.first]; }

    static inline int parent(int i) { return (i - 1) >> 1; }
    static inline int left(int i) { return (i << 1) + 1; }
    static inline int right(int i) { return (i + 1) << 1; }

    inline bool cmp(int i, int j) const
    {
        return pdist(arr[i]) < pdist(arr[j]);
    }

    void min_heapify(int i)
    {
        const int l{left(i)};
        const int r{right(i)};
        int min{l < _size && cmp(l, i) ? l : i};
        if (r < _size && cmp(r, min)) {
            min = r;
        }
        if (min != i) {
            const auto tmp = arr[i];
            arr[i] = arr[min];
            pidx(arr[i]) = i;
            arr[min] = tmp;
            pidx(tmp) = min;
            min_heapify(min);
        }
    }

    void decrease_key(int i, int key)
    {
        pdist(arr[i]) = key;
        int p{parent(i)};
        while (i && cmp(i, p)) {
            const auto tmp = arr[i];
            arr[i] = arr[p];
            pidx(arr[i]) = i;
            arr[p] = tmp;
            pidx(tmp) = p;
            i = p;
            p = parent(i);
        }
    }

  public:
    inline void init() { _size = 0; }
    inline bool empty() const { return _size == 0; }
    inline std::pair<int, int> min() const { return arr[0]; }

    std::pair<int, int> extract_min()
    {
        const auto p = arr[0];
        arr[0] = arr[--_size];
        pidx(arr[0]) = 0;
        min_heapify(0);
        return p;
    }

    void insert(std::pair<int, int> v)
    {
        const int val{pdist(v)};
        arr[_size] = v;
        pidx(v) = _size;
        pdist(v) = std::numeric_limits<int>::max();
        decrease_key(_size++, val);
    }

    inline void decrease_key(int x, int y, int key)
    {
        decrease_key(idx[y][x], key);
    }
} heap;

void init(int n)
{
    for (int i{0}; i < n; ++i) {
        for (int j{0}; j < n; ++j) {
            dist[i][j] = std::numeric_limits<int>::max();
        }
    }
}

constexpr int dx[] = {1, 0, -1, 0};
constexpr int dy[] = {0, 1, 0, -1};
int dijkstra(int n)
{
    init(n);

    heap.init();
    dist[0][0] = 0;
    for (int i{0}; i < n; ++i) {
        for (int j{0}; j < n; ++j) {
            heap.insert({i, j});
        }
    }

    while (!heap.empty()) {
        const auto p = heap.extract_min();
        for (int i{0}; i < 4; ++i) {
            const int nx{p.first + dx[i]};
            const int ny{p.second + dy[i]};
            if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
                if (pdist(p) + map[ny][nx] < dist[ny][nx]) {
                    dist[ny][nx] = pdist(p) + map[ny][nx];
                    heap.decrease_key(nx, ny, dist[ny][nx]);
                }
            }
        }
    }
    return dist[n - 1][n - 1];
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int n;
        std::cin >> n;

        for (int i{0}; i < n; ++i) {
            for (int j{0}; j < n; ++j) {
                std::cin >> map[i][j];
                map[i][j] -= '0';
            }
        }

        std::cout << '#' << test_case << ' ' << dijkstra(n) << '\n';
    }
    return 0;
}
