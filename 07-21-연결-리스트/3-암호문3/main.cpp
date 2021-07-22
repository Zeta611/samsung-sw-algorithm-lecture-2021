#include <iostream>

constexpr std::size_t SIZE{10000};

struct Node {
    long num;
    Node *next;
} pool[SIZE];

std::size_t list_cnt{0};
Node *head, *tail;

Node *alloc(long num)
{
    Node *node{&pool[list_cnt++]};
    node->num = num;
    node->next = nullptr;
    return node;
}

void append(std::size_t count)
{
    Node *node{tail};
    while (count--) {
        long num;
        std::cin >> num;
        if (!tail) {
            head = tail = alloc(num);
        } else {
            tail->next = alloc(num);
            tail = tail->next;
        }
    }
}

void insert(std::size_t pos, std::size_t count)
{
    Node **p{&head};
    while (pos--) {
        p = &(*p)->next;
    }
    Node *node;
    while (count--) {
        long num;
        std::cin >> num;
        node = alloc(num);
        node->next = *p;
        *p = node;
        p = &(*p)->next;
    }
    if (!*p) {
        tail = node;
    }
}

void remove(std::size_t pos, std::size_t count)
{
    Node **p{&head};
    Node *last{nullptr};
    while (pos--) {
        last = *p;
        p = &(*p)->next;
    }
    while (count--) {
        *p = (*p)->next;
    }
    if (!*p) {
        tail = last;
    }
}

void init_list(std::size_t count)
{
    list_cnt = 0;
    head = tail = nullptr;
    append(count);
}

void print_list(std::size_t count)
{
    Node *p{head};
    while (p && count--) {
        std::cout << p->num;
        p = p->next;
        if (p && count) {
            std::cout << ' ';
        }
    }
    std::cout << '\n';
}

void operate()
{
    char op;
    std::cin >> op;
    std::size_t pos, count;

    switch (op) {
    case 'I':
        std::cin >> pos >> count;
        insert(pos, count);
        break;
    case 'D':
        std::cin >> pos >> count;
        remove(pos, count);
        break;
    case 'A':
        std::cin >> count;
        append(count);
        break;
    }
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    constexpr int T{10};
    for (int test_case = 1; test_case <= T; ++test_case) {
        std::size_t n;
        // Length of the code
        std::cin >> n;
        init_list(n);
        // Number of operations
        std::cin >> n;
        while (n--) {
            operate();
        }
        std::cout << '#' << test_case << ' ';
        print_list(10);
    }
    return 0;
}
