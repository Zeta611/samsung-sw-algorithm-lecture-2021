#include <algorithm>
#include <functional>
#include <iostream>

constexpr long N{200000};
constexpr long MODULUS{20171109};

class Heap
{
    long _size{0};
    long arr[N + 1];

    std::function<bool(long, long)> cmp;

    static inline long parent(long i) { return (i - 1) >> 1; }
    static inline long left(long i) { return (i << 1) + 1; }
    static inline long right(long i) { return (i + 1) << 1; }

    void heapify(long i)
    {
        const long l{left(i)};
        const long r{right(i)};
        long ext{l < _size && cmp(l, i) ? l : i};
        if (r < _size && cmp(r, ext)) {
            ext = r;
        }
        if (ext != i) {
            const auto tmp = arr[i];
            arr[i] = arr[ext];
            arr[ext] = tmp;
            heapify(ext);
        }
    }

    void update_key(long i, long key)
    {
        arr[i] = key;
        long p{parent(i)};
        while (i && cmp(i, p)) {
            const auto tmp = arr[i];
            arr[i] = arr[p];
            arr[p] = tmp;
            i = p;
            p = parent(i);
        }
    }

  public:
    inline void init() { _size = 0; }
    inline bool empty() const { return _size == 0; }
    inline long top() const { return arr[0]; }

    long extract_top()
    {
        const long v{arr[0]};
        arr[0] = arr[--_size];
        heapify(0);
        return v;
    }

    inline void insert(long v) { update_key(_size++, v); }

    Heap(bool min)
    {
        if (min) {
            cmp = [this](long i, long j) -> bool { return arr[i] < arr[j]; };
        } else {
            cmp = [this](long i, long j) -> bool { return arr[i] > arr[j]; };
        }
    }
} min_heap(true), max_heap(false);

long median(long n)
{
    min_heap.init();
    max_heap.init();

    long sum{0};
    long med;
    std::cin >> med;
    while (n--) {
        long a, b;
        std::cin >> a >> b;
        if (a < b) {
            std::swap(a, b);
        }

        if (med <= b) {
            max_heap.insert(med);
            min_heap.insert(a);
            min_heap.insert(b);
            med = min_heap.extract_top();
        } else if (a <= med) {
            max_heap.insert(a);
            max_heap.insert(b);
            min_heap.insert(med);
            med = max_heap.extract_top();
        } else {
            max_heap.insert(b);
            min_heap.insert(a);
        }
        sum += med % MODULUS;
    }
    return sum % MODULUS;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        long n;
        std::cin >> n;
        std::cout << '#' << test_case << ' ' << median(n) << '\n';
    }
    return 0;
}
