#ifndef MNIST_DATA_H
#define MNIST_DATA_H

#include <vector>

#define IMG_SIZE 784  // 28x28 pixels
#define TRAIN_SIZE 60000
#define TEST_SIZE 10000

class MNISTData {
public:
    std::vector<unsigned char> images;  // Vector to store images
    std::vector<unsigned char> labels;  // Vector to store labels

    void load_data(const char *image_file, const char *label_file);
};

#endif
