#include <iostream>

constexpr long N{200000};
long days[N], start[N], end[N], accum[N + 1];

long max(long a, long b) { return a > b ? a : b; }

long check(long cnt, long extra)
{
    bool inside{false};
    long i{0};
    long chunks{0};
    for (int i{0}; i < cnt; ++i) {
        if (!inside) {
            start[chunks] = days[i];
            inside = true;
        }
        if (inside && (i + 1 == cnt || days[i + 1] - days[i] > 1)) {
            end[chunks] = days[i];
            accum[chunks + 1] = accum[chunks] + end[chunks] - start[chunks] + 1;
            ++chunks;
            inside = false;
        }
    }

    long opt{0};
    for (long i{0}; i < chunks; ++i) {
        long low{i};
        long high{chunks - 1};
        while (low <= high) {
            const long middle{(low + high) / 2};
            const long dur{end[middle] - start[i] + 1};
            const long diff{dur - accum[middle + 1] + accum[i]};
            if (diff == extra) {
                opt = max(opt, dur);
                break;
            } else if (diff < extra) {
                low = middle + 1;
                opt = max(opt, dur + extra - diff);
            } else {
                high = middle - 1;
            }
        }
    }

    return opt;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        long cnt, extra;
        std::cin >> cnt >> extra;
        for (int i{0}; i < cnt; ++i) {
            std::cin >> days[i];
        }
        std::cout << '#' << test_case << ' ' << check(cnt, extra) << '\n';
    }

    return 0;
}
