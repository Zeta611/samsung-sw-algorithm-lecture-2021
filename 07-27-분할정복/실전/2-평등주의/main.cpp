#include <iostream>

constexpr long N{100000};
long arr[N], old[N];

long abs(long n) { return n >= 0 ? n : -n; }
long max(long a, long b) { return a >= b ? a : b; }

long min_adj(long len, long op)
{
    long max_d{0};
    for (long i{0}; i < len - 1; ++i) {
        max_d = max(max_d, abs(arr[i + 1] - arr[i]));
    }

    for (long i{0}; i < len; ++i) {
        old[i] = arr[i];
    }

    long min_max{max_d};
    long low{0}, high{max_d - 1};
    while (low <= high) {
        const long middle{(low + high) / 2};
        long cnt{0};
        bool possible{true};

        for (long i{0}; i < len; ++i) {
            arr[i] = old[i];
        }

        for (long i{0}; i < len - 1; ++i) {
            long goal;
            if (arr[i + 1] > (goal = arr[i] + middle)) {
                cnt += arr[i + 1] - goal;
                arr[i + 1] = goal;
                if (cnt > op) {
                    possible = false;
                    break;
                }
            }
        }
        for (long i{len - 1}; i > 0; --i) {
            long goal;
            if (arr[i - 1] > (goal = arr[i] + middle)) {
                cnt += arr[i - 1] - goal;
                arr[i - 1] = goal;
                if (cnt > op) {
                    possible = false;
                    break;
                }
            }
        }

        if (possible) {
            min_max = middle;
            high = middle - 1;
        } else {
            low = middle + 1;
        }
    }

    return min_max;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        long len, op;
        std::cin >> len >> op;
        for (int i{0}; i < len; ++i) {
            std::cin >> arr[i];
        }
        std::cout << '#' << test_case << ' ' << min_adj(len, op) << '\n';
    }
    return 0;
}
