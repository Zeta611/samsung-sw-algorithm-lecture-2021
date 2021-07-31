constexpr int INF{32767};
constexpr int NAME_MAX{20};
constexpr int MEMBER_MAX{200};
constexpr unsigned HASH_MAX{32767};

int mstrcmp(const char *a, const char *b)
{
    int i;
    for (i = 0; a[i] != '\0'; ++i) {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
    }
    return a[i] - b[i];
}

void mstrcpy(char *dest, const char *src)
{
    int i{0};
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = src[i];
}

// Adapted from K&R.
unsigned hash(const char *a)
{
    unsigned val{0};
    while (*a) {
        val = *a++ + 31 * val;
    }
    return val;
}

enum Sex { MALE, FEMALE, MERGED };
enum Rel { SPOUSE, PARENT, CHILD };

struct Member {
    char name_m[NAME_MAX];
    char name_f[NAME_MAX];
    Sex sex;

    Member *parents_m{nullptr};
    Member *parents_f{nullptr};

    Member *children[MEMBER_MAX - 1]{};
    int children_cnt{0};

    int id;
    int d{INF};

    Member() {}
    Member(const char *name, Sex sex, int id) : sex{sex}, id{id}
    {
        mstrcpy(sex == MALE ? name_m : name_f, name);
    }
} pool[MEMBER_MAX];
int mcnt{0};
bool should_update{false};
int dist[MEMBER_MAX][MEMBER_MAX];
Member *name_table[HASH_MAX];
Sex sex_table[HASH_MAX];

Member *lookup(const char *name, Sex *sex = nullptr)
{
    unsigned hval{hash(name) % HASH_MAX};
    Member *m{nullptr};
    do {
        unsigned next{(hval + 1) % HASH_MAX};
        if (!name_table[next]) {
            m = name_table[hval];
            if (sex) {
                *sex = sex_table[hval];
            }
        } else if (mstrcmp(sex_table[hval] == MALE ? name_table[hval]->name_m
                                                   : name_table[hval]->name_f,
                           name) == 0) {
            m = name_table[hval];
            if (sex) {
                *sex = sex_table[hval];
            }
        } else {
            hval = next;
        }
    } while (!m);
    return m;
}

void put(Member *member, Sex sex)
{
    unsigned hval{hash(sex == MALE ? member->name_m : member->name_f) %
                  HASH_MAX};
    while (true) {
        if (!name_table[hval]) {
            name_table[hval] = member;
            sex_table[hval] = sex;
            break;
        }
        hval = (hval + 1) % HASH_MAX;
    }
}

Member *alloc(const char *name, Sex sex)
{
    Member *m{&pool[mcnt]};
    *m = Member(name, sex, mcnt++);
    put(m, sex);
    return m;
}

void alias(Member *member, const char *name)
{
    const Sex sex{member->sex == MALE ? FEMALE : MALE};
    member->sex = MERGED;
    mstrcpy(sex == MALE ? member->name_m : member->name_f, name);
    put(member, sex);
}

struct Node {
    Member *member;
    Node *next;
} nodes[MEMBER_MAX];
Node *head{nullptr};
Node *tail{nullptr};
int ncnt{0};
inline bool empty() { return !head; }
inline void init_queue()
{
    head = tail = nullptr;
    ncnt = 0;
}

Node *alloc(Member *member, Node *next = nullptr)
{
    Node *n{&nodes[ncnt++]};
    *n = {member, next};
    return n;
}

void enqueue(Member *member)
{
    if (!tail) {
        head = tail = alloc(member);
        return;
    }
    tail->next = alloc(member);
    tail = tail->next;
}

Member *dequeue()
{
    Member *m{head->member};
    head = head->next;
    if (!head) {
        tail = nullptr;
    }
    return m;
}

void visit(Member *member, int d)
{
    if (!member || member->d < INF) {
        return;
    }
    member->d = d + 1;
    enqueue(member);
}

void init(char *initialMemberName, int initialMemberSex)
{
    for (int i{0}; i < HASH_MAX; ++i) {
        name_table[i] = nullptr;
    }
    mcnt = 0;
    alloc(initialMemberName, static_cast<Sex>(initialMemberSex));
}

bool addMember(char *newMemberName, int newMemberSex, int relationship,
               char *existingMemberName)
{
    Sex sex;
    Member *member{lookup(existingMemberName, &sex)};
    const Sex new_sex{static_cast<Sex>(newMemberSex)};

    switch (relationship) {
    case SPOUSE:
        if (new_sex == member->sex || member->sex == MERGED) {
            return false;
        }
        alias(member, newMemberName);
        break;

    case PARENT: {
        Member *parents{sex == MALE ? member->parents_m : member->parents_f};
        if (parents) {
            if (parents->sex == MERGED || parents->sex == new_sex) {
                return false;
            }
            alias(parents, newMemberName);
        } else {
            parents = alloc(newMemberName, new_sex);
            parents->children[parents->children_cnt++] = member;
            (sex == MALE ? member->parents_m : member->parents_f) = parents;
            should_update = true;
        }
    } break;

    case CHILD: {
        Member *child{member->children[member->children_cnt++] =
                          alloc(newMemberName, new_sex)};
        (new_sex == MALE ? child->parents_m : child->parents_f) = member;
        should_update = true;
    } break;
    }
    return true;
}

void bfs(int id)
{
    Member *src{&pool[id]};
    init_queue();
    for (int i{0}; i < mcnt; ++i) {
        pool[i].d = INF;
    }
    src->d = 0;
    enqueue(src);

    int cnt{0};
    while (!empty()) {
        Member *m{dequeue()};
        dist[id][m->id] = m->d;
        visit(m->parents_m, m->d);
        visit(m->parents_f, m->d);
        for (int i{0}; i < m->children_cnt; ++i) {
            visit(m->children[i], m->d);
        }
    }
}

void update()
{
    for (int i{0}; i < mcnt; ++i) {
        bfs(i);
    }
}

int getDistance(char *nameA, char *nameB)
{
    if (should_update) {
        update();
        should_update = false;
    }
    return dist[lookup(nameA)->id][lookup(nameB)->id];
}

int countMember(char *name, int d)
{
    if (should_update) {
        update();
        should_update = false;
    }
    const int id{lookup(name)->id};
    int cnt{0};
    for (int i{0}; i < mcnt; ++i) {
        if (dist[id][i] == d) {
            cnt += pool[i].sex == MERGED ? 2 : 1;
        }
    }
    return d == 0 ? cnt - 1 : cnt;
}
