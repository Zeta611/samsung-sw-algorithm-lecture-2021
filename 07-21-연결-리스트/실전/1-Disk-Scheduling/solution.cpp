constexpr int MAX{100000}; // Should be long IMO...
constexpr int SHIFT{4};
constexpr int BUCKET_CNT{(MAX >> SHIFT) + 1}; // Bucket size = 2**SHIFT

int head_pos;
bool look_left;

struct Node {
    int val;
    Node *prev;
    Node *next;
} npool[MAX];
int ncnt;
void node_init() { ncnt = 0; }
Node *nalloc(int val, Node *prev = nullptr, Node *next = nullptr)
{
    Node *node{&npool[ncnt++]};
    *node = {val, prev, next};
    return node;
}

struct NNode {
    const Node *node;
    NNode *prev;
    NNode *next;
} nnpool[MAX];
int nncnt;
NNode *bucket[BUCKET_CNT];
int bucket_cnt[BUCKET_CNT];
void bucket_init()
{
    nncnt = 0;
    for (int i{0}; i < BUCKET_CNT; ++i) {
        bucket[i] = nullptr;
        bucket_cnt[i] = 0;
    }
}
NNode *nnalloc(const Node *node, NNode *prev = nullptr, NNode *next = nullptr)
{
    NNode *nnode{&nnpool[nncnt++]};
    *nnode = {node, prev, next};
    return nnode;
}
void bucket_remove(NNode **nnode)
{
    if ((*nnode)->next) {
        (*nnode)->next->prev = (*nnode)->prev;
    }
    if ((*nnode)->prev) {
        (*nnode)->prev->next = (*nnode)->next;
    } else {
        *nnode = (*nnode)->next;
    }
}

struct Queue {
    Node *enqueue(int n)
    {
        if (tail) {
            tail->next = nalloc(n, tail);
            tail = tail->next;
        } else {
            head = tail = nalloc(n);
        }
        return tail;
    }

    int dequeue()
    {
        if (!head) {
            return -1;
        }
        const int val{head->val};
        head = head->next;
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        return val;
    }

    void remove(const Node *node)
    {
        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }
    }

    void init() { head = tail = nullptr; }

  private:
    Node *head{nullptr};
    Node *tail{nullptr};
} queue;

NNode **search(int val)
{
    const int head_idx{head_pos >> SHIFT};
    NNode **nnode{&bucket[head_idx]};
    if (!*nnode) {
        return nullptr;
    }
    while ((*nnode)->node->val < head_pos && (*nnode)->next) {
        nnode = &(*nnode)->next;
    }
    return (*nnode)->node->val == head_pos ? nnode : nullptr;
}

NNode **left_nearest()
{
    NNode **nnode{nullptr};
    const int head_idx{head_pos >> SHIFT};
    for (int idx{head_idx}; !nnode && idx >= 0; --idx) {
        if (!bucket_cnt[idx]) {
            continue;
        }

        NNode **n{&bucket[idx]};
        if (!*n) {
            continue;
        }

        while ((*n)->next && (*n)->next->node->val <= head_pos) {
            n = &(*n)->next;
        }
        if ((*n)->node->val <= head_pos) {
            nnode = n;
        }
    }
    return nnode;
}

NNode **right_nearest()
{
    NNode **nnode{nullptr};
    const int head_idx{head_pos >> SHIFT};
    for (int idx{head_idx}; !nnode && idx < BUCKET_CNT; ++idx) {
        if (!bucket_cnt[idx]) {
            continue;
        }

        NNode **n{&bucket[idx]};
        if (!*n) {
            continue;
        }

        while ((*n)->next && (*n)->next->node->val <= head_pos) {
            n = &(*n)->next;
        }
        if ((*n)->node->val >= head_pos) {
            nnode = n;
        } else if ((*n)->next && (*n)->next->node->val > head_pos) {
            nnode = &(*n)->next;
        }
    }
    return nnode;
}

void init(int track_size, int head)
{
    head_pos = head;
    look_left = true;

    node_init();
    bucket_init();
    queue.init();
}

void request(int track)
{
    const Node *node{queue.enqueue(track)};
    const int idx{track >> SHIFT};
    ++bucket_cnt[idx];

    NNode **ptr{&bucket[idx]};
    while (*ptr && (*ptr)->node->val < track) {
        ptr = &(*ptr)->next;
    }

    if (*ptr) {
        *ptr = nnalloc(node, (*ptr)->prev, *ptr);
        (*ptr)->next->prev = *ptr;
    } else {
        *ptr = nnalloc(node);
    }
}

int fcfs()
{
    head_pos = queue.dequeue();
    bucket_remove(search(head_pos));
    return head_pos;
}

int sstf()
{
    NNode **lnn{left_nearest()};
    NNode **rnn{right_nearest()};
    NNode **nn{nullptr};
    if (lnn && rnn) {
        if (head_pos - (*lnn)->node->val <= (*rnn)->node->val - head_pos) {
            nn = lnn;
        } else {
            nn = rnn;
        }
    } else if (lnn) {
        nn = lnn;
    } else if (rnn) {
        nn = rnn;
    }

    const Node *n{(*nn)->node};
    bucket_remove(nn);
    queue.remove(n);

    return head_pos = n->val;
}

int look()
{
    NNode **nn{look_left ? left_nearest() : right_nearest()};
    if (!nn) {
        look_left = !look_left;
        return look();
    }

    const Node *n{(*nn)->node};
    bucket_remove(nn);
    queue.remove(n);

    return head_pos = n->val;
}

int clook()
{
    NNode **nn{left_nearest()};
    if (!nn) {
        head_pos = MAX;
        return clook();
    }

    const Node *n{(*nn)->node};
    bucket_remove(nn);
    queue.remove(n);

    return head_pos = n->val;
}
