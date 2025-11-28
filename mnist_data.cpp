#include "mnist_data.h"
#include <fstream>
#include <iostream>

void MNISTData::load_data(const char *image_file, const char *label_file) {
    std::ifstream img_file(image_file, std::ios::binary);
    std::ifstream lbl_file(label_file, std::ios::binary);

    if (!img_file || !lbl_file) {
        std::cout << "Error opening file.\n";
        exit(1);
    }

    img_file.seekg(16, std::ios::beg); // Skip the header for images
    lbl_file.seekg(8, std::ios::beg);  // Skip the header for labels

    images.resize(TRAIN_SIZE * IMG_SIZE);
    labels.resize(TRAIN_SIZE);

    img_file.read(reinterpret_cast<char*>(images.data()), TRAIN_SIZE * IMG_SIZE);
    lbl_file.read(reinterpret_cast<char*>(labels.data()), TRAIN_SIZE);

    img_file.close();
    lbl_file.close();
}
