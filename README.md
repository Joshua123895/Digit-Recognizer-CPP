# Digit-Recognizer-CPP

## ✨ Overview  
This project implements a simple Multi-Layer Perceptron (MLP) in C++ to recognize handwritten digits (0–9) using the MNIST dataset. You can also test the model by drawing a digit directly in the terminal.  

## Features  
- Implemented in C++ (no external ML framework required)  
- Train & test MLP on MNIST data  
- Terminal-based drawing test: allow user to draw a digit and get a prediction  

## Project Structure  
```bash
/ — root directory
├── main.cpp — program entry point
├── neural_network.h / .cpp — MLP implementation
├── train.cpp /.h — training logic
├── mnist_data.h /.cpp — loading MNIST dataset
├── draw_test.cpp /.h — drawing-and-testing interface
├── utils.h /.cpp — helper utilities
├── train_data/ — directory to store MNIST training/testing data
└── MLP.exe — compiled executable (if pre-compiled)
```
## Prerequisites  
- C++ compiler supporting C++11 or newer  
- (Optional) A terminal that supports character-drawing for the draw-test mode  

## Getting Started  

### 1. Prepare dataset  
Download the MNIST dataset (training + test images) and place the files inside `train_data/`.  

### 2. Build the project  
```bash
g++ main.cpp neural_network.cpp mnist_data.cpp train.cpp utils.cpp draw_test.cpp -o DigitRecognizer
```
(or use your preferred build system / IDE)
### 3. Train the network

Run the training executable (e.g., ./DigitRecognizer train) to train the MLP on MNIST data.

### 4. Test the network on MNIST test set

Run the executable in test mode (e.g., ./DigitRecognizer test) to evaluate accuracy on the MNIST test set.

### 5. Draw & recognize a digit (terminal mode)

Run the executable in draw/test mode (e.g., ./DigitRecognizer draw) — draw a digit using keyboard/terminal interface and get the model’s prediction.

Example Usage
# To train:
./DigitRecognizer train

# To test:
./DigitRecognizer test

# To draw and recognize:
./DigitRecognizer draw

## Limitations / Known Issues

 - No GUI — drawing in terminal may be clunky depending on terminal support

 - Accuracy limited by simple MLP architecture (not a CNN) — may misclassify some digits

 - Dataset files must be manually downloaded and placed in train_data/

## Contributing

Feel free to open issues or submit pull requests to improve:

 - support for a better drawing interface or GUI

 - support for more advanced architectures (e.g. convolutional networks)

 - data preprocessing improvements, etc.

License

This project is open-source. Use as you like.