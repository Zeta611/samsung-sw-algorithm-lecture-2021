#define CMP(i, j)                                                              \
    (posts[i].likes != posts[j].likes ? posts[i].likes > posts[j].likes        \
                                      : posts[i].time > posts[j].time)
constexpr int MAX_USERS{1000};
constexpr int MAX_POSTS{100000};
constexpr int MAX_TOP{10};

int user_cnt{0};
bool follow_table[MAX_USERS + 1][MAX_USERS + 1];
int follow_cnt[MAX_USERS + 1];
int following[MAX_USERS + 1][MAX_USERS];

struct Post {
    int user;
    int time;
    int likes;
} posts[MAX_POSTS + 1];
int post_cnt{0};
inline int post_alloc(int user, int time)
{
    posts[++post_cnt].user = user;
    posts[post_cnt].time = time;
    posts[post_cnt].likes = 0;
    return post_cnt;
}

struct PostNode {
    int post;
    int next;
} nodes[MAX_POSTS + 1];
int node_cnt{0};
inline int node_alloc(int post, int next)
{
    nodes[++node_cnt].post = post;
    nodes[node_cnt].next = next;
    return node_cnt;
}
int recents[MAX_USERS + 1];

int set_size{0};
int sorted_set[MAX_TOP];

void init(int n)
{
    user_cnt = n;
    post_cnt = node_cnt = 0;
    for (int i{1}; i <= user_cnt; ++i) {
        for (int j{1}; j <= user_cnt; ++j) {
            follow_table[i][j] = i == j;
        }
        follow_cnt[i] = 1;
        following[i][0] = i;
        recents[i] = 0;
    }
}

void follow(int user, int other, int now)
{
    follow_table[user][other] = true;
    following[user][follow_cnt[user]++] = other;
}

void makePost(int user, int post, int now)
{
    post_alloc(user, now);
    recents[user] = node_alloc(post, recents[user]);
}

void like(int post, int now) { ++posts[post].likes; }

void getFeed(int user, int now, int *dest)
{
    set_size = 0;

    int oldest{post_cnt + 1};
    for (int i{0}; i < follow_cnt[user]; ++i) {
        for (int node{recents[following[user][i]]};
             node && now - posts[nodes[node].post].time <= 1000;
             node = nodes[node].next) {
            const int post{nodes[node].post};
            oldest = oldest > post ? post : oldest;

            int j;
            for (j = 0; j < set_size; ++j) {
                if (CMP(post, sorted_set[j])) {
                    break;
                }
            }
            if (j == MAX_TOP) {
                continue;
            }
            if (set_size < MAX_TOP) {
                ++set_size;
            }
            for (int k{set_size - 1}; k > j; --k) {
                sorted_set[k] = sorted_set[k - 1];
            }
            sorted_set[j] = post;
        }
    }

    int cnt;
    for (cnt = 0; cnt < set_size; ++cnt) {
        dest[cnt] = sorted_set[cnt];
    }

    for (int post{oldest - 1}; cnt < MAX_TOP && post; --post) {
        if (follow_table[user][posts[post].user]) {
            dest[cnt++] = post;
        }
    }
}
