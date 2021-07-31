#include <iostream>

constexpr int N{300};
bool map[N][N], fill[N][N];
int width;

constexpr int DIR{8};
constexpr int dx[DIR]{1, 1, 0, -1, -1, -1, 0, 1};
constexpr int dy[DIR]{0, 1, 1, 1, 0, -1, -1, -1};

inline bool in_range(int x, int y)
{
    return x >= 0 && x < width && y >= 0 && y < width;
}

bool adjacent(int x, int y)
{
    for (int i{0}; i < DIR; ++i) {
        const int nx{x + dx[i]};
        const int ny{y + dy[i]};
        if (in_range(nx, ny) && map[ny][nx]) {
            return true;
        }
    }
    return false;
}

long start, end;
int queue_x[N * N], queue_y[N * N];
inline void init() { start = end = 0; }
inline bool empty() { return start == end; }
inline void enqueue(int x, int y)
{
    queue_x[end] = x;
    queue_y[end++] = y;
}
inline void dequeue(int &x, int &y)
{
    x = queue_x[start];
    y = queue_y[start++];
}

void flood_fill(int x, int y)
{
    fill[y][x] = true;
    init();

    enqueue(x, y);
    while (!empty()) {
        dequeue(x, y);
        for (int i{0}; i < DIR; ++i) {
            const int nx{x + dx[i]};
            const int ny{y + dy[i]};
            if (!in_range(nx, ny) || fill[ny][nx] || map[ny][nx]) {
                continue;
            }
            fill[ny][nx] = true;
            if (!adjacent(nx, ny)) {
                enqueue(nx, ny);
            }
        }
    }
}

long click()
{
    for (int y{0}; y < width; ++y) {
        for (int x{0}; x < width; ++x) {
            fill[y][x] = false;
        }
    }

    long cnt{0};
    for (int y{0}; y < width; ++y) {
        for (int x{0}; x < width; ++x) {
            if (map[y][x] || fill[y][x]) {
                continue;
            }
            if (adjacent(x, y)) {
                bool zero_found{false};
                for (int i{0}; i < DIR; ++i) {
                    const int nx{x + dx[i]};
                    const int ny{y + dy[i]};
                    if (!in_range(nx, ny) || map[ny][nx] || fill[ny][nx] ||
                        adjacent(nx, ny)) {
                        continue;
                    }
                    zero_found = true;
                    flood_fill(nx, ny);
                    ++cnt;
                    break;
                }
                if (!zero_found) {
                    fill[y][x] = true;
                    ++cnt;
                }
            } else {
                ++cnt;
                flood_fill(x, y);
            }
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
        std::cin >> width;
        for (int y{0}; y < width; ++y) {
            for (int x{0}; x < width; ++x) {
                char c;
                std::cin >> c;
                map[y][x] = c == '*';
            }
        }
        std::cout << '#' << test_case << ' ' << click() << '\n';
    }
    return 0;
}
