int graph[100][5];

void dfs_init(int N, int path[100][2])
{
    for (int i{1}; i < 100; ++i) {
        for (int j{0}; j < 5; ++j) {
            graph[i][j] = 0;
        }
    }
    for (int i{0}; i < N; ++i) {
        for (int j{0}; j < 5; ++j) {
            if (!graph[path[i][0]][j]) {
                graph[path[i][0]][j] = path[i][1];
                break;
            }
        }
    }
}

int search(int n, int thres)
{
    for (int i{0}; i < 5; ++i) {
        if (!graph[n][i]) {
            break;
        }
        if (graph[n][i] > thres) {
            return graph[n][i];
        }
        int sub{search(graph[n][i], thres)};
        if (sub != -1) {
            return sub;
        }
    }
    return -1;
}

int dfs(int n) { return search(n, n); }
