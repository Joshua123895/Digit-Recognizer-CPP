#ifndef DRAW_TEST_H
#define DRAW_TEST_H

void draw_grid(unsigned char *grid);
void test(NeuralNetwork &nn, MNISTData &test_data);
void get_mouse_position(int* x, int* y);
int is_mouse_clicked();
int map_click_to_grid(int mouseX, int mouseY, int x1, int y1, int x2, int y2);
void paint_grid(unsigned char *grid, int idx);
void drawTest(NeuralNetwork &nn);

#endif