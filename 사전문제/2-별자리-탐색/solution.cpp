#define MAX_N 1000
#define MAX_M 20
#define MAX_C 20
#define MAX_S 20000

struct Result {
	int y, x;
};

typedef struct {
	int x, y;
} Point;

static int n, m, (*map)[MAX_N];
static int star_cnt;
static Point all_stars[MAX_S];

void init(int N, int M, int Map[MAX_N][MAX_N])
{

	n = N;
	m = M;
	map = Map;
	star_cnt = 0;
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < n; ++x) {
			if (map[y][x]) {
				all_stars[star_cnt].x = x;
				all_stars[star_cnt].y = y;
				++star_cnt;
			}
		}
	}
}

void rotate90(Point *points, int size)
{
	for (int i = 0; i < size; ++i) {
		const int tmp = points[i].x;
		points[i].x = points[i].y;
		points[i].y = m - tmp - 1;
	}
}

Point search(Point *constellation, int size)
{
	const int cx = constellation[0].x;
	const int cy = constellation[0].y;
	for (int i = 0; i < star_cnt; ++i) {
		const int dx = all_stars[i].x - cx;
		const int dy = all_stars[i].y - cy;
		int j;
		for (j = 1; j < size; ++j) {
			const int x = dx + constellation[j].x;
			const int y = dy + constellation[j].y;
			if (x < 0 || y < 0 || x >= n || y >= n || !map[y][x]) {
				break;
			}
		}
		if (j == size) {
			Point offset = {dx, dy};
			return offset;
		}
	}
	Point none = {-1, -1};
	return  none;
}

Result findConstellation(int stars[MAX_M][MAX_M])
{
	Point constellation[MAX_C];
	int cnt = 0;
	int pivot;
	for (int y = 0; y < m; ++y) {
		for (int x = 0; x < m; ++x) {
			if (stars[y][x]) {
				constellation[cnt].x = x;
				constellation[cnt].y = y;
				if (stars[y][x] == 9) {
					pivot = cnt;
				}
				++cnt;
			}
		}
	}

	Result res = {-1, -1};
	for (int i = 0; i < 4; ++i) {
		Point offset = search(constellation, cnt);
		if (offset.x >= 0 && offset.y >= 0) {
			res.x = offset.x + constellation[pivot].x;
			res.y = offset.y + constellation[pivot].y;
			break;
		}
		rotate90(constellation, cnt);
	}
	return res;
}
