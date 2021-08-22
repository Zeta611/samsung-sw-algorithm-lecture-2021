#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT (100)
#define CMD_ASSEMBLE (200)
#define CMD_COUNT (300)

#define MAXN (50)

extern void init(int N, int tooth[4][MAXN]);
extern bool assemble(int tooth[4][MAXN]);
extern int toothCount(int mType);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static bool run()
{
    bool correct = false;
    int queryCnt;
    scanf("%d", &queryCnt);

    int tooth[4][MAXN], N;
    char temp[MAXN + 1];

    while (queryCnt--) {
        int cmd;
        scanf("%d", &cmd);

        if (cmd == CMD_INIT) {
            scanf("%d", &N);
            for (int i = 0; i < 4; i++) {
                scanf("%s", temp);
                for (int j = 0; j < N; j++) {
                    tooth[i][j] = temp[j] - '0';
                }
            }

            init(N, tooth);
            correct = true;
        } else if (cmd == CMD_ASSEMBLE) {
            for (int i = 0; i < 4; i++) {
                scanf("%s", temp);
                for (int j = 0; j < N; j++) {
                    tooth[i][j] = temp[j] - '0';
                }
            }

            int ret = -1;
            if (correct) {
                ret = assemble(tooth) ? 1 : 0;
            }

            int ans;
            scanf("%d", &ans);
            if (ret != ans) {
                printf("ASSEMBLE should be %d but was %d\n", ans, ret);
                correct = false;
            }
        } else if (cmd == CMD_COUNT) {
            int mType;
            scanf("%d", &mType);

            int ret = -1;
            if (correct) {
                ret = toothCount(mType);
            }

            int ans;
            scanf("%d", &ans);
            if (ret != ans) {
                printf("COUNT %d should be %d but was %d\n", mType, ans, ret);
                correct = false;
            }
        }
    }
    return correct;
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
