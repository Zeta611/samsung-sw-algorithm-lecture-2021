#define MAX_NODE 10000

struct Node {
    int data;
    Node *prev;
    Node *next;
};

Node node[MAX_NODE];
int nodeCnt;
Node *head;

Node *getNode(int data)
{
    node[nodeCnt].data = data;
    node[nodeCnt].prev = nullptr;
    node[nodeCnt].next = nullptr;
    return &node[nodeCnt++];
}

void init()
{
    nodeCnt = 1;
    head = node;
    head->next = head->prev = nullptr;
}

void addNode2Head(int data)
{
    Node *tmp{getNode(data)};
    tmp->next = head->next;
    tmp->prev = head;
    if (head->next) {
        head->next->prev = tmp;
    }
    head->next = tmp;
}

void addNode2Tail(int data)
{
    Node *p{head};
    while (p->next) {
        p = p->next;
    }
    p->next = getNode(data);
    p->next->prev = p;
}

void addNode2Num(int data, int num)
{
    Node *p{head};
    while (--num) {
        p = p->next;
    }
    Node *tmp{getNode(data)};
    tmp->next = p->next;
    tmp->prev = p;
    if (p->next) {
        p->next->prev = tmp;
    }
    p->next = tmp;
}

int findNode(int data)
{
    int i{1};
    Node *p{head};
    for (Node *p{head->next}; p && p->data != data; p = p->next, ++i)
        ;
    return i;
}

void removeNode(int data)
{
    Node *p{head->next};
    while (p) {
        if (p->data == data) {
            if (p->next) {
                p->next->prev = p->prev;
            }
            p->prev->next = p->next;
            break;
        }
        p = p->next;
    }
}

int getList(int output[MAX_NODE])
{
    int i{0};
    for (Node *p{head->next}; p; p = p->next) {
        output[i++] = p->data;
    }
    return i;
}

int getReversedList(int output[MAX_NODE])
{
    Node *p{head->next};
    if (!p) {
        return 0;
    }
    int i{1};
    while (p->next) {
        p = p->next;
        ++i;
    }
    const int cnt{i};
    i = 0;
    while (p != head) {
        output[i++] = p->data;
        p = p->prev;
    }
    return cnt;
}
