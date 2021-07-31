#include <iostream>

constexpr int INF{200};
constexpr int N{12};
int width;
int cnt;

bool map[N][N];
int core_x[N], core_y[N];

enum Dir { UP, DOWN, LEFT, RIGHT };
constexpr int dx[]{0, 0, -1, 1};
constexpr int dy[]{-1, 1, 0, 0};

struct Candidate {
    int core_cnt;
    int wire_len;
};

Candidate dfs(int depth, int core_cnt, int wire_len)
{
    if (depth == cnt) {
        return {core_cnt, wire_len};
    }

    int max_cores{0};
    int min_wire{INF};

    const int cx{core_x[depth]};
    const int cy{core_y[depth]};
    bool fix_dir{false};
    Dir dir;
    if (cx == 0) {
        fix_dir = true;
        dir = LEFT;
    } else if (cx == width - 1) {
        fix_dir = true;
        dir = RIGHT;
    } else if (cy == 0) {
        fix_dir = true;
        dir = UP;
    } else if (cy == width - 1) {
        fix_dir = true;
        dir = DOWN;
    }

    for (int i{0}; i < 4; ++i) {
        if (fix_dir && i != dir) {
            continue;
        }

        int added_x[N], added_y[N];
        int len{0};

        int x{cx};
        int y{cy};
        bool possible{true};

        while (true) {
            x += dx[i];
            y += dy[i];
            if (x < 0 || x >= width || y < 0 || y >= width) {
                break;
            }
            if (map[y][x]) {
                possible = false;
                break;
            } else {
                map[y][x] = true;
                added_x[len] = x;
                added_y[len++] = y;
            }
        }

        Candidate cand;
        if (possible) {
            if (core_cnt + cnt - depth >= max_cores) {
                cand = dfs(depth + 1, core_cnt + 1, wire_len + len);
                if (cand.core_cnt > max_cores) {
                    max_cores = cand.core_cnt;
                    min_wire = cand.wire_len;
                } else if (cand.core_cnt == max_cores &&
                           min_wire > cand.wire_len) {
                    min_wire = cand.wire_len;
                }
            }
            for (int j{0}; j < len; ++j) {
                map[added_y[j]][added_x[j]] = false;
            }
        } else {
            for (int j{0}; j < len; ++j) {
                map[added_y[j]][added_x[j]] = false;
            }
            if (core_cnt + cnt - depth - 1 >= max_cores) {
                cand = dfs(depth + 1, core_cnt, wire_len);
                if (cand.core_cnt > max_cores) {
                    max_cores = cand.core_cnt;
                    min_wire = cand.wire_len;
                } else if (cand.core_cnt == max_cores &&
                           min_wire > cand.wire_len) {
                    min_wire = cand.wire_len;
                }
            }
        }
    }

    return {max_cores, min_wire};
}

int min_cable(int n)
{
    width = n;
    cnt = 0;
    for (int y{0}; y < n; ++y) {
        for (int x{0}; x < n; ++x) {
            if (map[y][x]) {
                core_x[cnt] = x;
                core_y[cnt++] = y;
            }
        }
    }
    return dfs(0, 0, 0).wire_len;
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        int n;
        std::cin >> n;
        for (int y{0}; y < n; ++y) {
            for (int x{0}; x < n; ++x) {
                std::cin >> map[y][x];
            }
        }
        std::cout << '#' << test_case << ' ' << min_cable(n) << '\n';
    }
    return 0;
}
