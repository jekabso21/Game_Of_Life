#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <fcntl.h>

#define FRAME_RATE_LIMIT 30

#define DEAD 0
#define ALIVE 1

typedef char* VoxelBuffer;

static VoxelBuffer vbuffer;
static int height, width;

void onExit(int signo) {
	if (vbuffer != NULL) {
		free(vbuffer);
	}

	printf("\e[0m"); // clear color
	printf("\e[?1002l\e[?1015l\e[?1006l"); // Mouse trap all, urxvt, SGR1006  
	printf("\e[?25h"); // show cursor
	printf("\e[1;1f\e[2J"); // clear screen
	system("stty -echo"); // Disable characters printing
								   
	exit(0);
}

void drawScreen(VoxelBuffer buffer) {
	char* symbols[4] = { " ", "▄", "▀", "█" };
	char text[width * height * 2];
	int idx = 0;

	for (int y = 0; y < height / 2; y ++) {
		for (int x = 0; x < width; x ++) {
			idx += sprintf(text + idx, "%s", symbols[(vbuffer[x + y * 2 * width] << 1) | vbuffer[x + (y * 2 + 1) * width]]);
		}

		text[idx++] = '\n';
	}

	// Remove last endl
	text[idx] = '\0';

	// Print to screen
	fputs(text, stdout);
}

void onScreenChange() {
	// Resize buffer
	vbuffer = (VoxelBuffer) realloc(vbuffer, height * width);
	
	// Clear screen and reset buffer
	puts("\e[1;1f\e[2J"); 
	memset(vbuffer, DEAD, height * width);
}

int checkNeighbour(int _x, int _y) {
	int n = 0;

	for (int y = -1; y <= 1; y ++) {
		for (int x = -1; x <= 1; x ++) {
			if (x == 0 && y == 0) continue;
			if (_x + x < 0 || _x + x >= width || _y + y < 0 || _y + y >= height) continue;
			n += vbuffer[(x + _x) + (y + _y) * width] != DEAD;
		}
	}

	return n;
}

void onUpdate() {
	char buffer[height * width];

	for (int y = 0; y < height; y ++) {
		for (int x = 0; x < width; x ++) {
			int n = checkNeighbour(x, y);
			buffer[x + y * width] = (n == 2 && buffer[x + y * width] == ALIVE) || n == 3 ? ALIVE : DEAD;
		}
	}

	memcpy(vbuffer, buffer, height * width);

	// Reset mouse cursor & draw to terminal
	printf("\033[1;1H");
	drawScreen(vbuffer);
}

int main() {
	struct timespec start, end;

	char test[64];
	int flags;
	int x, y, n;
	
	signal(SIGINT, onExit);

	system("stty -icanon"); // Enable shell input
	system("stty -echo"); // Disable characters printing
	printf("\e[?1002h\e[?1015h\e[?1006h"); // Mouse trap all, urxvt, SGR1006  
	printf("\e[?25l"); // hide cursor
	printf("\e[0;34m"); // set color blue

	// Initialize default values
	height = 0;
	width = 0;
	vbuffer = NULL;
	flags = fcntl(STDIN_FILENO, F_GETFL);

	for (;;) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);

		// Check if screen size changed
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		if (height != w.ws_row * 2 - 3 || width != w.ws_col - 1) {
			height = w.ws_row * 2 - 3;
			width = w.ws_col - 1;
			onScreenChange();
		}

		// Read mouse input non blocking
// Set the file descriptor for stdin to non-blocking mode
fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

// Read from stdin while there is data available
while ((n = read(STDIN_FILENO, test, 64)) > 0) {

    // Initialize a pointer to the start of the buffer
    char* ptr = test;

    // Find the first semicolon or the end of the string
    while ((*ptr) != ';' && (*ptr) != '\0') ptr++;

    // Convert the string after the semicolon to an integer for the x coordinate
    x = atoi(++ptr);

    // Find the next semicolon or the end of the string
    while ((*ptr) != ';' && (*ptr) != '\0') ptr++;

    // Convert the string after the semicolon to an integer for the y coordinate
    // Multiply by 2 to account for the 2x2 cell size
    y = atoi(++ptr) * 2;

    // If the x or y coordinate is out of bounds, skip this iteration
    if (x + 1 >= width || y + 1 >= height) continue;

    // Set the 2x2 cells at the given coordinates to ALIVE
    vbuffer[x + y * width] = ALIVE;
    vbuffer[x + 1 + y * width] = ALIVE;
    vbuffer[x + (y+1) * width] = ALIVE;
    vbuffer[x + 1 + (y+1) * width] = ALIVE;
}

// Reset the file descriptor for stdin to blocking mode
fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);

// Call the function to update the screen
onUpdate();

// Get the current time
clock_gettime(CLOCK_MONOTONIC_RAW, &end);

// Calculate the time difference in microseconds
long delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

// Sleep for the remaining time in the frame, if any
usleep(fmax(1000000.0 / FRAME_RATE_LIMIT - delta_us, 0));
	}
}