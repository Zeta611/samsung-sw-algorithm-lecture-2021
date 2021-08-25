#define ATOI(c) ((c) < 'a' ? (c) - 'A' : (c) - 'a' + ALPH_CNT)

constexpr int ALPH_CNT{'z' - 'a' + 1};
constexpr int MAX_NAME_LEN{6};
constexpr int MAX_TYPE_LEN{3};
constexpr int MAX_TYPES{500};
constexpr int MAX_TYPES_PER_BOOK{5};
constexpr int MAX_SECTION{100};
constexpr int MAX_BOOKS{50000};

constexpr int TRIE_NODES_SIZE{MAX_BOOKS * (MAX_NAME_LEN + 1) +
                              MAX_TYPES * (MAX_TYPE_LEN + 1) + 2};
struct Trie;
extern Trie trie_nodes[TRIE_NODES_SIZE];
int trie_alloc();
struct Trie {
    int children[ALPH_CNT * 2];
    int index;

    void init()
    {
        index = 0;
        for (register int i{0}; i < ALPH_CNT * 2; ++i) {
            children[i] = 0;
        }
    }

    int get(const char *str) const
    {
        // Use only on the root trie node
        long trie{this - trie_nodes};
        while (*str) {
            if ((trie = trie_nodes[trie].children[ATOI(*str)])) {
                ++str;
            } else {
                return 0;
            }
        }
        return trie_nodes[trie].index;
    }

    void put(const char *str, int index)
    {
        // Use only on the root trie node
        long trie{this - trie_nodes};
        while (*str) {
            const int c{ATOI(*str)};
            int i;
            if (!(i = trie_nodes[trie].children[c])) {
                i = trie_nodes[trie].children[c] = trie_alloc();
            }
            trie = i;
            ++str;
        }
        trie_nodes[trie].index = index;
    }
} trie_nodes[TRIE_NODES_SIZE];
int trie_cnt;
inline int trie_alloc()
{
    trie_nodes[trie_cnt].init();
    return trie_cnt++;
}
Trie &book_trie = trie_nodes[0];
Trie &type_trie = trie_nodes[1];

struct Book {
    int cnt;
    int section;
    int types[MAX_TYPES_PER_BOOK];
    int nodes[MAX_TYPES_PER_BOOK];
} books[MAX_BOOKS + 1];
int book_cnt;
int type_cnt;

struct Node {
    int book;
    int prev;
    int next;
} nodes[MAX_BOOKS * MAX_TYPES_PER_BOOK + MAX_TYPES + 1];
int node_cnt;
inline int node_alloc()
{
    // Initialize a header for a circular doubly-linked list.
    ++node_cnt;
    return nodes[node_cnt].prev = nodes[node_cnt].next = node_cnt;
}
inline int node_alloc(int book, int prev, int next)
{
    nodes[++node_cnt].book = book;
    nodes[node_cnt].prev = prev;
    nodes[node_cnt].next = next;
    return node_cnt;
}
int library[MAX_TYPES + 1];

int section_cnt;
void init(int n)
{
    section_cnt = n;
    book_cnt = type_cnt = trie_cnt = node_cnt = 0;
    trie_alloc(); // book_trie initialization
    trie_alloc(); // type_trie initialization

    for (register int i{1}; i <= MAX_TYPES; ++i) {
        library[i] = node_alloc(); // list headers
    }
}

void add(char *name, int tcnt, char (*types)[MAX_TYPE_LEN + 1], int section)
{
    Book &b = books[++book_cnt];
    b.cnt = tcnt;
    b.section = section;
    book_trie.put(name, book_cnt);

    for (register int i{0}; i < tcnt; ++i) {
        int t;
        if ((t = type_trie.get(types[i]))) {
            b.types[i] = t;
        } else {
            type_trie.put(types[i], ++type_cnt);
            b.types[i] = type_cnt;
        }
    }

    for (register int i{0}; i < tcnt; ++i) {
        const int hd{library[b.types[i]]};
        const int nd{node_alloc(book_cnt, nodes[hd].prev, hd)};
        b.nodes[i] = nodes[nodes[nd].prev].next = nodes[hd].prev = nd;
    }
}

int moveType(char *type, int old_section, int new_section)
{
    int cnt{0};
    const int hd{library[type_trie.get(type)]};
    for (register int nd{nodes[hd].next}; nd != hd; nd = nodes[nd].next) {
        Book &b = books[nodes[nd].book];
        if (b.section == old_section) {
            ++cnt;
            b.section = new_section;
        }
    }
    return cnt;
}

void moveName(char *name, int section)
{
    books[book_trie.get(name)].section = section;
}

void deleteName(char *name)
{
    const Book &b = books[book_trie.get(name)];
    for (register int i{0}; i < b.cnt; ++i) {
        const int nd{b.nodes[i]};
        nodes[nodes[nd].prev].next = nodes[nd].next;
        nodes[nodes[nd].next].prev = nodes[nd].prev;
    }
}

bool book_found[MAX_BOOKS + 1];
int countBook(int tcnt, char (*types)[MAX_TYPE_LEN + 1], int section)
{
    for (register int i{1}; i <= book_cnt; ++i) {
        book_found[i] = false;
    }

    int cnt{0};
    for (register int i{0}; i < tcnt; ++i) {
        const int hd{library[type_trie.get(types[i])]};
        for (register int nd{nodes[hd].next}; nd != hd; nd = nodes[nd].next) {
            const int book{nodes[nd].book};
            if (books[book].section == section && !book_found[book]) {
                ++cnt;
                book_found[book] = true;
            }
        }
    }
    return cnt;
}
