#include <cstring>
#include <iostream>
#include <stack>

constexpr int MAX_LEN{400};
constexpr int ALPH_CNT{'z' - 'a' + 1};
constexpr long MAX_NODES{1 + MAX_LEN * (MAX_LEN + 1) / 2};
constexpr char NONE_STR[]{"none"};

char word[MAX_LEN + 1];

struct Trie;
Trie *alloc(const Trie *);
struct Trie {
    long bitmap;
    Trie *children[ALPH_CNT];
    const Trie *parent;

    bool terminal;
    char ch;
    int depth;

    void add(const char *str)
    {
        if (!*str) {
            terminal = true;
            return;
        }
        const long flag{1 << (*str - 'a')};
        if (!(bitmap & flag)) {
            children[*str - 'a'] = alloc(this);
            bitmap |= flag;
        }
        children[*str - 'a']->ch = *str;
        children[*str - 'a']->add(str + 1);
    }

    Trie *get(const char *str)
    {
        if (!*str) {
            return terminal ? this : nullptr;
        }
        const long flag{1 << (*str - 'a')};
        if (bitmap & flag) {
            return children[*str - 'a']->get(str + 1);
        }
        return nullptr;
    }

    inline void init(const Trie *parent)
    {
        bitmap = 0;
        terminal = false;

        this->parent = parent;
        depth = parent ? parent->depth + 1 : 0;
    }
} nodes[MAX_NODES];
int ncnt{0};
Trie *root;
std::stack<const Trie *> stack;
Trie *alloc(const Trie *parent = nullptr)
{
    Trie *node{&nodes[ncnt++]};
    node->init(parent);
    return node;
}
inline void init()
{
    ncnt = 0;
    root = alloc();
    stack = {};
}

char buffer[MAX_LEN + 1];
const char *to_str(const Trie *node)
{
    buffer[node->depth] = '\0';
    for (int i{node->depth - 1}; i >= 0; --i, node = node->parent) {
        buffer[i] = node->ch;
    }
    return buffer;
}

const char *postfix(int k)
{
    init();
    for (const char *str{word}; *str; ++str) {
        root->add(str);
    }

    stack.push(root);
    while (!stack.empty()) {
        const Trie *node{stack.top()};
        if (node->terminal && !--k) {
            return to_str(node);
        }
        stack.pop();

        for (long b{node->bitmap}, i{0}; b; b >>= 1, ++i) {
            if (b & 1) {
                stack.push(node->children[i]);
            }
        }
    }
    return NONE_STR;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int k;
        std::cin >> k >> word;
        // Nodes are pushed to stack in a reveresed order
        std::cout << '#' << test_case << ' '
                  << postfix(std::strlen(word) - k + 1) << '\n';
    }
    return 0;
}
