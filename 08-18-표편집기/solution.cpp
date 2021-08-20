#define MAX_WIDTH 1000
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SWAP(a, b, tmp) ((tmp) = (a), (a) = (b), (b) = (tmp))
#define INSIDE(r, c, top, left, bottom, right)                                 \
    ((r) >= (top) && (r) <= (bottom) && (c) >= (left) && (c) <= (right))

int top[2][MAX_WIDTH + 1][MAX_WIDTH + 1];
int left[2][MAX_WIDTH + 1][MAX_WIDTH + 1];
int bottom[2][MAX_WIDTH + 1][MAX_WIDTH + 1];
int right[2][MAX_WIDTH + 1][MAX_WIDTH + 1];

int curr;
int mod_top;
int mod_left;
int mod_bottom;
int mod_right;

void init(int R, int C)
{
    for (register int r{1}; r <= R; ++r) {
        for (register int c{1}; c <= C; ++c) {
            top[0][r][c] = bottom[0][r][c] = top[1][r][c] = bottom[1][r][c] = r;
            left[0][r][c] = right[0][r][c] = left[1][r][c] = right[1][r][c] = c;
        }
    }

    curr = mod_top = 0;
}

void getRect(int r, int c, int *rect)
{
    rect[0] = top[curr][r][c];
    rect[1] = left[curr][r][c];
    rect[2] = bottom[curr][r][c];
    rect[3] = right[curr][r][c];
}

bool check_dup[MAX_WIDTH + 1][MAX_WIDTH + 1];
int mergeCells(int cnt, int *rs, int *cs, int *rect)
{
    int min_top{MAX_WIDTH};
    int min_left{MAX_WIDTH};
    int max_bottom{1};
    int max_right{1};
    int area{0};

    for (int i{0}; i < cnt; ++i) {
        const int r{rs[i]};
        const int c{cs[i]};

        const int tp{top[curr][r][c]};
        const int lt{left[curr][r][c]};
        const int bm{bottom[curr][r][c]};
        const int rt{right[curr][r][c]};

        if (check_dup[tp][lt]) {
            while (i--) {
                check_dup[top[curr][rs[i]][cs[i]]][left[curr][rs[i]][cs[i]]] =
                    false;
            }
            return 0;
        }
        check_dup[tp][lt] = true;

        min_top = MIN(min_top, tp);
        min_left = MIN(min_left, lt);
        max_bottom = MAX(max_bottom, bm);
        max_right = MAX(max_right, rt);
        area += (bm - tp + 1) * (rt - lt + 1);
    }

    while (cnt--) {
        check_dup[top[curr][rs[cnt]][cs[cnt]]][left[curr][rs[cnt]][cs[cnt]]] =
            false;
    }

    if (area != (max_bottom - min_top + 1) * (max_right - min_left + 1)) {
        return 0;
    }

    if (mod_top) {
        for (register int row{mod_top}; row <= mod_bottom; ++row) {
            for (register int col{mod_left}; col <= mod_right; ++col) {
                top[curr ^ 1][row][col] = top[curr][row][col];
                left[curr ^ 1][row][col] = left[curr][row][col];
                bottom[curr ^ 1][row][col] = bottom[curr][row][col];
                right[curr ^ 1][row][col] = right[curr][row][col];
            }
        }
    }

    mod_top = min_top;
    mod_left = min_left;
    mod_bottom = max_bottom;
    mod_right = max_right;
    for (register int row{min_top}; row <= max_bottom; ++row) {
        for (register int col{min_left}; col <= max_right; ++col) {
            top[curr][row][col] = min_top;
            left[curr][row][col] = min_left;
            bottom[curr][row][col] = max_bottom;
            right[curr][row][col] = max_right;
        }
    }

    rect[0] = min_top;
    rect[1] = min_left;
    rect[2] = max_bottom;
    rect[3] = max_right;
    return 1;
}

int splitCell(int r, int c, int *rect)
{
    const int min_top{top[curr][r][c]};
    const int min_left{left[curr][r][c]};
    const int max_bottom{bottom[curr][r][c]};
    const int max_right{right[curr][r][c]};

    if (min_top == max_bottom && min_left == max_right) {
        return 0;
    }

    if (mod_top) {
        for (register int row{mod_top}; row <= mod_bottom; ++row) {
            for (register int col{mod_left}; col <= mod_right; ++col) {
                top[curr ^ 1][row][col] = top[curr][row][col];
                left[curr ^ 1][row][col] = left[curr][row][col];
                bottom[curr ^ 1][row][col] = bottom[curr][row][col];
                right[curr ^ 1][row][col] = right[curr][row][col];
            }
        }
    }

    mod_top = min_top;
    mod_left = min_left;
    mod_bottom = max_bottom;
    mod_right = max_right;
    for (register int row{min_top}; row <= max_bottom; ++row) {
        for (register int col{min_left}; col <= max_right; ++col) {
            top[curr][row][col] = bottom[curr][row][col] = row;
            left[curr][row][col] = right[curr][row][col] = col;
        }
    }

    rect[0] = min_top;
    rect[1] = min_left;
    rect[2] = max_bottom;
    rect[3] = max_right;
    return 1;
}

void undo()
{
    if (!mod_top) {
        return;
    }
    curr ^= 1;
}

int checkRectangle(int r1, int c1, int r2, int c2)
{
    if (r1 == r2 && c1 == c2) {
        return INSIDE(top[curr][r1][c1], left[curr][r1][c1], r1, c1, r2, c2) &&
               INSIDE(bottom[curr][r1][c1], right[curr][r1][c1], r1, c1, r2,
                      c2);
    }

    register int row, col;

    if (r1 == r2) {
        col = c1;
        while (col <= c2) {
            if (!INSIDE(top[curr][r1][col], left[curr][r1][col], r1, c1, r2,
                        c2) ||
                !INSIDE(bottom[curr][r1][col], right[curr][r1][col], r1, c1, r2,
                        c2)) {
                return 1;
            }
            col = right[curr][r1][col] + 1;
        }
        return 0;
    }

    if (c1 == c2) {
        row = r1;
        while (row <= r2) {
            if (!INSIDE(top[curr][row][c1], left[curr][row][c1], r1, c1, r2,
                        c2) ||
                !INSIDE(bottom[curr][row][c1], right[curr][row][c1], r1, c1, r2,
                        c2)) {
                return 1;
            }
            row = bottom[curr][row][c1] + 1;
        }
        return 0;
    }

    col = c1;
    while (col < c2) {
        if (!INSIDE(top[curr][r1][col], left[curr][r1][col], r1, c1, r2, c2) ||
            !INSIDE(bottom[curr][r1][col], right[curr][r1][col], r1, c1, r2,
                    c2)) {
            return 1;
        }
        col = right[curr][r1][col] + 1;
    }
    row = r1;
    while (row < r2) {
        if (!INSIDE(top[curr][row][c2], left[curr][row][c2], r1, c1, r2, c2) ||
            !INSIDE(bottom[curr][row][c2], right[curr][row][c2], r1, c1, r2,
                    c2)) {
            return 1;
        }
        row = bottom[curr][row][c2] + 1;
    }
    col = c1 + 1;
    while (col <= c2) {
        if (!INSIDE(top[curr][r2][col], left[curr][r2][col], r1, c1, r2, c2) ||
            !INSIDE(bottom[curr][r2][col], right[curr][r2][col], r1, c1, r2,
                    c2)) {
            return 1;
        }
        col = right[curr][r2][col] + 1;
    }
    row = r1 + 1;
    while (row <= r2) {
        if (!INSIDE(top[curr][row][c1], left[curr][row][c1], r1, c1, r2, c2) ||
            !INSIDE(bottom[curr][row][c1], right[curr][row][c1], r1, c1, r2,
                    c2)) {
            return 1;
        }
        row = bottom[curr][row][c1] + 1;
    }
    return 0;
}
