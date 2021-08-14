int mstrcpy(char *dst, const char *src)
{
    int i{0};
    while ((dst[i] = src[i])) {
        ++i;
    }
    return i;
}

constexpr int ALPH_CNT{'z' - 'a' + 1};
constexpr int MAX_WORD{25000};
constexpr int MAX_LEN{20};
constexpr int MAX_TRIE{MAX_WORD * MAX_LEN + 1};

int global_time;
struct Trie;
extern Trie trie_nodes[MAX_TRIE];
int trie_alloc(int parent, char ch, char depth);
struct Trie {
    int parent;
    int time;
    int count; // -1 for banned
    int best_child;
    int children[ALPH_CNT];
    char ch;
    char depth;

    void init(int parent, char ch, char depth)
    {
        this->parent = parent;
        this->time = 0;
        this->count = 0;
        this->best_child = 0;
        this->ch = ch;
        this->depth = depth;
        for (int i{0}; i < ALPH_CNT; ++i) {
            children[i] = 0;
        }
    }

    void update_best_child()
    {
        int cand{count > 0 ? static_cast<int>(this - trie_nodes) : 0};
        int cand_count{count};
        int cand_time{time};
        for (int i{0}; i < ALPH_CNT; ++i) {
            if (!children[i]) {
                continue;
            }
            const int t{trie_nodes[children[i]].best_child};
            if (!t) {
                continue;
            }
            const Trie &ref = trie_nodes[t];
            if (cand_count < ref.count ||
                cand_count == ref.count && cand_time < ref.time) {
                cand = t;
                cand_count = ref.count;
                cand_time = ref.time;
            }
        }
        best_child = cand;

        if (parent >= 0) {
            trie_nodes[parent].update_best_child();
        }
    }

    void put(const char *word, bool ban = false)
    {
        if (!*word) {
            if (count >= 0) {
                time = ++global_time;
                count = ban ? -1 : count + 1;
            }
            update_best_child();
            return;
        }
        if (!children[*word - 'a']) {
            children[*word - 'a'] =
                trie_alloc(this - trie_nodes, *word, depth + 1);
        }
        trie_nodes[children[*word - 'a']].put(word + 1, ban);
    }

    int get(const char *prefix) const
    {
        if (!*prefix) {
            return best_child;
        }
        if (!children[*prefix - 'a']) {
            return 0;
        }
        return trie_nodes[children[*prefix - 'a']].get(prefix + 1);
    }
} trie_nodes[MAX_TRIE];
Trie &root = trie_nodes[0];
int trie_node_cnt;
inline int trie_alloc(int parent, char ch, char depth)
{
    trie_nodes[trie_node_cnt].init(parent, ch, depth);
    return trie_node_cnt++;
}

void init()
{
    global_time = 0;
    trie_node_cnt = 1;
    root.init(-1, 0, 0);
}

void inputWord(char *word) { root.put(word, false); }

int recommend(char *prefix, char *completed)
{
    int trie{root.get(prefix)};
    if (!trie) {
        return mstrcpy(completed, prefix);
    }

    const int len{trie_nodes[trie].depth};
    while (trie_nodes[trie].parent >= 0) {
        completed[trie_nodes[trie].depth - 1] = trie_nodes[trie].ch;
        trie = trie_nodes[trie].parent;
    }
    completed[len] = '\0';
    return len;
}

void banWord(char *word) { root.put(word, true); }
