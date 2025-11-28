#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include <string>

#define IMG_SIZE 784  // 28x28 pixels
#define NUM_CLASSES 10 // Digits 0-9
#define PROGRESS_LENGTH 50

using namespace std;

class NeuralNetwork {
public:
    vector<double> input;    // Input layer 784 (28*28)
    vector<double> hidden;   // Hidden layer 128
    vector<double> output;   // Output layer 10
    vector<double> weights_ih;  // Weights between input and hidden
    vector<double> weights_ho;  // Weights between hidden and output
    vector<double> bias_h;  // Bias for hidden layer
    vector<double> bias_o;  // Bias for output layer
    int epoch;
    double learning_rate;

    NeuralNetwork();
    void forward(const vector<unsigned char> &input_data);
    void backpropagate(unsigned char target);
    void load(const string &filename);
    void save(const string &filename);
    double activation(double x);
    double activation_derivative(double x);
};

#endif
