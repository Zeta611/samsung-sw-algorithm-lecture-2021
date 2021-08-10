constexpr int MAX_LEN{6};
constexpr int ALPH_CNT{'z' - 'a' + 1};
constexpr int MAX_ADD{50000};
constexpr int MAX_NODES{1 + MAX_ADD * MAX_LEN};

struct Trie;
Trie *tstack[MAX_NODES];
const char *sstack[MAX_NODES];
int stk_end{0};
void push(Trie *t, const char *s)
{
    tstack[stk_end] = t;
    sstack[stk_end++] = s;
}
void pop(Trie **t, const char **s)
{
    *t = tstack[--stk_end];
    *s = sstack[stk_end];
}

Trie *talloc();
struct Trie {
    Trie *children[ALPH_CNT];
    char alph[ALPH_CNT];
    char children_cnt;

    int bitmap;
    int size;
    int terminal;

    void add(const char *str)
    {
        ++size;
        if (!*str) {
            ++terminal;
            return;
        }

        const int flag{1 << (*str - 'a')};
        if (!(bitmap & flag)) {
            bitmap |= flag;
            alph[*str - 'a'] = children_cnt;
            children[children_cnt++] = talloc();
        }
        children[alph[*str - 'a']]->add(str + 1);
    }

    int get(const char *str)
    {
        int cnt{0};

        push(this, str);
        while (stk_end) {
            Trie *trie;
            const char *str;
            pop(&trie, &str);

            if (!*str) {
                cnt += trie->terminal;
                continue;
            }

            if (*str == '*') {
                if (!*(str + 1)) {
                    cnt += trie->size;
                    continue;
                }
                for (int i{0}; i < trie->children_cnt; ++i) {
                    push(trie->children[i], str);
                }
                ++str;
            }

            if (trie->bitmap & (1 << (*str - 'a'))) {
                push(trie->children[trie->alph[*str - 'a']], str + 1);
            }
        }
        return cnt;
        // if (!*str) {
        //     return terminal;
        // }
        //
        // int cnt{0};
        // if (*str == '*') {
        //     if (!*(str + 1)) {
        //         return size;
        //     }
        //
        //     for (int i{0}; i < children_cnt; ++i) {
        //         cnt += children[i]->get(str);
        //     }
        //     ++str;
        // }
        //
        // if (bitmap & (1 << (*str - 'a'))) {
        //     cnt += children[alph[*str - 'a']]->get(str + 1);
        // }
        // return cnt;
    }

    int remove(const char *str)
    {
        int cnt{0};
        if (!*str) {
            cnt = terminal;
            size -= terminal;
            terminal = 0;
            return cnt;
        }

        if (*str == '*') {
            if (!*(str + 1)) {
                cnt = size;
                children_cnt = size = terminal = 0;
                return cnt;
            }

            for (int i{0}; i < children_cnt; ++i) {
                cnt += children[i]->remove(str);
            }
            ++str;
        }

        if (bitmap & (1 << (*str - 'a'))) {
            cnt += children[alph[*str - 'a']]->remove(str + 1);
        }

        size -= cnt;
        return cnt;
    }

    inline void init() { bitmap = size = terminal = children_cnt = 0; }
} tnodes[MAX_NODES];
int ncnt{0};
Trie *root;
Trie *talloc()
{
    Trie *node{&tnodes[ncnt++]};
    node->init();
    return node;
}

void init(void)
{
    ncnt = 0;
    root = talloc();
}

void addWord(char *str) { root->add(str); }

int removeWord(char *str) { return root->remove(str); }

int searchWord(char *str) { return root->get(str); }
