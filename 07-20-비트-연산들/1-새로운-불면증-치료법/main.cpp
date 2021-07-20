#include <iostream>

long count(unsigned long long n)
{
    constexpr unsigned fullset{(1U << 10) - 1U};
    unsigned bitset{0U};
    unsigned long long m{n};

    while (bitset != fullset) {
        for (auto copy = m; copy; copy /= 10) {
            bitset |= 1U << (copy % 10);
        }
        m += n;
    }

    return m - n;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;

    for (int test_case = 1; test_case <= T; ++test_case) {
        unsigned long long n;
        std::cin >> n;
        std::cout << '#' << test_case << ' ' << count(n) << '\n';
    }

    return 0;
}
