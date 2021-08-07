constexpr int MAX_USERS{100000};

int income[MAX_USERS];

class MaxHeap
{
    int _size{0};
    int arr[MAX_USERS];

    static inline int parent(int i) { return (i - 1) >> 1; }
    static inline int left(int i) { return (i << 1) + 1; }
    static inline int right(int i) { return (i + 1) << 1; }

    inline bool cmp(int i, int j) const
    {
        if (income[arr[i]] == income[arr[j]]) {
            return arr[i] < arr[j];
        }
        return income[arr[i]] > income[arr[j]];
    }

    void max_heapify(int i)
    {
        const int l{left(i)};
        const int r{right(i)};
        int max{l < _size && cmp(l, i) ? l : i};
        if (r < _size && cmp(r, max)) {
            max = r;
        }
        if (max != i) {
            const int tmp{arr[i]};
            arr[i] = arr[max];
            arr[max] = tmp;
            max_heapify(max);
        }
    }

    void increase_key(int i, int key)
    {
        income[arr[i]] = key;
        int p{parent(i)};
        while (i && cmp(i, p)) {
            const int tmp{arr[i]};
            arr[i] = arr[p];
            arr[p] = tmp;
            i = p;
            p = parent(i);
        }
    }

  public:
    inline void init() { _size = 0; }
    inline bool empty() const { return _size == 0; }
    inline int max() const { return arr[0]; }

    int extract_max()
    {
        const int key{arr[0]};
        arr[0] = arr[--_size];
        max_heapify(0);
        return key;
    }

    void insert(int user)
    {
        arr[_size] = user;
        const int key{income[user]};
        income[user] = 0;
        increase_key(_size++, key);
    }
} heap;

void init() { heap.init(); }

void addUser(int user, int key)
{
    income[user] = key;
    heap.insert(user);
}

int getTop10(int result[10])
{
    int cnt{0};
    while (cnt < 10 && !heap.empty()) {
        result[cnt++] = heap.extract_max();
    }
    for (int i{0}; i < cnt; ++i) {
        heap.insert(result[i]);
    }
    return cnt;
}
