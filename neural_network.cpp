#include "neural_network.h"
#include <vector>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

double NeuralNetwork::activation(double x) {
	return 1.0 / (1.0 + exp(-x));
}

double NeuralNetwork::activation_derivative(double x) {
    return x * (1.0 - x);
}

NeuralNetwork::NeuralNetwork() {
    input.resize(IMG_SIZE);
    hidden.resize(128);  // 128 nodes in hidden layer
    output.resize(NUM_CLASSES);

    weights_ih.resize(IMG_SIZE * 128); // weights between input and hidden
    weights_ho.resize(128 * NUM_CLASSES); // weights between hidden and output

    bias_h.resize(128, 0.0); // bias for hidden
    bias_o.resize(NUM_CLASSES, 0.0); // bias for output

    epoch = 0;
    
    learning_rate = 0.01;
    srand(time(NULL));
    // Initialize weights with small random values
    for (auto &a : weights_ih) a = ((double)rand() / RAND_MAX) * 0.1 - 0.05;
    for (auto &a : weights_ho) a = ((double)rand() / RAND_MAX) * 0.1 - 0.05;
}

void NeuralNetwork::forward(const vector<unsigned char> &input_data) {
    // Normalize the input data
    #pragma omp parallel for shared(input, input_data) default(none)
    for (int i = 0; i < IMG_SIZE; i++) {
        input[i] = (double)input_data[i] / 255.0;
    }

    // Compute hidden 1 layer activations
    #pragma omp parallel for shared(input, weights_ih, bias_h, hidden) default(none)
    for (int i = 0; i < 128; i++) {
        hidden[i] = 0.0;
        for (int j = 0; j < IMG_SIZE; j++) {
            hidden[i] += input[j] * weights_ih[j * 128 + i];
        }
        hidden[i] += bias_h[i];
        hidden[i] = activation(hidden[i]);
    }

    // Compute output layer activations
    #pragma omp parallel for shared(hidden, weights_ho, bias_o, output) default(none)
    for (int i = 0; i < NUM_CLASSES; i++) {
        output[i] = 0.0;
        for (int j = 0; j < 128; j++) {
            output[i] += hidden[j] * weights_ho[j * NUM_CLASSES + i];
        }
        output[i] += bias_o[i];
        output[i] = activation(output[i]);
    }
}

void NeuralNetwork::backpropagate(unsigned char target) {
    vector<double> output_error(NUM_CLASSES);
    vector<double> hidden_error(128);

    // Compute output layer error
    #pragma omp parallel for shared(output_error, output, target) default(none)
    for (int i = 0; i < NUM_CLASSES; i++) {
        output_error[i] = (target == i ? 1.0 : 0.0) - output[i];
    }

    // Compute hidden layer error
    #pragma omp parallel for shared(hidden_error, output_error, weights_ho, hidden) default(none)
    for (int i = 0; i < 128; i++) {
        hidden_error[i] = 0.0;
        for (int j = 0; j < NUM_CLASSES; j++) {
            hidden_error[i] += output_error[j] * weights_ho[i * NUM_CLASSES + j];
        }
        hidden_error[i] *= activation_derivative(hidden[i]);
    }

    // Update weights and biases for output layer
    #pragma omp parallel for shared(weights_ho, output_error, hidden, bias_o, learning_rate) default(none)
    for (int i = 0; i < NUM_CLASSES; i++) {
        for (int j = 0; j < 128; j++) {
            weights_ho[j * NUM_CLASSES + i] += learning_rate * output_error[i] * hidden[j];
        }
        bias_o[i] += learning_rate * output_error[i];
    }

    // Update weights and biases for hidden layer
    #pragma omp parallel for shared(weights_ih, hidden_error, input, bias_h, learning_rate) default(none)
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < IMG_SIZE; j++) {
            weights_ih[j * 128 + i] += learning_rate * hidden_error[i] * input[j];
        }
        bias_h[i] += learning_rate * hidden_error[i];
    }
}

void NeuralNetwork::load(const string &filename) {
    ifstream loadFile(filename, ios::binary);
    if (!loadFile) {
        cout << "Error opening file\n";
        return;
    }

    loadFile.read(reinterpret_cast<char*>(weights_ih.data()), IMG_SIZE * 128 * sizeof(double));
    loadFile.read(reinterpret_cast<char*>(weights_ho.data()), 128 * NUM_CLASSES * sizeof(double));
    loadFile.read(reinterpret_cast<char*>(bias_h.data()), 128 * sizeof(double));
    loadFile.read(reinterpret_cast<char*>(bias_o.data()), NUM_CLASSES * sizeof(double));
    loadFile.read(reinterpret_cast<char*>(&epoch), sizeof(int));

    cout << "Loaded successfully\n";
}

void NeuralNetwork::save(const string &filename) {
    ofstream saveFile(filename, ios::binary);
    saveFile.write(reinterpret_cast<char*>(weights_ih.data()), IMG_SIZE * 128 * sizeof(double));
    saveFile.write(reinterpret_cast<char*>(weights_ho.data()), 128 * NUM_CLASSES * sizeof(double));
    saveFile.write(reinterpret_cast<char*>(bias_h.data()), 128 * sizeof(double));
    saveFile.write(reinterpret_cast<char*>(bias_o.data()), NUM_CLASSES * sizeof(double));
    saveFile.write(reinterpret_cast<char*>(&epoch), sizeof(int));
}
