#include <iostream>
/* The actual 'scheduling' can be done via the interval graph coloring problem.
 * Since the corresponding paths for the maximally overlapping 'point' can be
 * scheduled greedily, the following algorithm works. */

constexpr int N{200};

int min_time(int *src, int *dest, int size)
{
    int corridor[N]{};
    for (int i{0}; i < size; ++i) {
        const int s{(src[i] - 1) / 2};
        const int d{(dest[i] - 1) / 2};
        for (int j{s < d ? s : d}; j <= (s < d ? d : s); ++j) {
            ++corridor[j];
        }
    }

    int m{0};
    for (int overlap : corridor) {
        if (m < overlap) {
            m = overlap;
        }
    }
    return m;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;

    int src[N], dest[N];
    for (int test_case{1}; test_case <= T; ++test_case) {
        int n;
        std::cin >> n;
        for (int i{0}; i < n; ++i) {
            std::cin >> src[i] >> dest[i];
        }

        std::cout << '#' << test_case << ' ' << min_time(src, dest, n) << '\n';
    }
    return 0;
}
