#include <stdio.h>
#include <unistd.h> // for usleep() function

void progressBar(int progress, int total) {
    const int barWidth = 70;
    float progressRatio = (float)progress / (float)total;
    int barLength = progressRatio * barWidth;

    printf("[");
    for (int i = 0; i < barWidth; ++i) {
        if (i < barLength) {
            printf("=");
        } else {
            printf(" ");
        }
    }
    printf("] %3.0f%%\r", progressRatio * 100.0);
    fflush(stdout);
}

int main() {
    const int totalSteps = 10;

    for (int i = 0; i <= totalSteps; ++i) {
        progressBar(i, totalSteps);
        // 何らかの処理をする（ここでは usleep を使って待機する例）
        usleep(200000); // 200ms待機（0.2秒）
    }
    
    printf("\n処理が完了しました。\n");

    return 0;
}
