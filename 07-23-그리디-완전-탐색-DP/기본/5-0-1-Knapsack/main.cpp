#include <iostream>

constexpr int N{100};
constexpr int K{1000};
int weight[N + 1], value[N + 1];
int dp[N + 1][K + 1];

inline int max(int a, int b) { return a > b ? a : b; }

int knapsack(int count, int capacity)
{
    for (int n{1}; n <= count; ++n) {
        for (int w{1}; w <= capacity; ++w) {
            const int wn{weight[n]};
            if (wn > w) {
                dp[n][w] = dp[n - 1][w];
            } else {
                dp[n][w] = max(dp[n - 1][w], value[n] + dp[n - 1][w - wn]);
            }
        }
    }
    return dp[count][capacity];
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;

    for (int test_case{1}; test_case <= T; ++test_case) {
        int count, capacity;
        std::cin >> count >> capacity;
        for (int i{1}; i <= count; ++i) {
            std::cin >> weight[i] >> value[i];
        }
        std::cout << '#' << test_case << ' ' << knapsack(count, capacity)
                  << '\n';
    }
    return 0;
}
