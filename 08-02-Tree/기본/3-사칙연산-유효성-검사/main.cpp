#include <iostream>

constexpr int N{200};
constexpr int BUF_SIZE{8};
bool node[N + 1];
int left[N + 1];
int right[N + 1];

inline bool leaf(int n, int v) { return v > n / 2; }
inline bool has_right(int n, int v) { return v * 2 < n; }

bool verify(int n, int v = 1)
{
    if (v > n) {
        return true;
    }
    if (leaf(n, v)) {
        return !node[v];
    }
    return node[v] && verify(n, left[v]) && verify(n, right[v]);
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    constexpr int T{10};
    char buf[BUF_SIZE];
    for (int test_case{1}; test_case <= T; ++test_case) {
        int n;
        std::cin >> n;
        for (int v{1}; v <= n; ++v) {
            int idx, l, r;
            std::cin >> idx >> buf;
            if (!leaf(n, v)) {
                std::cin >> l;
                left[idx] = l;
                if (has_right(n, v)) {
                    std::cin >> r;
                    right[idx] = r;
                }
            }
            node[idx] = buf[0] < '0';
        }
        std::cout << '#' << test_case << ' ' << verify(n) << '\n';
    }
    return 0;
}
