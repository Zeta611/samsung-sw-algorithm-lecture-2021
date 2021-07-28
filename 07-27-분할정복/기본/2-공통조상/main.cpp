#include <iostream>

constexpr int N{10000};

int parents[N + 1];

struct Node {
    int left;
    int right;
} tree[N + 1];

void init_tree()
{
    for (int i{1}; i <= N; ++i) {
        tree[i] = Node();
    }
}

int tree_size(int v)
{
    if (!v) {
        return 0;
    }
    return 1 + tree_size(tree[v].left) + tree_size(tree[v].right);
}

int lowest_common_ancestor(int v1, int v2)
{
    int depth1{0};
    int depth2{0};
    for (int n{v1}; n != 1; n = parents[n]) {
        ++depth1;
    }
    for (int n{v2}; n != 1; n = parents[n]) {
        ++depth2;
    }

    if (depth1 > depth2) {
        int tmp{depth1};
        depth1 = depth2;
        depth2 = tmp;
        tmp = v1;
        v1 = v2;
        v2 = tmp;
    }

    while (depth2 > depth1) {
        --depth2;
        v2 = parents[v2];
    }

    while (v1 != v2) {
        v1 = parents[v1];
        v2 = parents[v2];
    }

    return v1;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int vertices, edges, v1, v2;
        std::cin >> vertices >> edges >> v1 >> v2;

        init_tree();
        for (int i{0}; i < edges; ++i) {
            int parent, child;
            std::cin >> parent >> child;

            parents[child] = parent;
            if (!tree[parent].left) {
                tree[parent].left = child;
            } else {
                tree[parent].right = child;
            }
        }

        const int lcs{lowest_common_ancestor(v1, v2)};
        std::cout << '#' << test_case << ' ' << lcs << ' ' << tree_size(lcs)
                  << '\n';
    }
    return 0;
}
