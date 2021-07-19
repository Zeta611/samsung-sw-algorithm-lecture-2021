#define MAXR	99
#define MAXC	26
#define NULL	0

typedef enum {
	ADD, SUB, MUL, DIV, MAX, MIN, SUM, CONST
} Op;
typedef struct {
	int col, row;
} Addr;
typedef struct {
	Op op;
	Addr addr1;
	Addr addr2;
} Fn;
typedef struct {
	Fn fn;
	int result;
} Cell;

static Cell table[MAXR][MAXC];
static int COL, ROW;

static Addr deps[MAXR][MAXC][MAXR];
static int deps_size[MAXR][MAXC];
static bool visited[MAXR][MAXC];
static Addr stack[MAXR * MAXC];
static int stack_size;

void init(int C, int R) {
	COL = C;
	ROW = R;
	for (int r = 0; r < ROW; ++r) {
		for (int c = 0; c < COL; ++c) {
			table[r][c].fn.op = CONST;
			table[r][c].result = 0;
			deps_size[r][c] = 0;
		}
	}
}

static int readnum(const char *str, const char **ptr) {
	const char *p = str;
	bool negative = false;
	if (*p == '-') {
		++p;
		negative = true;
	}
	if (*p >= '0' && *p <= '9') {
		int val = 0;
		while (*p >= '0' && *p <= '9') {
			val *= 10;
			val += *p++ - '0';
		}
		if (ptr) {
			*ptr = p;
		}
		return negative ? -val: val;
	}
	return 0;
}

static void set_addr(int col, int row, int col1, int row1, int col2, int row2, bool isrange) {
	table[row][col].fn.addr1.col = col1;
	table[row][col].fn.addr1.row = row1;
	table[row][col].fn.addr2.col = col2;
	table[row][col].fn.addr2.row = row2;

	if (isrange) {
		if (col1 == col2) {
			int lim = row2 - row1 + 1;
			deps_size[row][col] = lim;
			for (int i = 0; i < lim; ++i) {
				deps[row][col][i].col = col1;
				deps[row][col][i].row = row1 + i;
			}
		} else {
			int lim = col2 - col1 + 1;
			deps_size[row][col] = lim;
			for (int i = 0; i < lim; ++i) {
				deps[row][col][i].col = col1 + i;
				deps[row][col][i].row = row1;
			}
		}
	} else {
		deps[row][col][0] = table[row][col].fn.addr1;
		deps[row][col][1] = table[row][col].fn.addr2;
		deps_size[row][col] = 2;
	}
}

void set(int col, int row, char input[]) {
	col -= 1;
	row -= 1;
	if (*input == '-' || *input >= '0' && *input <= '9') {
		table[row][col].fn.op = CONST;
		table[row][col].result = readnum(input, NULL);
	} else {
		bool isrange = false;
		switch (input[2]) {
		case 'D':
			table[row][col].fn.op = ADD;
			break;     
		case 'B':             
			table[row][col].fn.op = SUB;
			break;     
		case 'L':             
			table[row][col].fn.op = MUL;
			break;     
		case 'V':             
			table[row][col].fn.op = DIV;
			break;     
		case 'X':             
			table[row][col].fn.op = MAX;
			isrange = true;
			break;
		case 'N':
			table[row][col].fn.op = MIN;
			isrange = true;
			break;
		case 'M':
			table[row][col].fn.op = SUM;
			isrange = true;
			break;
		default:
			break;
		}

		const char *ptr = input + 4;
		int col1 = *ptr++ - 'A';
		int row1 = readnum(ptr, &ptr) - 1;
		int col2 = *(++ptr)++ - 'A';
		int row2 = readnum(ptr, NULL) - 1;
		set_addr(col, row, col1, row1, col2, row2, isrange);
	}
}

Addr next_addr(int col, int row) {
	Addr addr = {col, row};
	if (col == COL - 1 && row == ROW - 1) {
		addr.col = addr.row = -1;
	} else if (row == ROW - 1) {
		addr.row = 0;
		++addr.col;
	} else {
		++addr.row;
	}
	return addr;
}

static void topological_sort(int col, int row) {
	visited[row][col] = true;
	for (int i = 0; i < deps_size[row][col]; ++i) {
		Addr dep = deps[row][col][i];
		if (!visited[dep.row][dep.col]) {
			topological_sort(dep.col, dep.row);
		}
	}
	Addr addr = {col, row};
	stack[stack_size++] = addr;
}

static inline int min(int a, int b) {
	return a < b ? a : b;
}

static inline int max(int a, int b) {
	return a < b ? b : a;
}

static void evaluate(int col, int row) {
	int col1 = table[row][col].fn.addr1.col;
	int row1 = table[row][col].fn.addr1.row;
	int col2 = table[row][col].fn.addr2.col;
	int row2 = table[row][col].fn.addr2.row;

	int res1 = table[row1][col1].result;
	int res2 = table[row2][col2].result;

	int acc;
	switch (table[row][col].fn.op) {
	case ADD:             
		table[row][col].result = res1 + res2;
		break;     
	case SUB:             
		table[row][col].result = res1 - res2;
		break;     
	case MUL:             
		table[row][col].result = res1 * res2;
		break;     
	case DIV:             
		table[row][col].result = res1 / res2;
		break;
	case MAX:
		acc = res1;
		if (col1 == col2) {
			for (int r = row1; r <= row2; ++r) {
				acc = max(acc, table[r][col1].result);
			}
		} else {
			for (int c = col1; c <= col2; ++c) {
				acc = max(acc, table[row1][c].result);
			}
		}
		table[row][col].result = acc;
		break;
	case MIN:
		acc = res1;
		if (col1 == col2) {
			for (int r = row1; r <= row2; ++r) {
				acc = min(acc, table[r][col1].result);
			}
		} else {
			for (int c = col1; c <= col2; ++c) {
				acc = min(acc, table[row1][c].result);
			}
		}
		table[row][col].result = acc;
		break;
	case SUM:
		acc = 0;
		if (col1 == col2) {
			for (int r = row1; r <= row2; ++r) {
				acc += table[r][col1].result;
			}
		} else {
			for (int c = col1; c <= col2; ++c) {
				acc += table[row1][c].result;
			}
		}
		table[row][col].result = acc;
		break;
	default:
		break;
	}
}

void update(int value[MAXR][MAXC]) {
	stack_size = 0;
	for (int r = 0; r < ROW; ++r) {
		for (int c = 0; c < COL; ++c) {
			visited[r][c] = false;
		}
	}
	for (int r = 0; r < ROW; ++r) {
		for (int c = 0; c < COL; ++c) {
			if (!visited[r][c]) {
				topological_sort(c, r);
			}
		}
	}
	for (int i = 0; i < stack_size; ++i) {
		int col = stack[i].col;
		int row = stack[i].row;
		if (deps_size[row][col] == 0) {
			continue;
		}
		evaluate(col, row);
	}
	for (int r = 0; r < ROW; ++r) {
		for (int c = 0; c < COL; ++c) {
			value[r][c] = table[r][c].result;
		}
	}
}
