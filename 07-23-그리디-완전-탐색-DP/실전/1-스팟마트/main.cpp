#include <algorithm>
#include <iostream>

constexpr int N{3000}, M{100};
long group1[N + 1], group2[M + 1];
long dp[M + 2][M + 2][3];

inline int max(int a, int b) { return a > b ? a : b; }

long choose(int n, int m)
{
    std::sort(group2 + 1, group2 + m + 1);

    for (int d{0}; d <= m; ++d) {
        for (int l{1}; l <= m - d + 1; ++l) {
            const int r{l + d};
            dp[l][r][0] = dp[l][r][1] = dp[l][r][2] = 0;
        }
    }

    for (int i{1}; i <= n; ++i) {
        for (int d{0}; d <= m; ++d) {
            for (int l{1}; l <= m - d + 1; ++l) {
                const int r{l + d};
                dp[l][r][0] = dp[l][r][1];
                dp[l][r][1] = dp[l][r][2];

                long opt{max(dp[l][r][1], dp[l][r][0] + group1[i])};
                if (l < r) {
                    opt = max(opt, max(dp[l + 1][r][1] + group1[i],
                                       dp[l][r - 1][1] + group2[r - 1]));
                }
                if (l < r - 1) {
                    opt = max(opt, dp[l + 1][r - 1][2] + group2[r - 1]);
                }
                dp[l][r][2] = opt;
            }
        }
    }

    return dp[1][m + 1][2];
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int n;
        std::cin >> n;
        for (int i{1}; i <= n; ++i) {
            std::cin >> group1[i];
        }
        int m;
        std::cin >> m;
        for (int i{1}; i <= m; ++i) {
            std::cin >> group2[i];
        }
        std::cout << '#' << test_case << ' ' << choose(n, m) << '\n';
    }
    return 0;
}
