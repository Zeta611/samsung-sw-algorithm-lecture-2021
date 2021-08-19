constexpr int MAX_GROUPS{100};
constexpr int MAX_USERS{1000};
constexpr int MAX_LEN{15};

constexpr int MAX_EVENTS_PER_USER{100};
constexpr int MAX_UNIQUE_PER_GROUP{200};
constexpr int MAX_DUPL_PER_EVENT{5};

void mstrcpy(char *dst, const char *src)
{
    int c{0};
    while ((dst[c] = src[c]) != 0) {
        ++c;
    }
}

int mstrcmp(const char *str1, const char *str2)
{
    int c{0};
    while (str1[c] != 0 && str1[c] == str2[c]) {
        ++c;
    }
    return str1[c] - str2[c];
}

int group_event_cnt[MAX_GROUPS];
char group_events[MAX_GROUPS][MAX_UNIQUE_PER_GROUP][MAX_LEN];

int group_event_user_cnt[MAX_GROUPS][MAX_UNIQUE_PER_GROUP];
int group_event_users[MAX_GROUPS][MAX_UNIQUE_PER_GROUP][MAX_DUPL_PER_EVENT];

int user_event_cnt[MAX_USERS];
char user_events[MAX_USERS][MAX_EVENTS_PER_USER][MAX_LEN];
int user_event_group[MAX_USERS][MAX_EVENTS_PER_USER];

void init()
{
    for (int i{0}; i < MAX_GROUPS; ++i) {
        group_event_cnt[i] = 0;
        for (int j{0}; j < MAX_UNIQUE_PER_GROUP; ++j) {
            group_event_user_cnt[i][j] = 0;
        }
    }
    for (int i{0}; i < MAX_USERS; ++i) {
        user_event_cnt[i] = 0;
    }
}

void addEvent(int user, char *event, int group)
{
    int i{0};
    bool exists{false};
    for (; i < group_event_cnt[group]; ++i) {
        if (!mstrcmp(group_events[group][i], event)) {
            exists = true;
            break;
        }
    }

    if (!exists) {
        mstrcpy(group_events[group][group_event_cnt[group]++], event);

        group_event_users[group][i][0] = user;
        group_event_user_cnt[group][i] = 1;
    } else {
        group_event_users[group][i][group_event_user_cnt[group][i]++] = user;
    }

    mstrcpy(user_events[user][user_event_cnt[user]], event);
    user_event_group[user][user_event_cnt[user]++] = group;
}

int deleteEvent(int user, char *event)
{
    int i{0};
    for (; i < user_event_cnt[user]; ++i) {
        if (!mstrcmp(user_events[user][i], event)) {
            break;
        }
    }
    if (i == user_event_cnt[user]) {
        return 0;
    }

    const int group{user_event_group[user][i]};
    int j{0};
    for (; j < group_event_cnt[group]; ++j) {
        if (!mstrcmp(group_events[group][j], event)) {
            break;
        }
    }

    int k{0};
    for (; k < group_event_user_cnt[group][j]; ++k) {
        if (group_event_users[group][j][k] == user) {
            break;
        }
    }
    if (k == 0) {
        const int cnt{group_event_user_cnt[group][j]};

        for (int l{0}; l < group_event_user_cnt[group][j]; ++l) {
            const int u{group_event_users[group][j][l]};
            for (int m{0}; m < user_event_cnt[u]; ++m) {
                if (!mstrcmp(user_events[u][m], event)) {
                    mstrcpy(user_events[u][m],
                            user_events[u][user_event_cnt[u] - 1]);
                    user_event_group[u][m] =
                        user_event_group[u][--user_event_cnt[u]];
                    break;
                }
            }
        }

        group_event_user_cnt[group][j] =
            group_event_user_cnt[group][group_event_cnt[group] - 1];
        for (int l{0}; l < group_event_user_cnt[group][j]; ++l) {
            group_event_users[group][j][l] =
                group_event_users[group][group_event_cnt[group] - 1][l];
        }

        mstrcpy(group_events[group][j],
                group_events[group][--group_event_cnt[group]]);

        return cnt;
    } else {
        mstrcpy(user_events[user][i],
                user_events[user][user_event_cnt[user] - 1]);
        user_event_group[user][i] =
            user_event_group[user][--user_event_cnt[user]];

        group_event_users[group][j][k] =
            group_event_users[group][j][--group_event_user_cnt[group][j]];

        return 1;
    }
}

int changeEvent(int user, char *event, char *cname)
{
    int i{0};
    for (; i < user_event_cnt[user]; ++i) {
        if (!mstrcmp(user_events[user][i], event)) {
            break;
        }
    }
    if (i == user_event_cnt[user]) {
        return 0;
    }

    const int group{user_event_group[user][i]};
    int j{0};
    for (; j < group_event_cnt[group]; ++j) {
        if (!mstrcmp(group_events[group][j], event)) {
            break;
        }
    }

    int k{0};
    for (; k < group_event_user_cnt[group][j]; ++k) {
        if (group_event_users[group][j][k] == user) {
            break;
        }
    }
    if (k == 0) {
        const int cnt{group_event_user_cnt[group][j]};

        for (int l{0}; l < group_event_user_cnt[group][j]; ++l) {
            const int u{group_event_users[group][j][l]};
            for (int m{0}; m < user_event_cnt[u]; ++m) {
                if (!mstrcmp(user_events[u][m], event)) {
                    mstrcpy(user_events[u][m], cname);
                    break;
                }
            }
        }

        mstrcpy(group_events[group][j], cname);

        return cnt;
    } else {
        mstrcpy(user_events[user][i], cname);

        group_event_users[group][j][k] =
            group_event_users[group][j][--group_event_user_cnt[group][j]];

        const int e{group_event_cnt[group]};
        mstrcpy(group_events[group][e], cname);
        group_event_users[group][e][0] = user;
        group_event_user_cnt[group][e] = 1;
        ++group_event_cnt[group];

        return 1;
    }
}

int getCount(int user) { return user_event_cnt[user]; }
