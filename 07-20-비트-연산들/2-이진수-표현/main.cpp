#include <iostream>

inline bool lower_on(int n, long m) { return ((m & (1 << n) - 1) + 1) >> n; }

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case = 1; test_case <= T; ++test_case) {
        int n;
        long m;
        std::cin >> n >> m;
        std::cout << '#' << test_case << (lower_on(n, m) ? " ON\n" : " OFF\n");
    }
    return 0;
}
