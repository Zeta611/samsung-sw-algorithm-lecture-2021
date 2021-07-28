#include <iostream>

constexpr long N{100000};
long start[N], end[N], accum[N + 1];

long max(long a, long b) { return a > b ? a : b; }

void swap(long *a, long *b)
{
    long tmp{*a};
    *a = *b;
    *b = tmp;
}

void quick_sort(long *begin, long *end)
{
    if (end - begin < 2) {
        return;
    }

    long *l{begin};
    long *r{end - 1};
    const long pivot{*(begin + (end - begin) / 2)};

    while (l < r) {
        while (*l < pivot) {
            ++l;
        }
        while (*r > pivot) {
            --r;
        }
        if (l <= r) {
            swap(l++, r--);
        }
    }

    quick_sort(begin, r + 1);
    quick_sort(l, end);
}

long max_overlap(long len, long cnt)
{
    quick_sort(start, start + cnt);
    quick_sort(end, end + cnt);

    for (long i{0}; i < cnt; ++i) {
        accum[i + 1] = accum[i] + end[i] - start[i];
    }

    long opt{0};
    for (long i{0}; i < cnt; ++i) {
        const long lim{start[i] + len};

        long low{i};
        long high{cnt - 1};
        while (low <= high) {
            const long middle{(low + high) / 2};
            if (start[middle] >= lim) {
                high = middle - 1;
            } else if (end[middle] <= lim) {
                low = middle + 1;
                opt = max(opt, accum[middle + 1] - accum[i]);
            } else {
                opt = max(opt, accum[middle] - accum[i] + lim - start[middle]);
                break;
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
        long len, cnt;
        std::cin >> len >> cnt;
        for (int i{0}; i < cnt; ++i) {
            std::cin >> start[i] >> end[i];
        }

        std::cout << '#' << test_case << ' ' << max_overlap(len, cnt) << '\n';
    }
    return 0;
}
