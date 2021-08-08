#include <iostream>
#include <stack>
#include <string>

constexpr int ALPH_CNT{'z' - 'a' + 1};
constexpr long MAX_LEN{400};
constexpr long MAX_NODES{1 << 20};
constexpr char NONE_STR[]{"none"};

std::string word;

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

const char *substr(long k)
{
    init();

    const int len{static_cast<int>(word.size())};
    for (int i{0}; i < len; ++i) {
        for (int j{i}; j < len; ++j) {
            root->add(word.substr(i, j - i + 1).c_str());
        }
    }

    stack.push(root);
    while (!stack.empty()) {
        const Trie *node{stack.top()};
        if (node->terminal && !--k) {
            return to_str(node);
        }
        stack.pop();

        static std::stack<const Trie *> s;
        for (long b{node->bitmap}, i{0}; b; b >>= 1, ++i) {
            if (b & 1) {
                s.push(node->children[i]);
            }
        }
        while (!s.empty()) {
            stack.push(s.top());
            s.pop();
        }
    }
    return NONE_STR;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    word.reserve(MAX_LEN);
    std::cout << sizeof nodes << '\n';

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        long k;
        std::cin >> k >> word;
        std::cout << '#' << test_case << ' ' << substr(k) << '\n';
    }
    return 0;
}
