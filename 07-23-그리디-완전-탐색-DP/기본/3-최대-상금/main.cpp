#include <iostream>

long backtrack(int pos, int *digits, int cnt, int swap, bool dup)
{
    if (!pos || !swap) {
        if (swap % 2 && !dup) {
            const int d{digits[0]};
            digits[0] = digits[1];
            digits[1] = d;
        }
        long sum{0};
        for (int i{cnt - 1}; i >= 0; --i) {
            sum *= 10;
            sum += digits[i];
        }
        return sum;
    }

    long max{0};
    for (int i{0}; i < pos; ++i) {
        if (digits[pos] < digits[i]) {
            const int d{digits[i]};
            digits[i] = digits[pos];
            digits[pos] = d;

            const long m{backtrack(pos - 1, digits, cnt, swap - 1, dup)};
            if (max < m) {
                max = m;
            }

            digits[pos] = digits[i];
            digits[i] = d;
        }
    }
    return max ? max : backtrack(pos - 1, digits, cnt, swap, dup);
}

long price(long number, int swap)
{
    int digits[6];
    int pos{0};

    int found{0};
    bool dup{false};

    while (number) {
        const int d{static_cast<int>(number % 10)};
        number /= 10;
        digits[pos++] = d;

        if (!dup) {
            const int flag{1 << d};
            if (found & flag) {
                dup = true;
            } else {
                found |= flag;
            }
        }
    }

    return backtrack(pos - 1, digits, pos, swap, dup);
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;
    for (int test_case{1}; test_case <= T; ++test_case) {
        long number;
        int swap;
        std::cin >> number >> swap;
        std::cout << '#' << test_case << ' ' << price(number, swap) << '\n';
    }
    return 0;
}
