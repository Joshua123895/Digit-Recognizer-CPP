#include "mnist_data.h"
#include "neural_network.h"
#include "train.h"
#include "draw_test.h"
#include "utils.h"
#include <omp.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>


using namespace std;

void enter(string str) {
	cout << str;
	cin.ignore();
	cin.get();
}

void resetNN(NeuralNetwork &nn) {
	int confirm;
	cout << "are you sure[1/0]? ";
	cin >> confirm;
	if (confirm) {
    	nn = NeuralNetwork();  // Reset neural network
		enter("Reseted succesfully");
	}
}

void loadNN(NeuralNetwork &nn) {
	string nameFile;
	cout << "load file.dat as: ";
	cin >> nameFile;
	nameFile = "train_data/" + nameFile + ".dat";
	nn.load(nameFile);
	enter("");
}

int main(int argc, char *argv[]) {
    #ifdef _OPENMP
        cout << "OpenMP is enabled! Version: " << _OPENMP << endl;
        omp_set_num_threads(6);
    #else
        cout << "OpenMP is NOT enabled!" << endl;
    #endif
    cin.get();
    // Load MNIST data
    MNISTData train_data, test_data;
    train_data.load_data("../train-images.idx3-ubyte", "../train-labels.idx1-ubyte");
    test_data.load_data("../t10k-images.idx3-ubyte", "../t10k-labels.idx1-ubyte");

    // Initialize neural network
    NeuralNetwork nn;

    // Menu loop
    int menu;
    do {
        system("cls");
        puts("MLP ");
        puts("1. reset progress");
        puts("2. load .dat file");
        puts("3. train");
        puts("4. test");
        puts("5. draw and test");
        puts("6. save and exit");
        printf(">> ");
        scanf("%d", &menu); getchar();
        switch (menu) {
            case 1:
            	resetNN(nn);
                break;
            case 2:
            	loadNN(nn);
                break;
            case 3:
                train(nn, train_data);
                break;
            case 4:
                test(nn, test_data);
                break;
            case 5:
                drawTest(nn);
                break;
        }
    } while (menu != 6);

	string nameFile;
	cout << "save file as: ";
	cin >> nameFile;
	nameFile = "train_data/" + nameFile + ".dat";
    nn.save(nameFile);

    return 0;
}
