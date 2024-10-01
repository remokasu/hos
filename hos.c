#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>

// Macro definitions
#define TEXT "BABEL"
#define DURATION 3600
#define DELAY 500000  // Delay in microseconds (0.5 seconds)
#define LINE_DELAY 100000  // Delay between lines in microseconds (0.1 seconds)
#define BLINK_INTERVAL 100000  // 0.1 seconds

// Function to blink the screen by changing colors
void blinkScreen() {
    for (int i = 0; i < 3; i++) {
        // Set black background
        bkgd(COLOR_PAIR(1));
        refresh();
        usleep(BLINK_INTERVAL);

        // Set red background
        bkgd(COLOR_PAIR(2));
        refresh();
        usleep(BLINK_INTERVAL);

        // Set white background
        bkgd(COLOR_PAIR(3));
        refresh();
        usleep(BLINK_INTERVAL);
    }
    // After blinking, restore to original black background and red text
    bkgd(COLOR_PAIR(1));
    refresh();
}

// Helper function to print text with delay
void printDelayed(const char *str, int row, int start_col) {
    move(row, start_col);
    for (int i = 0; str[i] != '\0'; i++) {
        addch(str[i]);
        refresh();
        usleep(20000); // 0.01 seconds per character
    }
    usleep(500000); // 0.5 seconds delay after the line
}


void inputPassword(){
    clear();
    bkgd(COLOR_PAIR(1));
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    const char *line1 = "attach cd 01 /";
    const char *line2 = "enter author password";
    const char *line3 = "pass: ";
    int start_col = (COLS - strlen(line1)) / 2;

    printDelayed(line1, LINES / 2 - 1, start_col);
    usleep(1000000);  // Wait 1 second after the first line
    printDelayed(line2, LINES / 2, start_col);
    printDelayed(line3, LINES / 2 + 1, start_col);

    input_mode:
    ;
    char input[100];
    memset(input, 0, sizeof(input));
    echo();
    keypad(stdscr, TRUE);
    
    int ch, input_pos = 0;
    move(LINES / 2 + 1, start_col + strlen(line3));
    while ((ch = getch()) != '\n') {
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (input_pos > 0) {
                input[--input_pos] = '\0';
                mvprintw(LINES / 2 + 1, start_col + strlen(line3), "%s ", input);
                move(LINES / 2 + 1, start_col + strlen(line3) + input_pos);
            }
        } else if (isprint(ch)) {
            if (input_pos < sizeof(input) - 1) {
                input[input_pos++] = ch;
                input[input_pos] = '\0';
            }
        }
        refresh();
    }

    if (strcmp(input, "H.HOBA") != 0) {
        mvprintw(LINES / 2 + 1, start_col + strlen(line3), "%*s", (int)strlen(input), "");
        move(LINES / 2 + 1, start_col + strlen(line3));
        goto input_mode;
    }
}


void dispSpeech() {
    clear();
    bkgd(COLOR_PAIR(1));
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(4));

    const char *lines[] = {
        " Go to, let us go down, and there confound",
        "their language, that they may not understand",
        "one another's speech."
    };

    int max_length = 0;
    for (int i = 0; i < 3; i++) {
        if (strlen(lines[i]) > max_length) {
            max_length = strlen(lines[i]);
        }
    }
    int start_col = (COLS - max_length) / 2;

    for (int i = 0; i < 3; i++) {
        mvprintw(LINES / 2 - 1 + i, start_col, "");
        for (int j = 0; lines[i][j] != '\0'; j++) {
            addch(lines[i][j]);
            refresh();
            usleep(20000);  // Wait 0.02 seconds between each character
        }
        usleep(500000);  // Wait 0.5 seconds after each line
    }
    usleep(3000000);
}


void buildTower() {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);

    clear();
    blinkScreen();

    time_t start_time = time(NULL);
    int max_rows, max_cols;

    getmaxyx(stdscr, max_rows, max_cols);

    char buffer[max_rows][max_cols + 1];
    memset(buffer, 0, sizeof(buffer));

    int line_count = 0;
    time_t last_blink_time = time(NULL);

    while (time(NULL) - start_time < DURATION) {
        if (difftime(time(NULL), last_blink_time) >= 5) {
            blinkScreen();  // Force a blink
            last_blink_time = time(NULL);
        }

        for (int i = 0; i < max_rows - 1; i++) {
            strcpy(buffer[i], buffer[i + 1]);
        }

        int col = 0;
        char new_line[max_cols + 1];
        memset(new_line, 0, sizeof(new_line));

        if (line_count % 2 == 0) {
            new_line[col++] = ' ';
            while (col + strlen(TEXT) <= max_cols) {
                strncat(new_line, TEXT, strlen(TEXT));
                strncat(new_line, " ", 1);
                col += strlen(TEXT) + 1;
            }
            if (col < max_cols) {
                strncat(new_line, TEXT, max_cols - col);
            }
        } else {
            new_line[col++] = TEXT[strlen(TEXT) - 1];
            new_line[col++] = ' '; 

            while (col + strlen(TEXT) <= max_cols) {
                strncat(new_line, TEXT, strlen(TEXT));
                strncat(new_line, " ", 1);
                col += strlen(TEXT) + 1;
            }
            if (col < max_cols) {
                strncat(new_line, TEXT, max_cols - col);
            }
        }
        strncpy(buffer[max_rows - 1], new_line, max_cols);
        buffer[max_rows - 1][max_cols] = '\0';

        clear();
        for (int i = 0; i < max_rows; i++) {
            mvprintw(i, 0, "%s", buffer[i]);
        }
        refresh();
        line_count++;
        usleep(LINE_DELAY);
    }
}

int main() {

    // Initialize ncurses
    initscr();
    start_color();
    cbreak();
    noecho();
    srand(time(NULL));  // Seed for random

    inputPassword();

    curs_set(0);  // Hide cursor
    dispSpeech();
    buildTower();

    endwin();
    return 0;
}
