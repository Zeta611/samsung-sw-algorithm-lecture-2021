#include <iostream>

void exchange(long amount)
{
    constexpr int denom[]{50000, 10000, 5000, 1000, 500, 100, 50, 10};
    for (int d : denom) {
        std::cout << amount / d;
        if (d != 10) {
            std::cout << ' ';
        }
        amount %= d;
    }
    std::cout << '\n';
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        long amount;
        std::cin >> amount;
        std::cout << '#' << test_case << '\n';
        exchange(amount);
    }
    return 0;
}
