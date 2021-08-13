#include <cstring>
#include <iostream>

constexpr int HASH_SIZE{200003};
constexpr int MAX_SIZE{100000};
constexpr int MAX_LEN{50};

char set1[MAX_SIZE][MAX_LEN + 1];
char set2[MAX_SIZE][MAX_LEN + 1];

int table1[HASH_SIZE];

unsigned long djb2(const char *str)
{
    unsigned long hash{5381};
    char c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

inline void increment(int &hash)
{
    ++hash;
    if (hash == HASH_SIZE) {
        hash = 0;
    }
}

void put(int *table, const char (*set)[MAX_LEN + 1], int i)
{
    int hash{static_cast<int>(djb2(set[i]) % HASH_SIZE)};
    while (table[hash] >= 0) {
        increment(hash);
    }
    table[hash] = i;
}

int get(int *table, const char (*set)[MAX_LEN + 1], const char *str)
{
    int hash{static_cast<int>(djb2(str) % HASH_SIZE)};
    while (table[hash] >= 0) {
        if (strcmp(str, set[table[hash]]) == 0) {
            return hash;
        }
        increment(hash);
    };
    return -1;
}

int intersection(int n, int m)
{
    for (int i{0}; i < HASH_SIZE; ++i) {
        table1[i] = -1;
    }

    for (int i{0}; i < n; ++i) {
        put(table1, set1, i);
    }

    int cnt{0};
    for (int i{0}; i < m; ++i) {
        int j = get(table1, set1, set2[i]);
        if (j >= 0) {
            ++cnt;
        }
    }
    return cnt;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int n, m;
        std::cin >> n >> m;
        for (int i{0}; i < n; ++i) {
            std::cin >> set1[i];
        }
        for (int i{0}; i < m; ++i) {
            std::cin >> set2[i];
        }
        std::cout << '#' << test_case << ' ' << intersection(n, m) << '\n';
    }
    return 0;
}
