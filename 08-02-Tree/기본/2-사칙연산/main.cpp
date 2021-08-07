#include <iostream>

constexpr int N{1000};
int arr[N + 1], left[N + 1], right[N + 1];

enum Op {
    PLUS = '+' - '0',
    MINUS = '-' - '0',
    TIMES = '*' - '0',
    DIV = '/' - '0'
};

double eval(int n = 1)
{
    if (arr[n] > 0) {
        return arr[n];
    }
    switch (arr[n]) {
    case PLUS:
        return eval(left[n]) + eval(right[n]);
    case MINUS:
        return eval(left[n]) - eval(right[n]);
    case TIMES:
        return eval(left[n]) * eval(right[n]);
    case DIV:
        return eval(left[n]) / eval(right[n]);
    default: // Error
        return 0;
    }
}

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    constexpr int T{10};
    char buf[10];
    for (int test_case{1}; test_case <= T; ++test_case) {
        int n;
        std::cin >> n;
        for (int i{0}; i < n; ++i) {
            int idx, l, r;
            std::cin >> idx >> buf;
            if (buf[0] > '0') {
                arr[idx] = std::atoi(buf);
            } else {
                arr[idx] = buf[0] - '0';
                std::cin >> l >> r;
                left[idx] = l;
                right[idx] = r;
            }
        }
        std::cout << '#' << test_case << ' ' << static_cast<long>(eval())
                  << '\n';
    }
    return 0;
}
