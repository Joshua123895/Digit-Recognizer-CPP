#ifndef TRAIN_H
#define TRAIN_H

#include "mnist_data.h"
#include "neural_network.h"

// Function to train the network
void train(NeuralNetwork &nn, MNISTData &train_data);

#endif
