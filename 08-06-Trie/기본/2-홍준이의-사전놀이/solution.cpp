constexpr int MAX_LEN{10};
constexpr int ALPH_CNT{'z' - 'a' + 1};
constexpr int MAX_ADD{100000};
constexpr int MAX_NODES{1 + MAX_ADD * MAX_LEN};

struct Trie;
Trie *alloc();
struct Trie {
    Trie *children[ALPH_CNT];
    int size;

    Trie *add(int len, const char *str)
    {
        ++size;
        if (!len || !*str) {
            return this;
        }
        const int flag{1 << (*str - 'a')};
        if (!children[*str - 'a']) {
            children[*str - 'a'] = alloc();
        }
        return children[*str - 'a']->add(len - 1, str + 1);
    }

    int get(int len, const char *str)
    {
        if (!len || !*str) {
            return size;
        }
        if (children[*str - 'a']) {
            return children[*str - 'a']->get(len - 1, str + 1);
        }
        return 0;
    }

    inline void init()
    {
        for (int i{0}; i < ALPH_CNT; ++i) {
            children[i] = nullptr;
        }
        size = 0;
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
void init(void)
{
    ncnt = 0;
    root = alloc();
}

void insert(int buffer_size, char *buf) { root->add(buffer_size, buf); }

int query(int buffer_size, char *buf) { return root->get(buffer_size, buf); }
