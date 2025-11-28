#include <stdio.h>
#include "utils.h"
#include <windows.h>
#include <vector>
#include "neural_network.h"
#include "mnist_data.h"
#include "draw_test.h"

void draw_grid(unsigned char *grid) {
	int len_shade = 24;
	puts(" +--------------------------------------------------------+");
	for (int i = 0; i < 28; i++) {
		printf(" |");
		for (int j = 0; j < 28; j++) {
			int bit = (int) (grid[i*28 + j] * len_shade / 255.0);
			bit -= bit / len_shade; // wont go >= len, not out of bound
			change_colour(bit);
			printf("  ");
		}
		reset_colour();
		puts("|");
	}
	puts(" +--------------------------------------------------------+");
}


// Test function
void test(NeuralNetwork &nn, MNISTData &test_data) {
    system("cls");
    printf("Test accuracy: Loading...");
    int correct = 0;
    for (int i = 0; i < TEST_SIZE; i++) {
        nn.forward({test_data.images.begin() + i * IMG_SIZE, test_data.images.begin() + (i + 1) * IMG_SIZE});
        int predicted = 0;
        double max_output = nn.output[0];
        for (int j = 1; j < NUM_CLASSES; j++) {
            if (nn.output[j] > max_output) {
                max_output = nn.output[j];
                predicted = j;
            }
        }

        if (predicted == test_data.labels[i]) {
            correct++;
        }
    }
    system("cls");
    printf("Test accuracy: %.2f%%\n", (double) correct / TEST_SIZE * 100);
    getchar();
    int index;
    do {
    	system("cls");
    	printf("Test from 0 to 9.999 [-1 to exit]: ");
    	scanf("%d", &index);getchar();
    	if (index < 0 || index > 9999) break;
    	draw_grid(&test_data.images[index * IMG_SIZE]);
        nn.forward({test_data.images.begin() + index * IMG_SIZE, test_data.images.begin() + (index + 1) * IMG_SIZE});
        int predicted = 0;
        double max_output = nn.output[0];
        for (int j = 1; j < NUM_CLASSES; j++) {
            if (nn.output[j] > max_output) {
                max_output = nn.output[j];
                predicted = j;
            }
        }
        printf("Expected: %d, predicted: %d\n", test_data.labels[index], predicted);
    	getchar();
	} while (1);
}

void get_mouse_position(int* x, int* y) {
    // Get the mouse cursor position on the screen
    POINT p;
    if (GetCursorPos(&p)) {
        *x = p.x;
        *y = p.y;
    }
}

int is_mouse_clicked() {
    // Check if the left mouse button is pressed
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
}

int map_click_to_grid(int mouseX, int mouseY, int x1, int y1, int x2, int y2) {
    // Map the mouse click position to a grid index within the rectangle
    if (mouseX < x1) mouseX = x1;
    if (mouseX >= x2) mouseX = x2-1;
    if (mouseY < y1) mouseY = y1;
    if (mouseY >= y2) mouseY = y2-1;
    int gridX = (mouseX - x1) * 28 / (x2 - x1);
    int gridY = (mouseY - y1) * 28 / (y2 - y1);
	int index = gridY*28 + gridX;
    // Ensure gridX and gridY are within bounds
    return index;
}

void paint_grid(unsigned char *grid, int idx) {
	static int index = -1;
	if (index == idx) return;
	index = idx;
    // Ensure grid[index] does not exceed 254 before adding
    grid[index] = std::min(grid[index] + 100, UCHAR_MAX);
    
    int left = (index % 28 > 0);
    int right = (index % 28 < 27);
    int up = (index > 27);
    int down = (index < (28 * 28 - 28));

    if (left) grid[index - 1] = std::min(grid[index - 1] + 80, UCHAR_MAX);
    if (right) grid[index + 1] = std::min(grid[index + 1] + 80, UCHAR_MAX);
    if (up) grid[index - 28] = std::min(grid[index - 28] + 80, UCHAR_MAX);
    if (down) grid[index + 28] = std::min(grid[index + 28] + 80, UCHAR_MAX);

    if (left && up) grid[index - 1 - 28] = std::min(grid[index - 1 - 28] + 40, UCHAR_MAX);
    if (right && up) grid[index + 1 - 28] = std::min(grid[index + 1 - 28] + 40, UCHAR_MAX);
    if (right && down) grid[index + 1 + 28] = std::min(grid[index + 1 + 28] + 40, UCHAR_MAX);
    if (left && down) grid[index - 1 + 28] = std::min(grid[index - 1 + 28] + 40, UCHAR_MAX);
}

void hide_cursor() {
	printf("\e[?25l");
}

void show_cursor() {
	printf("\e[?25h");
}

void enableMouseOnly() {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hInput, &mode);

    // Disable Quick Edit Mode but keep Mouse Input enabled
    mode &= ~ENABLE_QUICK_EDIT_MODE;  // Disable text selection
    mode |= ENABLE_MOUSE_INPUT;       // Enable mouse click detection

    SetConsoleMode(hInput, mode);
}

void drawTest(NeuralNetwork &nn) {
    system("cls");
    fullscreen();
    
    int mouseX = 0, mouseY = 0;
    int clicked = 0;
    int x1 = -1, y1 = -1, x2 = -1, y2 = -1; // Coordinates for the rectangle
    int dragable = 0;
    
    // test drawing
    {
        change_colour(23);
        printf("  ");
        change_colour(15);
        printf("  ");
        reset_colour();
        printf("\n");
        change_colour(15);
        printf("  ");
        change_colour(23);
        printf("  ");
        reset_colour();

        for (int i = 1; i < 28; i++) {
            printf("\n");
        }
        
        for (int i = 0; i < 28; i++) {
            printf("  ");
        }
        change_colour(23);
        printf("  ");
        change_colour(15);
        printf("  ");
        reset_colour();
        printf("\n");

        for (int i = 0; i < 28; i++) {
            printf("  ");
        }
        change_colour(15);
        printf("  ");
        change_colour(23);
        printf("  ");
        reset_colour();
    }
    
    printf("click in order: top left corner");
    
    while (1) {
        if (is_mouse_clicked() && !clicked) {
        	get_mouse_position(&mouseX, &mouseY);
            clicked = 1;
            x1 = mouseX;
            y1 = mouseY;
            printf(", then bottom right corner");
            break;
        }
    }

    while (1) {        
        if (is_mouse_clicked() && !clicked) {
        	get_mouse_position(&mouseX, &mouseY);
            clicked = 1;
            x2 = mouseX;
            y2 = mouseY;
            break;
        } else if (!is_mouse_clicked()) {
            clicked = 0;
        }
    }
    
    system("cls");
    
    unsigned char* grid = new unsigned char[28 * 28]();
    
    // int index = 0;
    hide_cursor();
    enableMouseOnly();
    draw_grid(grid);
    printf("Now you can draw! Press ESC to finish drawing");
    while (1) {
        if (is_mouse_clicked() && (!clicked || dragable)) {
        	get_mouse_position(&mouseX, &mouseY);
            int index = map_click_to_grid(mouseX, mouseY, x1, y1, x2, y2);
            paint_grid(grid, index);
        } else if (!is_mouse_clicked()) {
            clicked = 0;
            dragable = 1;
        }
        
        moveCursorTo(0, 0);
        draw_grid(grid);
        
        // Quit the program if ESC key is pressed
        if (GetAsyncKeyState(VK_ESCAPE)) {
            break;
        }
    }
    show_cursor();

    system("cls");
    draw_grid(grid);
	std::vector<unsigned char> grid_vector(grid, grid + (28 * 28));
    nn.forward(grid_vector);
    int predicted = 0;
    double max_output = nn.output[0];
    for (int j = 1; j < NUM_CLASSES; j++) {
        if (nn.output[j] > max_output) {
            max_output = nn.output[j];
            predicted = j;
        }
    }
    printf("predicted: %d\n", predicted);
    getchar();
    fullscreen();
}