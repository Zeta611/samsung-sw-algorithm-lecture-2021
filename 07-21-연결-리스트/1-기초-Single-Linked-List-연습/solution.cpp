#define MAX_NODE 10000

struct Node {
    int data;
    Node *next;
};

Node node[MAX_NODE];
int nodeCnt;
Node *head;

Node *getNode(int data)
{
    node[nodeCnt].data = data;
    node[nodeCnt].next = nullptr;
    return &node[nodeCnt++];
}

void init()
{
    nodeCnt = 1;
    head = node;
    head->next = nullptr;
}

void addNode2Head(int data)
{
    Node *tmp{getNode(data)};
    tmp->next = head->next;
    head->next = tmp;
}

void addNode2Tail(int data)
{
    Node *p{head};
    while (p->next) {
        p = p->next;
    }
    p->next = getNode(data);
}

void addNode2Num(int data, int num)
{
    Node *p{head};
    while (--num) {
        p = p->next;
    }
    Node *tmp{getNode(data)};
    tmp->next = p->next;
    p->next = tmp;
}

void removeNode(int data)
{
    Node **p{&head->next};
    while (*p) {
        if ((*p)->data == data) {
            *p = (*p)->next;
            break;
        }
        p = &(*p)->next;
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
