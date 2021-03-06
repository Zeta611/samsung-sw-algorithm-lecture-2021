#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define INPUTWORD (100)
#define RECOMMEND (200)
#define BANWORD (300)

extern void init();
extern void inputWord(char mWord[20]);
extern int recommend(char mUser[20], char mAnswer[20]);
extern void banWord(char mWord[20]);

static int mstrcmp(const char str1[], const char str2[])
{
    int c = 0;
    while (str1[c] != 0 && str1[c] == str2[c])
        ++c;
    return str1[c] - str2[c];
}

static int mstrlen(const char str[])
{
    int ret = 0;
    while (str[ret])
        ++ret;
    return ret;
}

static int run()
{
    int ret_val = 1;
    int type, len;
    char mWord[20] = {}, mAnswer[20] = {}, ans[20] = {};

    init();

    int N;
    scanf("%d", &N);

    for (int i = 0; i < N; ++i) {
        scanf("%d %s", &type, mWord);

        switch (type) {
        case INPUTWORD:
            inputWord(mWord);
            break;
        case RECOMMEND:
            len = recommend(mWord, mAnswer);
            scanf("%s", ans);

            if (len != mstrlen(ans)) {
                printf("%d: RECOMMEND %s len should be %d not %d\n", i, mWord,
                       mstrlen(ans), len);
                ret_val = 0;
            }
            if (mstrcmp(ans, mAnswer) != 0) {
                printf("%d: RECOMMEND %s should be %s not %s\n", i, mWord, ans,
                       mAnswer);
                ret_val = 0;
            }

            break;
        case BANWORD:
            banWord(mWord);
            break;
        default:
            ret_val = 0;
            break;
        }
    }

    return ret_val;
}

int main()
{
    setbuf(stdout, NULL);
    // freopen("sample_input.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++) {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    return 0;
}
