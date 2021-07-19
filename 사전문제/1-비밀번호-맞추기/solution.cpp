#define N 4

typedef struct {
	int hit;
	int miss;
} Result;

// API
extern Result query(int guess[]);

typedef struct {
	int index;	// 0...3, -1 for no diff
	int original;	// 0...9
	int modified;	// 0...9
} Diff;

static bool done_index(const bool (&candidates)[10]) {
	int true_cnt{0};
	for (bool b : candidates) {
		true_cnt += b;
		if (true_cnt > 1) {
			return false;
		}
	}
	return true;
}

static int first_true(const bool *items, int start) {
	for (int i = start; i < 10; ++i) {
		if (items[i]) {
			return i;
		}
	}
	return -1;
}

static int true_count(const bool (&items)[10]) {
	int count{0};
	for (bool b : items) {
		count += b;
	}
	return count;
}

static bool make_guess(int depth, int *guess, const bool (*candidates)[10]) {
	if (depth == 4) {
		return true;
	}

	bool find_more;
	int candidate{-1};
	do {
		find_more = false;
		candidate = first_true(candidates[depth], candidate + 1);
		if (candidate == -1) {
			return false;
		}
		for (int i = 0; i < depth; ++i) {
			if (guess[i] == candidate) {
				find_more = true;
				break;
			}
		}
		if (!find_more) {
			guess[depth] = candidate;
			if (make_guess(depth + 1, guess, candidates)) {
				return true;
			} else {
				find_more = true;
			}
		}
	} while (find_more);
	return false;
}

static Diff alter_guess(const int *guess, const bool (*candidates)[10]) {
	for (int index = 0; index < N; ++index) {
		if (done_index(candidates[index])) {
			continue;
		}

		int original{guess[index]};
		int modified;
		for (modified = 0; modified < 10; ++modified) {
			if (!candidates[index][modified] || modified == original) {
				continue;
			}

			bool duplicate{false};
			for (int j = 0; j < N; ++j) {
				if (index == j) {
					continue;
				}
				if (guess[j] == modified) {
					duplicate = true;
					break;
				}
			}
			if (!duplicate) {
				break;
			}
		}
		if (modified == 10) {
			continue;
		}

		return {index, original, modified};
	}

	return {-1, 0, 0};
}

static int dfs_stack[N];
static bool dfs_visited[N];
static bool dfs_found;

static void init_dfs() {
	dfs_found = false;
	for (bool &b : dfs_visited) {
		b = false;
	}
}

static void visit_dfs(int depth, int size, const int *indices, int *guess, bool (*candidates)[10]) {
	if (depth == size) {
		for (int j = 0; j < size; ++j) {
			if (!candidates[indices[j]][guess[dfs_stack[j]]]) {
				return;
			}
		}

		int copy[N];
		for (int i = 0; i < size; ++i) {
			copy[indices[i]] = guess[indices[i]];
		}
		for (int i = 0; i < size; ++i) {
			guess[indices[i]] = copy[dfs_stack[i]];
		}
		const int hit{query(guess).hit};
		if (hit == 4) {
			dfs_found = true;
			return;
		}
		if (hit == 0) {
			for (int i = 0; i < size; ++i) {
				const int j{indices[i]};
				candidates[j][guess[j]] = false;
			}
		}
		for (int i = 0; i < size; ++i) {
			guess[indices[i]] = copy[indices[i]];
		}
		return;
	}

	for (int i = 0; i < size; ++i) {
		if (dfs_found) {
			return;
		}
		if (dfs_visited[i]) {
			continue;
		}

		dfs_stack[depth] = indices[i];
		dfs_visited[i] = true;
		visit_dfs(depth + 1, size, indices, guess, candidates);
		dfs_visited[i] = false;
	}
}

static void permute_guess_2(int *guess, const bool (*candidates)[10]) {
	// Maximum 3 queries
	int hitmiss[N]{0}; // 0: undetermined, 1: hit, -1: miss
	for (int i = 0; i < N; ++i) {
		if (i == 2 && hitmiss[0] == hitmiss[1]) {
			hitmiss[2] = hitmiss[3] = -hitmiss[0];
			break;
		} else if (i == 3) {
			hitmiss[3] = hitmiss[0] == hitmiss[2] ? hitmiss[1] : hitmiss[0];
			break;
		}

		if (true_count(candidates[i]) == 1) {
			hitmiss[i] = 1;
			continue;
		}
		int cand;
		for (cand = 0; cand < 10; ++cand) {
			bool duplicate{false};
			for (int j = 0; j < N; ++j) {
				if (cand == guess[j]) {
					duplicate = true;
					break;
				}
			}
			if (!duplicate) {
				break;
			}
		}

		const int tmp{guess[i]};
		guess[i] = cand;
		const int hit{query(guess).hit};
		guess[i] = tmp;

		hitmiss[i] = hit < 2 ? 1 : -1;
	}

	int miss1, miss2;
	bool find1{true};
	for (int i = 0; i < N; ++i) {
		if (find1 && hitmiss[i] == -1) {
			miss1 = i;
			find1 = false;
		} else if (!find1 && hitmiss[i] == -1) {
			miss2 = i;
			break;
		}
	}
	const int tmp{guess[miss1]};
	guess[miss1] = guess[miss2];
	guess[miss2] = tmp;
}

static void permute_guess_1(int *guess, bool (*candidates)[10]) {
	// Maximum 8 queries
	int hit_index{-1};
	for (int i = 0; i < N; ++i) {
		if (i == 3) {
			// If a hit was found, i cannot reach 3.
			hit_index = 3;
			break;
		}

		if (true_count(candidates[i]) == 1) {
			hit_index = i;
			break;
		}
		int cand;
		for (cand = 0; cand < 10; ++cand) {
			bool duplicate{false};
			for (int j = 0; j < N; ++j) {
				if (cand == guess[j]) {
					duplicate = true;
					break;
				}
			}
			if (!duplicate) {
				break;
			}
		}

		const int tmp{guess[i]};
		guess[i] = cand;
		const int hit{query(guess).hit};
		guess[i] = tmp;

		if (hit < 1) {
			hit_index = i;
			break;
		}
	}

	int misses[3];
	int j{0};
	for (int i = 0; i < N; ++i) {
		if (i == hit_index) {
			continue;
		}
		misses[j++] = i;
	}
	visit_dfs(0, 3, misses, guess, candidates);
}

static void permute_guess_0(int *guess, bool (*candidates)[10]) {
	int misses[N];
	for (int i = 0; i < N; ++i) {
		misses[i] = i;
	}
	visit_dfs(0, 4, misses, guess, candidates);
}

static void permute_guess(int *guess, bool (*candidates)[10], int hit, int miss) {
	switch (hit) {
	case 2:
		permute_guess_2(guess, candidates);
		return;
	case 1:
		permute_guess_1(guess, candidates);
		return;
	case 0:
		permute_guess_0(guess, candidates);
		return;
	default:
		return;
	}
}

static Result query_diff(int *guess, Diff diff) {
	guess[diff.index] = diff.modified;
	const auto result{query(guess)};
	guess[diff.index] = diff.original;
	return result;
}

static void refine_candidates(int *guess, bool (&candidates)[N][10], int hit, int miss) {
	if (miss == 0) {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				if (i == j) {
					continue;
				}
				candidates[i][guess[j]] = false;
			}
		}
	}
	if (hit == 0) {
		for (int i = 0; i < N; ++i) {
			candidates[i][guess[i]] = false;
		}
	}

	const auto diff{alter_guess(guess, candidates)};
	const auto result_diff{query_diff(guess, diff)};
	const int hit_diff{result_diff.hit};
	const int miss_diff{result_diff.miss};

	if (hit_diff == hit && miss_diff == miss) {
		candidates[diff.index][diff.original] = false;
		candidates[diff.index][diff.modified] = false;
		if (miss == 0) {
			for (auto &c : candidates) {
				c[diff.original] = c[diff.modified] = false;
			}
		}
		if (hit == 3) {
			for (int i = 0; i < N; ++i) {
				if (i == diff.index) {
					continue;
				}
				for (int j = 0; j < 10; ++j) {
					candidates[i][j] = j == guess[i];
				}
			}
		}
	} else if (hit_diff == hit + 1 && miss_diff == miss) {
		for (int i = 0; i < 10; ++i) {
			candidates[diff.index][i] = i == diff.modified;
		}
		for (int i = 0; i < N; ++i) {
			if (i == diff.index) {
				continue;
			}
			candidates[i][diff.original] = false;
			candidates[i][diff.modified] = false;
		}
	} else if (hit_diff == hit - 1 && miss_diff == miss) {
		for (int i = 0; i < 10; ++i) {
			candidates[diff.index][i] = i == diff.original;
		}
		for (int i = 0; i < N; ++i) {
			if (i == diff.index) {
				continue;
			}
			candidates[i][diff.original] = false;
			candidates[i][diff.modified] = false;
		}
	} else if (hit_diff == hit && miss_diff == miss + 1) {
		candidates[diff.index][diff.modified] = false;

		int modified_count = 0;
		int modified_last_index = -1;
		for (int i = 0; i < N; ++i) {
			candidates[i][diff.original] = false;

			if (i != diff.index && candidates[i][diff.modified]) {
				++modified_count;
				modified_last_index = i;
			}
		}

		if (modified_count == 1) {
			for (int i = 0; i < 10; ++i) {
				candidates[modified_last_index][i] = i == diff.modified;
			}
			for (int i = 0; i < N; ++i) {
				if (i == modified_last_index) {
					continue;
				}
				candidates[i][diff.modified] = false;
			}
		}
	} else if (hit_diff == hit && miss_diff == miss - 1) {
		candidates[diff.index][diff.original] = false;

		int original_count{0};
		int original_last_index{-1};
		for (int i = 0; i < N; ++i) {
			candidates[i][diff.modified] = false;

			if (i != diff.index && candidates[i][diff.original]) {
				++original_count;
				original_last_index = i;
			}
		}

		if (original_count == 1) {
			for (int i = 0; i < 10; ++i) {
				candidates[original_last_index][i] = i == diff.original;
			}
			for (int i = 0; i < N; ++i) {
				if (i == original_last_index) {
					continue;
				}
				candidates[i][diff.original] = false;
			}
		}
	} else if (hit_diff == hit + 1 && miss_diff == miss - 1) {
		for (int i = 0; i < 10; ++i) {
			candidates[diff.index][i] = i == diff.modified;
		}

		int original_count{0};
		int original_last_index{-1};
		for (int i = 0; i < N; ++i) {
			if (i == diff.index) {
				continue;
			}
			candidates[i][diff.modified] = false;

			if (candidates[i][diff.original]) {
				++original_count;
				original_last_index = i;
			}
		}

		if (original_count == 1) {
			for (int i = 0; i < 10; ++i) {
				candidates[original_last_index][i] = i == diff.original;
			}
			for (int i = 0; i < N; ++i) {
				if (i == original_last_index) {
					continue;
				}
				candidates[i][diff.original] = false;
			}
		}
	} else if (hit_diff == hit - 1 && miss_diff == miss + 1) {
		for (int i = 0; i < 10; ++i) {
			candidates[diff.index][i] = i == diff.original;
		}

		int modified_count{0};
		int modified_last_index{-1};
		for (int i = 0; i < N; ++i) {
			if (i == diff.index) {
				continue;
			}
			candidates[i][diff.original] = false;

			if (candidates[i][diff.modified]) {
				++modified_count;
				modified_last_index = i;
			}
		}

		if (modified_count == 1) {
			for (int i = 0; i < 10; ++i) {
				candidates[modified_last_index][i] = i == diff.modified;
			}
			for (int i = 0; i < N; ++i) {
				if (i == modified_last_index) {
					continue;
				}
				candidates[i][diff.modified] = false;
			}
		}
	}
}

void doUserImplementation(int guess[]) {
	init_dfs();
	bool candidates[N][10];
	for (auto &row : candidates) {
		for (auto &b : row) {
			b = true;
		}
	}

	while (true) {
		make_guess(0, guess, candidates);
		const auto [hit, miss]{query(guess)};

		// Special cases
		if (hit == N) { // Done
			return;
		}
		if (hit + miss == N) {
			permute_guess(guess, candidates, hit, miss);
			return;
		}
		refine_candidates(guess, candidates, hit, miss);
	}
}
