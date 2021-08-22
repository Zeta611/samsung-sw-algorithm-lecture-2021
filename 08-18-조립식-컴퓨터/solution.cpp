#include <algorithm>
#include <cstring>

constexpr int SIDES{4};
constexpr int MAX_BLOCKS{750};
constexpr int MAX_WIDTH{50};

enum Side { UP, RIGHT, DOWN, LEFT };

int width;
int min_x, max_x, min_y, max_y;
int lookup[2 * (MAX_BLOCKS + 1) + 1][2 * (MAX_BLOCKS + 1) + 1];

struct Block {
    int x;
    int y;
    int teeth[SIDES][MAX_WIDTH];
} blocks[MAX_BLOCKS + 1];
int block_cnt;
inline int balloc(int x, int y)
{
    blocks[block_cnt].x = x;
    blocks[block_cnt].y = y;
    return lookup[MAX_BLOCKS + x][MAX_BLOCKS + y] = block_cnt++;
}

void init(int n, int (*teeth)[MAX_WIDTH])
{
    block_cnt = min_x = max_x = min_y = max_y = 0;
    width = n;

    std::memset(lookup, -1, sizeof lookup);

    balloc(0, 0);
    for (Side side{UP}; side <= LEFT;
         side = static_cast<Side>(static_cast<int>(side) + 1)) {
        for (int i{0}; i < width; ++i) {
            blocks[0].teeth[side][i] = teeth[side][i];
        }
    }
}

constexpr bool match_table[SIDES + 1][SIDES + 1]{
    {},
    {false, false, true},
    {false, true},
    {false, false, false, false, true},
    {false, false, false, true}};
bool teeth_match(int *teeth1, int *teeth2)
{
    for (int i{0}; i < width; ++i) {
        if (!match_table[teeth1[i]][teeth2[width - i - 1]]) {
            return false;
        }
    }
    return true;
}

//      1           2          -          0
// UP: 230  RIGHT: |31  DOWN: 032  LEFT: 13|
//      -           0          1          2
constexpr int rel_pos_diff[SIDES][SIDES][2]{
    {{1, 1}, {0, 2}, {-1, 1}, {0, 1}},
    {{1, -1}, {2, 0}, {1, 1}, {1, 0}},
    {{-1, -1}, {0, -2}, {1, -1}, {0, -1}},
    {{-1, 1}, {-2, 0}, {-1, -1}, {-1, 0}},
};
constexpr Side rel_side_diff[SIDES - 1]{LEFT, DOWN, RIGHT};

bool assemble(int (*teeth)[MAX_WIDTH])
{
    int min_prod{MAX_BLOCKS * MAX_BLOCKS};
    Side up_side;
    int new_x;
    int new_y;

    bool found{false};
    for (register int curr_block{0}; curr_block < block_cnt; ++curr_block) {
        for (register Side curr_side{UP}; curr_side <= LEFT;
             curr_side = static_cast<Side>(static_cast<int>(curr_side) + 1)) {
            const int curr_x{blocks[curr_block].x};
            const int curr_y{blocks[curr_block].y};
            if (lookup[rel_pos_diff[curr_side][3][0] + curr_x + MAX_BLOCKS]
                      [rel_pos_diff[curr_side][3][1] + curr_y + MAX_BLOCKS] >=
                0) {
                continue;
            }

            bool match{false};
            register Side match_side{UP};
            for (; match_side <= LEFT; match_side = static_cast<Side>(
                                           static_cast<int>(match_side) + 1)) {
                if (!teeth_match(blocks[curr_block].teeth[curr_side],
                                 teeth[(curr_side + 2 + match_side) % SIDES])) {
                    continue;
                }

                bool should_continue{false};
                for (register int i{0}; i < SIDES - 1; ++i) {
                    int b;
                    if ((b = lookup[rel_pos_diff[curr_side][i][0] + curr_x +
                                    MAX_BLOCKS][rel_pos_diff[curr_side][i][1] +
                                                curr_y + MAX_BLOCKS]) >= 0) {
                        if (!teeth_match(
                                blocks[b].teeth[(curr_side + rel_side_diff[i]) %
                                                SIDES],
                                teeth[(curr_side + rel_side_diff[i] +
                                       match_side + 2) %
                                      SIDES])) {
                            should_continue = true;
                            break;
                        }
                    }
                }
                if (should_continue) {
                    continue;
                }

                found = match = true;
                break;
            }

            if (!match) {
                continue;
            }

            int prod;
            int x;
            int y;
            switch (curr_side) {
            case UP:
                x = blocks[curr_block].x;
                y = blocks[curr_block].y + 1;
                prod = (std::max(max_y, y) - min_y + 1) * (max_x - min_x + 1);
                break;
            case RIGHT:
                x = blocks[curr_block].x + 1;
                y = blocks[curr_block].y;
                prod = (max_y - min_y + 1) * (std::max(max_x, x) - min_x + 1);
                break;
            case DOWN:
                x = blocks[curr_block].x;
                y = blocks[curr_block].y - 1;
                prod = (max_y - std::min(min_y, y) + 1) * (max_x - min_x + 1);
                break;
            case LEFT:
                x = blocks[curr_block].x - 1;
                y = blocks[curr_block].y;
                prod = (max_y - min_y + 1) * (max_x - std::min(min_x, x) + 1);
                break;
            }

            if (prod < min_prod) {
                min_prod = prod;
                up_side = match_side;
                new_x = x;
                new_y = y;
            } else if (prod == min_prod && y > new_y) {
                up_side = match_side;
                new_x = x;
                new_y = y;
            } else if (prod == min_prod && y == new_y && x < new_x) {
                up_side = match_side;
                new_x = x;
            }
        }
    }
    if (!found) {
        return false;
    }

    min_x = std::min(min_x, new_x);
    max_x = std::max(max_x, new_x);
    min_y = std::min(min_y, new_y);
    max_y = std::max(max_y, new_y);

    const int new_block{balloc(new_x, new_y)};
    for (register Side side{UP}; side <= LEFT;
         side = static_cast<Side>(static_cast<int>(side) + 1)) {
        for (register int i{0}; i < width; ++i) {
            blocks[new_block].teeth[side][i] =
                teeth[(side + up_side) % SIDES][i];
        }
    }
    return true;
}

int toothCount(int type)
{
    int cnt{0};
    for (register int curr_block{0}; curr_block < block_cnt; ++curr_block) {
        for (register Side curr_side{UP}; curr_side <= LEFT;
             curr_side = static_cast<Side>(static_cast<int>(curr_side) + 1)) {
            const int curr_x{blocks[curr_block].x};
            const int curr_y{blocks[curr_block].y};
            if (lookup[rel_pos_diff[curr_side][3][0] + curr_x + MAX_BLOCKS]
                      [rel_pos_diff[curr_side][3][1] + curr_y + MAX_BLOCKS] >=
                0) {
                continue;
            }

            for (register int i{0}; i < width; ++i) {
                if (blocks[curr_block].teeth[curr_side][i] == type) {
                    ++cnt;
                }
            }
        }
    }
    return cnt;
}
