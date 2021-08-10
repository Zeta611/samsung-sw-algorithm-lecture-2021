constexpr int MAX_LEN{6};
constexpr int ALPH_CNT{'z' - 'a' + 1};
constexpr int MAX_ADD{50000};
constexpr int MAX_DUP{47};
constexpr int MAX_NODES{1 + MAX_ADD * MAX_LEN};
constexpr int MAX_GLOB_NODES{1 + MAX_ADD * MAX_DUP / 3};
constexpr int MAX_LINKS{1 + MAX_LEN * (MAX_LEN + 1) / 2};

int mstrlen(const char *str)
{
    int i;
    for (i = 0; str[i]; ++i)
        ;
    return i;
}

int tstack[MAX_GLOB_NODES];
const char *sstack[MAX_GLOB_NODES];
int stk_end{0};
void push(int t, const char *s)
{
    tstack[stk_end] = t;
    sstack[stk_end++] = s;
}
void pop(int *t, const char **s)
{
    *t = tstack[--stk_end];
    *s = sstack[stk_end];
}

struct GlobTrie;
extern GlobTrie gnodes[MAX_GLOB_NODES];
int galloc();

struct GlobTrie {
    int children[ALPH_CNT + 1];
    int bitmap;
    int terminal;
    char alph[ALPH_CNT + 1];
    char children_cnt;

    int add(const char *str)
    {
        if (!*str) {
            ++terminal;
            return this - gnodes;
        }

        const int offset{*str == '*' ? ALPH_CNT : *str - 'a'};
        const int flag{1 << offset};
        if (!(bitmap & flag)) {
            bitmap |= flag;
            alph[offset] = children_cnt;
            children[children_cnt++] = galloc();
        }
        return gnodes[children[alph[offset]]].add(str + 1);
    }

    int get(const char *str)
    {
        int cnt{0};

        push(this - gnodes, str);
        while (stk_end) {
            int trie;
            const char *str;
            pop(&trie, &str);

            if (!*str) {
                cnt += gnodes[trie].terminal;
                continue;
            }

            const int offset{*str == '*' ? ALPH_CNT : *str - 'a'};
            if (gnodes[trie].bitmap & (1 << offset)) {
                push(gnodes[trie].children[gnodes[trie].alph[offset]], str + 1);
            }
        }
        return cnt;
    }

    inline void init() { bitmap = terminal = children_cnt = 0; }
} gnodes[MAX_GLOB_NODES];

int gcnt{0};
int galloc()
{
    gnodes[gcnt].init();
    return gcnt++;
}

char buf[MAX_LEN + 1];
struct Trie;
extern Trie tnodes[MAX_NODES];
int talloc();

struct Trie {
    int children[ALPH_CNT];
    int glob_links[MAX_LINKS];
    int bitmap;
    char alph[ALPH_CNT];
    char children_cnt;
    char links_cnt;
    bool terminal;

    void add(const char *str, const char *orig)
    {
        if (!*str) {
            terminal = true;
            links_cnt = 0;
            glob_links[links_cnt++] = gnodes[0].add(orig);

            const int len{mstrlen(orig)};
            for (int k{len == 6 ? 1 : 0}; k <= len; ++k) {
                for (int i{0}; i <= len - k; ++i) {
                    for (int j{0}; j < i; ++j) {
                        buf[j] = orig[j];
                    }
                    buf[i] = '*';
                    for (int j{i + k}; j < len; ++j) {
                        buf[j - k + 1] = orig[j];
                    }
                    buf[len - k + 1] = '\0';

                    glob_links[links_cnt++] = gnodes[0].add(buf);
                }
            }
            return;
        }

        const int offset{*str == '*' ? ALPH_CNT : *str - 'a'};
        const int flag{1 << offset};
        if (!(bitmap & flag)) {
            bitmap |= flag;
            alph[offset] = children_cnt;
            children[children_cnt++] = talloc();
        }
        tnodes[children[alph[offset]]].add(str + 1, orig);
    }

    int remove(const char *str)
    {
        int cnt{0};

        push(this - tnodes, str);
        while (stk_end) {
            int trie;
            const char *str;
            pop(&trie, &str);

            if (*str == '*') {
                for (int i{0}; i < tnodes[trie].children_cnt; ++i) {
                    push(tnodes[trie].children[i], str);
                }
                ++str;
            }

            if (!*str) {
                if (tnodes[trie].terminal) {
                    const int c{gnodes[tnodes[trie].glob_links[0]].terminal};
                    cnt += c;
                    gnodes[tnodes[trie].glob_links[0]].terminal = 0;
                    for (char i{1}; i < tnodes[trie].links_cnt; ++i) {
                        gnodes[tnodes[trie].glob_links[i]].terminal -= c;
                    }
                    tnodes[trie].terminal = false;
                }
                continue;
            }

            const int offset{*str == '*' ? ALPH_CNT : *str - 'a'};
            if (tnodes[trie].bitmap & (1 << offset)) {
                push(tnodes[trie].children[tnodes[trie].alph[offset]], str + 1);
            }
        }
        return cnt;
    }

    inline void init() { bitmap = terminal = children_cnt = links_cnt = 0; }
} tnodes[MAX_NODES];

int tcnt{0};
int talloc()
{
    tnodes[tcnt].init();
    return tcnt++;
}

void init(void)
{
    gnodes[0].init();
    tnodes[0].init();
    gcnt = tcnt = 1;
}

void addWord(char *str) { tnodes[0].add(str, str); }

int removeWord(char *str) { return tnodes[0].remove(str); }

int searchWord(char *str) { return gnodes[0].get(str); }
