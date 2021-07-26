#include <iostream>

constexpr int N{1000};
int dp[N + 1][N + 1]{};

inline int max(int a, int b) { return a > b ? a : b; }

int lcs(const char *str1, const char *str2)
{
    int len1{0}, len2{0};
    const char *p1{str1}, *p2{str2};
    for (char c{*p1}; c; c = *++p1) {
        ++len1;
    }
    for (char c{*p2}; c; c = *++p2) {
        ++len2;
    }

    for (int i{0}; i <= len1; ++i) {
        dp[i][len2] = 0;
    }
    for (int j{0}; j <= len2; ++j) {
        dp[len1][j] = 0;
    }

    for (int i{len1 - 1}; i >= 0; --i) {
        for (int j{len2 - 1}; j >= 0; --j) {
            if (str1[i] == str2[j]) {
                dp[i][j] = dp[i + 1][j + 1] + 1;
            } else {
                dp[i][j] = max(dp[i][j + 1], dp[i + 1][j]);
            }
        }
    }

    return dp[0][0];
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;

    char str1[N + 1], str2[N + 1];
    for (int test_case{1}; test_case <= T; ++test_case) {
        std::cin >> str1 >> str2;
        std::cout << '#' << test_case << ' ' << lcs(str1, str2) << '\n';
    }
    return 0;
}
