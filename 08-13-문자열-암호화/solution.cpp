#define ATOI(c) ((c) - 'a')
#define HASH(str, i)                                                           \
    ((ATOI((str)[i]) * ALPH_CNT * ALPH_CNT) +                                  \
     (ATOI((str)[(i) + 1]) * ALPH_CNT) + ATOI((str)[(i) + 2]))

constexpr int MAX_LEN{50000};
constexpr int SUBSTR_LEN{3};
constexpr int ALPH_CNT{'z' - 'a' + 1};

int len;
char str[MAX_LEN + 1];
int ind[ALPH_CNT * ALPH_CNT * ALPH_CNT];

struct Node {
    int i;
    int next;
} nodes[MAX_LEN + SUBSTR_LEN * 2];
int next_free;
int nalloc(int i, int next)
{
    const int idx{next_free};
    next_free = nodes[idx].next;

    nodes[idx].i = i;
    nodes[idx].next = next;
    return idx;
}

void insert(int hash, int i)
{
    int *node{&ind[hash]};
    while (*node && nodes[*node].i < i) {
        node = &nodes[*node].next;
    }
    *node = nalloc(i, *node);
}

void remove(int hash, int i)
{
    int *node{&ind[hash]};
    while (*node && nodes[*node].i != i) {
        node = &nodes[*node].next;
    }

    if (*node && nodes[*node].i == i) {
        const int n{*node};
        *node = nodes[n].next;

        nodes[n].next = next_free;
        next_free = n;
    }
}

void init(int n, char *original)
{
    len = n;
    for (int i{0}; i <= len; ++i) {
        str[i] = original[i];
    }

    for (int i{0}; i < ALPH_CNT * ALPH_CNT * ALPH_CNT; ++i) {
        ind[i] = 0;
    }

    next_free = 1;
    for (int i{1}; i < sizeof nodes / sizeof nodes[0]; ++i) {
        nodes[i].next = i + 1;
    }

    for (int i{len - SUBSTR_LEN}; i >= 0; --i) {
        const int hash{HASH(str, i)};
        ind[hash] = nalloc(i, ind[hash]);
    }
}

int change(char *src, char *dest)
{
    int cnt{0};
    for (int n{ind[HASH(src, 0)]}, i{nodes[n].i}; n;) {
        int idx;
        do {
            n = nodes[n].next;
            idx = nodes[n].i;
        } while (n && idx - i < SUBSTR_LEN);

        const int js{i >= 2 ? i - 2 : i >= 1 ? i - 1 : i};
        const int je{i + 2 * SUBSTR_LEN - 1 <= len
                         ? i + SUBSTR_LEN
                         : i + 2 * SUBSTR_LEN - 2 <= len ? i + SUBSTR_LEN - 1
                                                         : i + SUBSTR_LEN - 2};
        for (int j{js}; j < je; ++j) {
            remove(HASH(str, j), j);
        }
        str[i] = dest[0];
        str[i + 1] = dest[1];
        str[i + 2] = dest[2];
        for (int j{js}; j < je; ++j) {
            insert(HASH(str, j), j);
        }

        i = idx;
        ++cnt;
    }
    return cnt;
}

void result(char *result)
{
    for (int i{0}; i <= len; ++i) {
        result[i] = str[i];
    }
}
