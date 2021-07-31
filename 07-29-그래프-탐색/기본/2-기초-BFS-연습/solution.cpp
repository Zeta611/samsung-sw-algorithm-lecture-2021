constexpr int INF{1000};
int queue_x[100], queue_y[100];
int start, end;

int (*map)[10];
int dist[10][10];

inline void enqueue(int x, int y)
{
    queue_x[end] = x;
    queue_y[end++] = y;
}

inline void dequeue(int *x, int *y)
{
    *x = queue_x[start];
    *y = queue_y[start++];
}

inline bool empty() { return start == end; }

void bfs_init(int N, int (*map_)[10]) { map = map_; }

constexpr int dx[]{0, 0, -1, 1};
constexpr int dy[]{-1, 1, 0, 0};

int bfs(int x1, int y1, int x2, int y2)
{
    if (map[y1 - 1][x1 - 1] || map[y2 - 1][x2 - 1]) {
        return -1;
    }

    start = end = 0;
    for (int y{0}; y < 10; ++y) {
        for (int x{0}; x < 10; ++x) {
            // visited[y][x] = false;
            dist[y][x] = INF;
        }
    }

    dist[y1 - 1][x1 - 1] = 0;
    enqueue(x1, y1);
    while (!empty()) {
        int x, y;
        dequeue(&x, &y);
        for (int i{0}; i < 4; ++i) {
            const int nx{x + dx[i]};
            const int ny{y + dy[i]};
            if (nx < 1 || nx > 10 || ny < 1 || ny > 10) {
                continue;
            }
            if (!map[ny - 1][nx - 1] && dist[ny - 1][nx - 1] == INF) {
                dist[ny - 1][nx - 1] = dist[y - 1][x - 1] + 1;
                enqueue(nx, ny);
            }
            if (nx == x2 && ny == y2) {
                return dist[ny - 1][nx - 1];
            }
        }
    }
    return -1;
}
