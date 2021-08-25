#define SWAP(a, b) (SWAP_TMP = (a), (a) = (b), (b) = SWAP_TMP)
int SWAP_TMP;
#define LEFT(i) (((i) << 1) + 1)
#define RIGHT(i) (((i) + 1) << 1)
#define PARENT(i) (((i)-1) >> 1)
// `true` if `p1` has a higher priority than `p2`.
#define CMP(p1, p2)                                                            \
    (products[p1].price < products[p2].price ||                                \
     products[p1].price == products[p2].price &&                               \
         products[p1].id < products[p2].id)

constexpr int MAX_USERS{1000};
constexpr int MAX_AREAS{10};
constexpr int MAX_PRODUCT_ID{1000000000};
constexpr int MAX_FRIENDS{20};
constexpr int MAX_PRICE{1000};
constexpr int MAX_PRODUCTS{40000};
constexpr int TABLE_SIZE{200003};

struct Product {
    int id;
    int area;
    int price;
    bool reserved;
} products[MAX_PRODUCTS + 1];
int product_cnt;
inline int product_alloc(int id, int area, int price)
{
    products[++product_cnt].id = id;
    products[product_cnt].area = area;
    products[product_cnt].price = price;
    products[product_cnt].reserved = false;
    return product_cnt;
}

struct BinaryHeap {
    int size;
    int container[MAX_PRODUCTS];

    void heapify(int i)
    {
        const int l{LEFT(i)};
        const int r{RIGHT(i)};
        int m{l < size && CMP(container[l], container[i]) ? l : i};
        if (r < size && CMP(container[r], container[m])) {
            m = r;
        }
        if (m != i) {
            SWAP(container[i], container[m]);
            heapify(m);
        }
    }

    void update(register int i, int index)
    {
        register int p{PARENT(i)};
        while (i && CMP(index, container[p])) {
            SWAP(container[i], container[p]);
            i = p;
            p = PARENT(i);
        }
    }

    inline void push(int index)
    {
        container[size] = index;
        update(size++, index);
    }

    inline void pop()
    {
        container[0] = container[--size];
        heapify(0);
    }
} heap[MAX_AREAS + 1];

int hash_table[TABLE_SIZE];
int get(int key)
{
    int hash{key % TABLE_SIZE};
    int index;
    while ((index = hash_table[hash])) {
        if (products[index].id == key) {
            return index;
        }
        hash = (hash + 1) % TABLE_SIZE;
    }
    return 0;
}
void put(int key, int index)
{
    int hash{key % TABLE_SIZE};
    while (hash_table[hash]) {
        hash = (hash + 1) % TABLE_SIZE;
    }
    hash_table[hash] = index;
}

int friend_list[MAX_USERS + 1][MAX_FRIENDS + 1];
int reservation_cnt[MAX_USERS + 1][MAX_AREAS + 1];

int user_cnt;
int area_cnt;
void init(int n, int m)
{
    user_cnt = n;
    area_cnt = m;
    product_cnt = 0;

    for (int i{1}; i <= area_cnt; ++i) {
        heap[i].size = 0;
    }
    for (register int i{0}; i < TABLE_SIZE; ++i) {
        hash_table[i] = 0;
    }
    for (register int i{1}; i <= user_cnt; ++i) {
        for (int j{0}; j <= MAX_FRIENDS; ++j) {
            friend_list[i][j] = 0;
        }
        for (int j{0}; j <= area_cnt; ++j) {
            reservation_cnt[i][j] = 0;
        }
    }
}

void befriend(int user1, int user2)
{
    int i{0};
    while (friend_list[user1][i]) {
        ++i;
    }
    friend_list[user1][i] = user2;

    i = 0;
    while (friend_list[user2][i]) {
        ++i;
    }
    friend_list[user2][i] = user1;
}

void add(int product, int area, int price)
{
    const int index{product_alloc(product, area, price)};
    put(product, index);
    heap[area].push(index);
}

void reserve(int user, int product)
{
    const int index{get(product)};
    ++reservation_cnt[user][products[index].area];
    products[index].reserved = true;
}

int reservation_cnt_sum[MAX_AREAS + 1];
int recommend(int user)
{
    for (int i{1}; i <= area_cnt; ++i) {
        reservation_cnt_sum[i] = 0;
    }

    for (int i{0}, friend_; (friend_ = friend_list[user][i]); ++i) {
        for (int j{1}; j <= area_cnt; ++j) {
            reservation_cnt_sum[j] += reservation_cnt[friend_][j];
        }
    }
    for (int i{1}; i <= area_cnt; ++i) {
        reservation_cnt_sum[i] += reservation_cnt[user][i];
    }

    int index;
    int max_cnt{-1};
    for (int i{1}; i <= area_cnt; ++i) {
        while (heap[i].size && products[heap[i].container[0]].reserved) {
            heap[i].pop();
        }

        if (heap[i].size) {
            if (reservation_cnt_sum[i] > max_cnt ||
                reservation_cnt_sum[i] == max_cnt &&
                    CMP(heap[i].container[0], index)) {
                index = heap[i].container[0];
                max_cnt = reservation_cnt_sum[i];
            }
        }
    }

    return products[index].id;
}
