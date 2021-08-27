#define ATOI(c) ((c) - 'a')
constexpr int SHIFT{5};
using word_t = unsigned long long;
word_t hash(const char *word)
{
    word_t val{0};
    while (*word) {
        val <<= SHIFT;
        val += ATOI(*word++) + 1;
    }
    return val;
}

constexpr int MAX_USERS{20};
constexpr int MAX_GROUPS{10};
constexpr int MAX_NAME_LEN{8};
constexpr int MAX_PATH_LEN{410};
constexpr int MAX_DIR_NAME{8};
constexpr int MAX_EXT_NAME{8};
constexpr int MAX_DIR_DEPTH{50};
constexpr int MAX_DIRS{10000};
constexpr int MAX_FILES{10000};
constexpr int ALPH_CNT{'z' - 'a' + 1};
constexpr int TRIE_SIZE{(MAX_USERS + MAX_GROUPS) * MAX_NAME_LEN + 2};

struct Trie;
extern Trie tries[TRIE_SIZE];
extern int trie_cnt;
int trie_alloc();
struct Trie {
    int index;
    int children[ALPH_CNT];

    void init()
    {
        index = 0;
        for (int i{0}; i < ALPH_CNT; ++i) {
            children[i] = 0;
        }
    }

    int get(const char *str) const
    {
        long t{this - tries};
        while (*str) {
            if (!(t = tries[t].children[ATOI(*str++)])) {
                return 0;
            }
        }
        return tries[t].index;
    }

    void put(const char *str, int index)
    {
        long t{this - tries};
        while (*str) {
            const int c{ATOI(*str++)};
            if (!tries[t].children[c]) {
                tries[t].children[c] = trie_alloc();
            }
            t = tries[t].children[c];
        }
        tries[t].index = index;
    }
} tries[TRIE_SIZE];
Trie &user_trie = tries[0];
Trie &group_trie = tries[1];
int trie_cnt;
inline int trie_alloc()
{
    tries[trie_cnt].init();
    return trie_cnt++;
}

int group_table[MAX_USERS + 1];

struct Dir {
    word_t name;
    int user;
    int permission;
    int subdirs;
    int files;
} dirs[MAX_DIRS + 1];
int dir_cnt;
inline int dir_alloc(word_t name, int user, int permission)
{
    dirs[++dir_cnt] = {name, user, permission, 0, 0};
    return dir_cnt;
}

struct File {
    word_t name;
    word_t ext;
} files[MAX_FILES + 1];
int file_cnt;
inline int file_alloc(word_t name, word_t ext)
{
    files[++file_cnt] = {name, ext};
    return file_cnt;
}

struct Node {
    int index;
    int next;
} nodes[MAX_DIRS + MAX_FILES + 1];
int node_cnt;
inline int node_alloc(int index, int next)
{
    nodes[++node_cnt] = {index, next};
    return node_cnt;
}

int get_subdir(int dir, word_t subdir)
{
    for (int node{dirs[dir].subdirs}; node; node = nodes[node].next) {
        if (dirs[nodes[node].index].name == subdir) {
            return nodes[node].index;
        }
    }
    return 0;
}

inline int allowed(int dir, int user)
{
    return dirs[dir].permission == 2 || dirs[dir].user == user ||
           dirs[dir].permission == 1 &&
               group_table[dirs[dir].user] == group_table[user];
}

int user_cnt;
int group_cnt;
void init()
{
    trie_cnt = 0;
    trie_alloc(); // user_trie
    trie_alloc(); // group_trie
    static const char admin[] = "admin";
    user_trie.put(admin, user_cnt = 1);
    group_trie.put(admin, group_cnt = 1);
    group_table[1] = 1;
    for (int i{2}; i <= MAX_USERS; ++i) {
        group_table[i] = 0;
    }

    dir_cnt = file_cnt = node_cnt = 0;
    dir_alloc(0, 1, 2); // root directory
}

void createUser(char *user, char *group)
{
    user_trie.put(user, ++user_cnt);
    int g;
    if (!(g = group_trie.get(group))) {
        group_trie.put(group, g = ++group_cnt);
    }
    group_table[user_cnt] = g;
}

int createDirectory(char *user, char *path, char *directory, int permission)
{
    const int u{user_trie.get(user)};

    int dir{1};
    while (*++path) {
        const char *name{path};
        while (*path != '/') {
            ++path;
        }

        *path = '\0';
        dir = get_subdir(dir, hash(name));
        *path = '/';
        if (!allowed(dir, u)) {
            return 0;
        }
    }

    dirs[dir].subdirs = node_alloc(dir_alloc(hash(directory), u, permission),
                                   dirs[dir].subdirs);
    return 1;
}

int createFile(char *user, char *path, char *file, char *extension)
{
    const int u{user_trie.get(user)};

    int dir{1};
    while (*++path) {
        const char *name{path};
        while (*path != '/') {
            ++path;
        }

        *path = '\0';
        dir = get_subdir(dir, hash(name));
        *path = '/';
        if (!allowed(dir, u)) {
            return 0;
        }
    }

    dirs[dir].files =
        node_alloc(file_alloc(hash(file), hash(extension)), dirs[dir].files);
    return 1;
}

int find(int user, int dir, char *pattern)
{
    if (!allowed(dir, user)) {
        return 0;
    }

    const char *name{++pattern};
    while (*pattern != '/' && *pattern != '.') {
        ++pattern;
    }

    if (*pattern == '/') {
        if (*name != '*') {
            *pattern = '\0';
            dir = get_subdir(dir, hash(name));
            *pattern = '/';
            return find(user, dir, pattern);
        } else {
            int cnt{0};
            for (int node{dirs[dir].subdirs}; node; node = nodes[node].next) {
                cnt += find(user, nodes[node].index, pattern);
            }
            return cnt;
        }
    }

    const bool name_glob{*name == '*'};
    *pattern = '\0';
    const word_t name_hash{name_glob ? 0 : hash(name)};
    *pattern = '.';

    const char *ext{++pattern};
    while (*pattern) {
        ++pattern;
    }
    const bool ext_glob{*ext == '*'};
    const word_t ext_hash{ext_glob ? 0 : hash(ext)};

    if (name_glob && ext_glob) {
        int cnt{0};
        for (int node{dirs[dir].files}; node; node = nodes[node].next) {
            ++cnt;
        }
        return cnt;
    } else if (name_glob) {
        int cnt{0};
        for (int node{dirs[dir].files}; node; node = nodes[node].next) {
            if (files[nodes[node].index].ext == ext_hash) {
                ++cnt;
            }
        }
        return cnt;
    } else if (ext_glob) {
        int cnt{0};
        for (int node{dirs[dir].files}; node; node = nodes[node].next) {
            if (files[nodes[node].index].name == name_hash) {
                ++cnt;
            }
        }
        return cnt;
    } else {
        for (int node{dirs[dir].files}; node; node = nodes[node].next) {
            const File &file = files[nodes[node].index];
            if (file.name == name_hash && file.ext == ext_hash) {
                return 1;
            }
        }
        return 0;
    }
}

int find(char *user, char *pattern)
{
    return find(user_trie.get(user), 1, pattern);
}
