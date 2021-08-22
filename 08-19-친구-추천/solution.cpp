constexpr int MAX_USERS{10000};
constexpr int MAX_FRIENDS{50000 * 10};
constexpr int MAX_RECOMMEND{5};

struct Node {
    int id;
    int next;
} pool[2 * MAX_FRIENDS + 1];
int node_cnt;
int node_alloc(int id, int next)
{
    pool[++node_cnt].id = id;
    pool[node_cnt].next = next;
    return node_cnt;
}

int friend_cnt;
int friends[MAX_USERS + 1];

void init(int n)
{
    node_cnt = 0;
    friend_cnt = n;
    for (int i{1}; i <= friend_cnt; ++i) {
        friends[i] = 0;
    }
}

void add(int user, int cnt, int *users)
{
    while (cnt--) {
        friends[*users] = node_alloc(user, friends[*users]);
        friends[user] = node_alloc(*users++, friends[user]);
    }
}

void del(int user, int other)
{
    int *node{&friends[user]};
    while (*node && pool[*node].id != other) {
        node = &pool[*node].next;
    }
    *node = pool[*node].next;

    node = &friends[other];
    while (*node && pool[*node].id != user) {
        node = &pool[*node].next;
    }
    *node = pool[*node].next;
}

int common_friend_cnt[MAX_FRIENDS + 1];
int recommend(int user, int *list)
{
    for (int i{1}; i <= friend_cnt; ++i) {
        common_friend_cnt[i] = 0;
    }

    for (int node{friends[user]}; node; node = pool[node].next) {
        for (int n{friends[pool[node].id]}; n; n = pool[n].next) {
            ++common_friend_cnt[pool[n].id];
        }
    }
    for (int node{friends[user]}; node; node = pool[node].next) {
        common_friend_cnt[pool[node].id] = 0;
    }
    common_friend_cnt[user] = 0;

    int cnt{0};
    for (int i{1}; i <= friend_cnt; ++i) {
        if (!common_friend_cnt[i]) {
            continue;
        }

        if (!cnt) {
            list[cnt++] = i;
            continue;
        }

        int j{cnt - 1};
        for (; j >= 0 && (common_friend_cnt[list[j]] < common_friend_cnt[i] ||
                          common_friend_cnt[list[j]] == common_friend_cnt[i] &&
                              list[j] > i);
             --j) {
            if (j + 1 < MAX_RECOMMEND) {
                list[j + 1] = list[j];
            }
        }
        if (j + 1 < MAX_RECOMMEND) {
            list[j + 1] = i;
            if (cnt < MAX_RECOMMEND) {
                ++cnt;
            }
        }
    }
    return cnt;
}
