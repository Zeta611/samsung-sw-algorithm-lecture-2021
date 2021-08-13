#include <iostream>
#define BOOL(c) (c == 'o')

constexpr int MAX_SIZE{2000};
constexpr int RADIX{3};
unsigned long long powers[MAX_SIZE * MAX_SIZE];

char map[MAX_SIZE][MAX_SIZE + 1];
char submap[MAX_SIZE][MAX_SIZE + 1];

using hash_t = unsigned long long;
hash_t hash_row[MAX_SIZE][MAX_SIZE];
hash_t hash_window[MAX_SIZE];

void init_powers()
{
    powers[0] = 1;
    for (int i{1}; i < MAX_SIZE * MAX_SIZE; ++i) {
        powers[i] = powers[i - 1] * RADIX;
    }
}

int match(int h, int w, int n, int m)
{
    if (h > n || w > m) {
        return 0;
    }

    hash_t hash_submap{0};
    for (int i{0}; i < h; ++i) {
        for (int j{0}; j < w; ++j) {
            hash_submap += BOOL(submap[i][j]) * powers[(h - i) * w - j - 1];
        }
    }

    for (int i{0}; i < n; ++i) {
        for (int j{0}; j <= m - w; ++j) {
            if (j == 0) {
                hash_t hash{0};
                for (int k{0}; k < w; ++k) {
                    hash += BOOL(map[i][k]) * powers[w - k - 1];
                }
                hash_row[i][j] = hash;
            } else {
                hash_row[i][j] =
                    (hash_row[i][j - 1] - BOOL(map[i][j - 1]) * powers[w - 1]) *
                        RADIX +
                    BOOL(map[i][j + w - 1]);
            }
        }
    }

    int cnt{0};
    for (int i{0}; i <= n - h; ++i) {
        for (int j{0}; j <= m - w; ++j) {
            if (i == 0) {
                hash_t hash{0};
                for (int k{0}; k < h; ++k) {
                    hash += hash_row[k][j] * powers[(h - k - 1) * w];
                }
                hash_window[j] = hash;
            } else {
                hash_window[j] = (hash_window[j] -
                                  hash_row[i - 1][j] * powers[w * (h - 1)]) *
                                     powers[w] +
                                 hash_row[i + h - 1][j];
            }
            if (hash_submap == hash_window[j]) {
                ++cnt;
            }
        }
    }
    return cnt;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    init_powers();

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int h, w, n, m;
        std::cin >> h >> w >> n >> m;
        for (int i{0}; i < h; ++i) {
            std::cin >> submap[i];
        }
        for (int i{0}; i < n; ++i) {
            std::cin >> map[i];
        }
        std::cout << '#' << test_case << ' ' << match(h, w, n, m) << '\n';
    }
    return 0;
}
