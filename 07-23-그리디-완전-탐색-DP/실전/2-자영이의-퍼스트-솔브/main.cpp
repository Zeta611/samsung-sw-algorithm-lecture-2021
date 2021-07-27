#include <algorithm>
#include <iostream>

constexpr int N{1000};
constexpr long INF{1000 * 1000000 + 1};
int idx[N + 1];
long dur[N + 1], thres[N + 1];
long dp[N + 1][N + 1];

void init_dp()
{
    for (int i{1}; i <= N; ++i) {
        dp[0][i] = INF;
    }
}

int first_solve(int n)
{
    for (int i{1}; i <= n; ++i) {
        idx[i] = i;
    }
    std::sort(idx + 1, idx + n + 1,
              [](int i, int j) { return thres[i] < thres[j]; });

    for (int i{1}; i <= n; ++i) {
        for (int j{1}; j <= n; ++j) {
            long opt{std::min(dp[i - 1][j], dp[i - 1][j - 1] + dur[idx[i]])};
            dp[i][j] = opt > thres[idx[i]] ? INF : opt;
        }
    }

    int i{n};
    while (dp[n][i] == INF && i > 0) {
        --i;
    }
    return i;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    init_dp();

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int n;
        std::cin >> n;
        for (int i{1}; i <= n; ++i) {
            std::cin >> dur[i] >> thres[i];
        }
        std::cout << '#' << test_case << ' ' << first_solve(n) << '\n';
    }
    return 0;
}
