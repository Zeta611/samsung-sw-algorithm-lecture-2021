#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>

constexpr int ALPH_CNT{'z' - 'a' + 1};
constexpr int MAX_LEN{30};
constexpr int MAX_SIZE{30000};
constexpr long MAX_NODES{1 + MAX_SIZE * MAX_LEN};

char dict[MAX_SIZE][MAX_LEN + 1];
char query[MAX_SIZE][MAX_LEN + 1];
int order[MAX_SIZE];
int idx[MAX_SIZE];

struct Trie;
Trie *alloc();
struct Trie {
    Trie *children[ALPH_CNT];
    long bitmap;
    long size;
    int order;
    char ch;

    void add(const char *str, int order)
    {
        ++size;
        if (!*str) {
            this->order = order;
            return;
        }
        const long flag{1 << (*str - 'a')};
        if (!(bitmap & flag)) {
            children[*str - 'a'] = alloc();
            bitmap |= flag;
        }
        children[*str - 'a']->ch = *str;
        children[*str - 'a']->add(str + 1, order);
    }

    int get(const char *str)
    {
        if (!*str) {
            return order;
        }
        const long flag{1 << (*str - 'a')};
        if (bitmap & flag) {
            return children[*str - 'a']->get(str + 1);
        }
        return std::numeric_limits<int>::max();
    }

    long cmp(const char *str)
    {
        if (!*str || !(bitmap & 1 << (*str - 'a'))) {
            return size;
        }
        return children[*str - 'a']->cmp(str + 1) + size;
    }

    inline void init()
    {
        size = bitmap = 0;
        order = std::numeric_limits<int>::max();
    }
} nodes[MAX_NODES];
int ncnt{0};
Trie *root;
Trie *alloc()
{
    Trie *node{&nodes[ncnt++]};
    node->init();
    return node;
}
inline void init()
{
    ncnt = 0;
    root = alloc();
}

long query_cnt(int n, int m)
{
    init();
    for (int i{0}; i < n; ++i) {
        root->add(dict[i], i);
    }

    for (int i{0}; i < m; ++i) {
        idx[i] = i;
        order[i] = root->get(query[i]);
    }
    std::sort(idx, idx + m, [](int a, int b) { return order[a] < order[b]; });

    int bound;
    for (bound = m - 1; bound >= 0; --bound) {
        if (order[idx[bound]] < std::numeric_limits<int>::max()) {
            break;
        }
    }

    init();
    long cnt{0};
    int j{0};
    for (int i{0}; i < m; ++i) {
        if (i <= bound || i > bound && j < n) {
            while (j <= (i <= bound ? order[idx[i]] : n - 1)) {
                root->add(dict[j], j);
                ++j;
            }
        }
        cnt += root->cmp(query[idx[i]]);
    }
    return cnt;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int n, m;
        std::cin >> n;
        for (int i{0}; i < n; ++i) {
            std::cin >> dict[i];
        }
        std::cin >> m;
        for (int i{0}; i < m; ++i) {
            std::cin >> query[i];
        }

        std::cout << '#' << test_case << ' ' << query_cnt(n, m) << '\n';
    }
    return 0;
}
