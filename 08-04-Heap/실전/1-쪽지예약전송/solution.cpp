constexpr int INF{2147483647};
constexpr int MAX_MSGS{100000};
constexpr int MAX_USRS{1000};

struct Message {
    int time;
    int schedule;

    int sender;
    int receiver;

    bool received;
    bool sender_read;
    bool sender_deleted;
    bool receiver_read;
    bool receiver_deleted;
} msgs[MAX_MSGS + 1];
int msg_cnt;
int msg_alloc(int time, int schedule, int sender, int receiver)
{
    msgs[++msg_cnt] = {time,  schedule, sender, receiver, false,
                       false, false,    false,  false};
    return msg_cnt;
}

struct MsgNode {
    int msg;
    int prev;
    int next;
} nodes[2 * (MAX_MSGS + MAX_USRS)];
int node_cnt;
int node_alloc(int msg, int prev = -1, int next = -1)
{
    nodes[node_cnt] = {msg, prev, next};
    return node_cnt++;
}
void node_remove(int header, int node)
{
    const int prev{nodes[node].prev};
    const int next{nodes[node].next};
    nodes[prev].next = next;
    nodes[next].prev = prev;
    --nodes[header].msg;
}
void node_add(int header, int node)
{
    const int next{nodes[header].next};
    nodes[node].next = nodes[header].next;
    nodes[node].prev = header;
    nodes[next].prev = node;
    nodes[header].next = node;
    ++nodes[header].msg;
}
int recent_sent[MAX_USRS + 1];
int recent_received[MAX_USRS + 1];

class MinHeap
{
    int _size{0};
    int arr[MAX_MSGS];

    static inline int parent(int i) { return (i - 1) >> 1; }
    static inline int left(int i) { return (i << 1) + 1; }
    static inline int right(int i) { return (i + 1) << 1; }
    inline int val(int i) const { return msgs[arr[i]].schedule; }
    inline bool cmp(int i, int j) const
    {
        return val(i) > val(j) || val(i) == val(j) && arr[i] > arr[j];
    }

    void min_heapify(int i)
    {
        const int l{left(i)};
        const int r{right(i)};
        int min{l < _size && cmp(i, l) ? l : i};
        if (r < _size && cmp(min, r)) {
            min = r;
        }
        if (min != i) {
            const int tmp{arr[i]};
            arr[i] = arr[min];
            arr[min] = tmp;
            min_heapify(min);
        }
    }

    void decrease_key(int i, int key)
    {
        msgs[arr[i]].schedule = key;
        int p{parent(i)};
        while (i && cmp(p, i)) {
            const int tmp{arr[i]};
            arr[i] = arr[p];
            arr[p] = tmp;
            i = p;
            p = parent(i);
        }
    }

  public:
    inline void init() { _size = 0; }
    inline bool empty() const { return _size == 0; }
    inline int min() { return arr[0]; }

    int extract_min()
    {
        const int msg{arr[0]};
        arr[0] = arr[--_size];
        min_heapify(0);
        return msg;
    }

    void insert(int msg)
    {
        arr[_size] = msg;
        const long t{msgs[msg].schedule};
        msgs[msg].schedule = INF;
        decrease_key(_size++, t);
    }
} msg_queue;

void update(int now)
{
    while (!msg_queue.empty() && msgs[msg_queue.min()].schedule <= now) {
        const int msg{msg_queue.extract_min()};
        if (msgs[msg].sender_deleted) {
            continue;
        }
        node_add(recent_received[msgs[msg].receiver], node_alloc(msg));
        msgs[msg].received = true;
    }
}

void init(int n)
{
    msg_cnt = 0;
    msg_queue.init();
    for (int usr{1}; usr <= n; ++usr) {
        recent_sent[usr] = node_alloc(0, node_cnt, node_cnt);
    }
    for (int usr{1}; usr <= n; ++usr) {
        recent_received[usr] = node_alloc(0, node_cnt, node_cnt);
    }
}

void sendMessage(int now, int sender, int receiver, int msg, int schedule)
{
    msg_queue.insert(msg_alloc(now, schedule, sender, receiver));
    node_add(recent_sent[sender], node_alloc(msg));
}

int retrieveSentbox(int now, int sender, int *dest, int *receivers, int *status)
{
    update(now);

    const int header{recent_sent[sender]};
    int cnt{0};
    for (int n{nodes[header].next}; n != header && cnt < 3; n = nodes[n].next) {
        const int msg{nodes[n].msg};
        if (msgs[msg].sender_deleted) {
            node_remove(header, n);
        } else {
            dest[cnt] = msg;
            receivers[cnt] = msgs[msg].receiver;
            status[cnt++] = msgs[msg].receiver_read;
        }
    }
    return cnt;
}

int retrieveInbox(int now, int receiver, int *dest, int *senders, int *status)
{
    update(now);

    const int header{recent_received[receiver]};
    int cnt{0};
    for (int n{nodes[header].next}; n != header && cnt < 3; n = nodes[n].next) {
        const int msg{nodes[n].msg};
        if (msgs[msg].receiver_deleted) {
            node_remove(header, n);
        } else {
            dest[cnt] = msg;
            senders[cnt] = msgs[msg].sender;
            status[cnt++] = msgs[msg].receiver_read;
        }
    }
    return cnt;
}

int readMessage(int now, int user, int msg)
{
    update(now);

    if (msgs[msg].received && msgs[msg].receiver == user &&
        !msgs[msg].receiver_deleted) {
        msgs[msg].receiver_read = true;
        return 1;
    } else if (msgs[msg].sender == user && !msgs[msg].sender_deleted) {
        msgs[msg].sender_read = true;
        return 1;
    } else {
        return 0;
    }
}

int deleteMessage(int now, int user, int msg)
{
    update(now);

    if (msgs[msg].received && msgs[msg].receiver == user &&
        !msgs[msg].receiver_deleted) {
        msgs[msg].receiver_deleted = true;
        return 1;
    } else if (msgs[msg].sender == user && !msgs[msg].sender_deleted) {
        msgs[msg].sender_deleted = true;
        return 1;
    } else {
        return 0;
    }
}
