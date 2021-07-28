#include <cstring>
#include <iostream>

constexpr long N{20000};
constexpr int MAX_LEN{50};
char names[N][MAX_LEN + 1];
long idx[N];

void swap(long *a, long *b)
{
    long tmp{*a};
    *a = *b;
    *b = tmp;
}

int cmp(const char *a, const char *b)
{
    const int la{static_cast<int>(strlen(a))};
    const int lb{static_cast<int>(strlen(b))};
    if (la == lb) {
        return strcmp(a, b);
    }
    return la - lb;
}

void quick_sort(long begin, long end)
{
    if (end - begin < 2) {
        return;
    }

    long l{begin};
    long r{end - 1};
    const char *pivot{names[idx[(l + r) / 2]]};

    while (l < r) {
        while (cmp(names[idx[l]], pivot) < 0) {
            ++l;
        }
        while (cmp(names[idx[r]], pivot) > 0) {
            --r;
        }
        if (l <= r) {
            swap(&idx[l++], &idx[r--]);
        }
    }

    quick_sort(begin, r + 1);
    quick_sort(l, end);
}

void sort_dedup_print(long cnt)
{
    for (long i{0}; i < cnt; ++i) {
        idx[i] = i;
    }
    quick_sort(0, cnt);

    const char *prev{""};
    for (long i{0}; i < cnt; ++i) {
        if (strcmp(prev, names[idx[i]])) {
            std::cout << names[idx[i]] << '\n';
            prev = names[idx[i]];
        }
    }
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        long cnt;
        std::cin >> cnt;
        for (long i{0}; i < cnt; ++i) {
            std::cin >> names[i];
        }
        std::cout << '#' << test_case << '\n';
        sort_dedup_print(cnt);
    }
    return 0;
}
