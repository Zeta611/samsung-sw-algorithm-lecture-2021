#include <iostream>

constexpr long N{200000};
long wearlevel[N], blocks[N];

long min_wearlevel(long n, long k)
{
    long low{0};
    long high{N};
    long min_w{high};
    while (low <= high) {
        const long middle{(low + high) / 2};
        bool possible{true};

        long i{0};
        long j{0};
        while (i < n && j < k) {
            if (i + blocks[j] > n) {
                possible = false;
                break;
            }

            bool block_found{true};
            for (long l{i}; l < i + blocks[j]; ++l) {
                if (wearlevel[l] > middle) {
                    i = l + 1;
                    block_found = false;
                    break;
                }
            }
            if (block_found) {
                i += blocks[j++];
            }
        }

        if (j < k) {
            possible = false;
        }

        if (possible) {
            min_w = middle;
            high = middle - 1;
        } else {
            low = middle + 1;
        }
    }
    return min_w;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        long n, k;
        std::cin >> n >> k;
        for (int i{0}; i < n; ++i) {
            std::cin >> wearlevel[i];
        }
        for (int i{0}; i < k; ++i) {
            std::cin >> blocks[i];
        }
        std::cout << '#' << test_case << ' ' << min_wearlevel(n, k) << '\n';
    }
    return 0;
}
