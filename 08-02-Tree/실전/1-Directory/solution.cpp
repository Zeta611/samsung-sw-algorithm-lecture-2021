#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999

constexpr long max_dir{50000};

int mstrlen(const char *a)
{
    int len = 0;
    while (a[len])
        ++len;
    return len;
}

int mstrncmp(const char *a, const char *b, int len)
{
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i])
            return a[i] - b[i];
    }
    return 0;
}

void mstrcpy(char *dest, const char *src)
{
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = src[i];
}

struct Dir {
    char name[NAME_MAXLEN];
    int cnt = 0;
    Dir *parent;
    Dir *children = nullptr;
    Dir *next = nullptr;

    void add_child(Dir *child)
    {
        ++cnt;
        child->next = children;
        children = child;
    }
} pool[max_dir];
long dcnt;
Dir *root;
Dir buf[PATH_MAXLEN / 2];

Dir *alloc(const char *name, Dir *parent = nullptr)
{
    Dir *dir{&pool[dcnt++]};
    dir->cnt = 0;
    dir->children = dir->next = nullptr;
    dir->parent = parent;
    mstrcpy(dir->name, name);
    if (parent) {
        parent->add_child(dir);
    }
    return dir;
}

void init(int n = 0)
{
    dcnt = 0;
    root = alloc("");
}

Dir **last_dir(const char *path, Dir **parent = nullptr)
{
    Dir **dir{&root};
    const char *start{path + 1};
    while (*start) {
        if (parent) {
            *parent = *dir;
        }

        const char *p{start};
        while (*p != '/') {
            ++p;
        }
        for (Dir **d{&(*dir)->children}; *d; d = &(*d)->next) {
            if (mstrlen((*d)->name) == p - start &&
                mstrncmp((*d)->name, start, p - start) == 0) {
                dir = d;
            }
        }
        start = p + 1;
    }
    return dir;
}

void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1])
{
    alloc(name, *last_dir(path));
}

Dir *detach(char *path)
{
    Dir *parent;
    Dir **target{last_dir(path, &parent)};
    Dir *ret;
    if (*target == root) {
        init();
    } else {
        --parent->cnt;
        ret = *target;
        *target = (*target)->next;
    }
    return ret;
}

void cmd_rm(char path[PATH_MAXLEN + 1]) { detach(path); }

void cmd_cp(Dir *src, Dir *dst)
{
    Dir *cpy{alloc(src->name, dst)};
    for (Dir *child{src->children}; child; child = child->next) {
        cmd_cp(child, cpy);
    }
}

void cmd_cp(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1])
{
    cmd_cp(*last_dir(srcPath), *last_dir(dstPath));
}

void cmd_mv(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1])
{
    Dir *detached{detach(srcPath)};
    (*last_dir(dstPath))->add_child(detached);
}

int cmd_find(Dir *dir)
{
    int cnt{dir->cnt};
    for (Dir *child{dir->children}; child; child = child->next) {
        cnt += cmd_find(child);
    }
    return cnt;
}

int cmd_find(char path[PATH_MAXLEN + 1]) { return cmd_find(*last_dir(path)); }
