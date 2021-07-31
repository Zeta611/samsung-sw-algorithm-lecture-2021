#include <cmath>
#include <iostream>

constexpr long long INF{9223372036854775807LL};
constexpr int N{1000};
long long x[N], y[N];

struct Node {
    int idx;
    long long dist;
    Node *next;
} pool[N];
int ncnt{0};
Node *pqueue{nullptr};

Node *alloc(int idx, Node *next = nullptr)
{
    Node *node{&pool[ncnt++]};
    *node = {idx, INF, next};
    return node;
}

void add(int idx, Node **list) { *list = alloc(idx, *list); }

void init_vertices()
{
    ncnt = 0;
    pqueue = nullptr;
}

inline long long square(long long n) { return n * n; }
long long sqdist(int i, int j)
{
    return square(x[i] - x[j]) + square(y[i] - y[j]);
}

long long min_price(int cnt, double coeff)
{
    long long price{0};

    init_vertices();
    for (int i{cnt - 1}; i >= 0; --i) {
        add(i, &pqueue);
    }
    pqueue->dist = 0;

    while (pqueue) {
        long long min_d{INF};
        Node **min_n;
        for (Node **n{&pqueue}; *n; n = &(*n)->next) {
            if ((*n)->dist < min_d) {
                min_d = (*n)->dist;
                min_n = n;
            }
        }
        Node *node{*min_n};
        *min_n = node->next;
        price += min_d;

        for (Node *n{pqueue}; n; n = n->next) {
            const long long d{sqdist(n->idx, node->idx)};
            if (n->dist > d) {
                n->dist = d;
            }
        }
    }

    return std::round(price * coeff);
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int cnt;
        std::cin >> cnt;
        for (int i{0}; i < cnt; ++i) {
            std::cin >> x[i];
        }
        for (int i{0}; i < cnt; ++i) {
            std::cin >> y[i];
        }
        double coeff;
        std::cin >> coeff;
        std::cout << '#' << test_case << ' ' << min_price(cnt, coeff) << '\n';
    }
    return 0;
}
