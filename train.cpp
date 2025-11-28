#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "train.h"
#include "utils.h"
#include "mnist_data.h"
#include "neural_network.h"

int countDigits(int a) {
	if (a == 0) return 0;
	return static_cast<int> (log10(abs(a))) + 1;
}

void train(NeuralNetwork &nn, MNISTData &train_data) {
    double initial_lr = 0.01;
    double decay_rate = 0.1;
    printf("How many epochs? ");
    int EPOCHS;
    scanf("%d", &EPOCHS); getchar();
    system("cls");
    time_t start, end;
    for (int e = 0; e < EPOCHS; e++) {
        start = clock();
        // learning rate decay
        nn.learning_rate = initial_lr * exp(-decay_rate * nn.epoch);
        // decoration progress bar
        printf("epoch %d/%d: --------------------------------------------------   0%%", e + 1, EPOCHS);
        fflush(stdout);
        int percent = 0;
        int offset = countDigits(e+1) + countDigits(EPOCHS);
        for (int i = 0; i < TRAIN_SIZE; i++) {
            // progress bar too
            int currentPercent = static_cast<int>((i+1) * 100.0 / TRAIN_SIZE);
            if (currentPercent > percent) {
                percent = currentPercent;
                int cursorPos = currentPercent / 100.0 * PROGRESS_LENGTH - (percent > 50);
                moveCursorTo(10 + cursorPos + offset, e+1);
                printf("#");
                moveCursorTo(11 + PROGRESS_LENGTH + offset, e+1);
                printf("%3d%%", percent);
                fflush(stdout);
            }

            // real training
            nn.forward({train_data.images.begin() + i * IMG_SIZE, train_data.images.begin() + (i + 1) * IMG_SIZE});
            nn.backpropagate(train_data.labels[i]);
        }
        end = clock();
        printf(" time: %.2lf s\n", ((double) (end - start)) / CLOCKS_PER_SEC);
        nn.epoch++;
    }
    printf("done training!");
    getchar();
}