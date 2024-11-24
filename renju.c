#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 15

// char board[SIZE][SIZE];  // Game board
char board[SIZE][SIZE] = { 0, };
int player = 1;          // Player 1 or 2
char screen[2] = { '1', '2' };

// Clear the screen based on the operating system
void clearScreen() {
#ifdef _TEST
    printf("clear\n");
#else
#ifdef _WIN32
    system("cls");    // Windows
#else
    system("clear");  // Ubuntu, macOS
#endif
#endif
}

// Initialize the board with '.'
void initBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0b00;
        }
    }
}

// Convert Board Status word to '.'/'1'/'2'
inline char board2char(char board) { return board ? screen[board-1] : '.'; }
// Check is index x fit into the board
inline int isFitBoard(int idx) { return 0<=idx && idx<SIZE; }

// Print the game board
void printBoard() {
    printf("   ");
    for (int i = 0; i < SIZE; i++) {
        printf("%c ", 'A' + i);  // Column labels
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%2d ", i + 1);  // Row labels
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board2char(board[i][j]));
        }
        printf("\n");
    }
}

// Convert input like 'A13' into board indices
int convertInput(char col, int row, int *x, int *y) {
    if (col < 'A' || col > 'O' || row < 1 || row > SIZE) {
        return 0;  // Invalid input
    }
    *x = row - 1;
    *y = col - 'A';
    return 1;
}

//
// ***** int checkUtil(int x, int y, int * pattern, int len); *****
//
// Check is [pattern] condition in four direction and return number of direction that satisfy [pattern]
// pattern is mask for len*(2bit word)
// 01 for player 1, 10 for player 2, 00 for empty board
// To clarify end of array, end of the pattern array must be 0 
//
// function rperequire unreline:
//  stone[x][y] == 0b01, the first player
//  player to check [pattern] is always 0b01, the first player
//  End of the pattern array must be 0
//
int checkUtil(const int x, const int y, const int* pattern, const int len) {
    static const int dx[] = {1, 0, 1, 1};
    static const int dy[] = {0, 1, 1, -1}; 
    int pat, flag, bit;
    int res = 0;
    for(int dir=0; dir < 4; dir++) {
        flag = 0;
        for(int idx=0;pat=pattern[idx]; idx++) {
            for(int tar=0, cur=0;tar<len;tar++){
                if ((pat>>tar*2&0b11) != 0b01) { continue; }
                if (x+(len-1-tar)*dx[dir] >= SIZE || x+(-tar)*dx[dir] < 0){ continue; }
                if (!isFitBoard(y+(len-1-tar)*dy[dir]) || !isFitBoard(y+(-tar)*dy[dir])){ continue; }
                for(cur=tar+1;cur<len;cur++){
                    if(board[x+(cur-tar)*dx[dir]][y+(cur-tar)*dy[dir]] != (pat>>cur*2&0b11)) { break; } }
                if (cur < len) { continue; }
                for(cur=tar-1;cur>=0;cur--){
                    if(board[x+(cur-tar)*dx[dir]][y+(cur-tar)*dy[dir]] != (pat>>cur*2&0b11)) { break; } }
                if (cur >= 0) { continue; }
                flag = 1; bit = dir*2 + (tar>len/2);
                if (flag){ res |= 1 << bit; flag=0; continue; }
            } 
        }
    }
    return res;
}

//
// ***** int check33(int x, int y); *****
//
// Check is 33 condition in four direction and return number of direction that satisfy 33
// strongly depends on checkUtil
// pattern is mask for 6*(2bit word)
//
// function prerequire underline:
//  stone[x][y] == 0b01, the first player
//  player to check 33 is always 0b01,the first player
//
int check33(int x, int y) {
    static const int pattern[5] = {0x054, 0x114, 0x144, 0x150, 0};
    static const int pattern_banned[6] = {0x055, 0x115, 0x145, 0x151, 0x154, 0};
    int bann = checkUtil(x, y, pattern_banned, 5); bann |= (bann & 0x5555) << 1 | (bann & 0xAAAA) >> 1;
    int util = checkUtil(x, y, pattern, 6); util &= ~bann;
    int res; for(res=0; util; util >>= 1){ res += util&1; }
    return res;
}

//
// ***** int check44(int x, int y); *****
//
// Check is 44 condition in four direction and return number of direction that satisfy 44
// strongly depends on checkUtil
// pattern is mask for 5*(2bit word)
//
// function prerequire underline:
//  stone[x][y] == 0b01, the first player
//  player to check 44 is always 0b01,the first player
//
int check44(int x, int y) {
    static const int pattern[6] = {0x055, 0x115, 0x145, 0x151, 0x154, 0};
    int util = checkUtil(x, y, pattern, 5);
    int res; for(res=0; util; util >>= 1){ res += util&1; }
    return res;
}

//
// ***** int check6pp(int x, int y); *****
//
// Check is there six or more stones in a row (row that includes [x][y])
// Check6pp returns 1 when the 6pp line exist. Unless, check6pp returns 0.
// strongly depends on checkUtil
//
// function prerequire underline:
//  stone[x][y] == 0b01, the first player
//  player to check 6pp is always 0b01,the first player
//
int check6pp(int x, int y) {
    static const int pattern[2] = {0x555, 0};
    return checkUtil(x, y, pattern, 6) > 0;
}

// Check if there are 5 stones in a row
int checkWin(int x, int y) {
    char stone = board[x][y];
    int dx[] = {1, 0, 1, 1};   // Horizontal, vertical, diagonal directions
    int dy[] = {0, 1, 1, -1};  // Horizontal, vertical, diagonal directions

    for (int dir = 0; dir < 4; dir++) {
        int count = 1;  // Include the current position

        // Check in one direction
        for (int step = 1; step < 5; step++) {
            int nx = x + step * dx[dir];
            int ny = y + step * dy[dir];
            if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && board[nx][ny] == stone) {
                count++;
            } else {
                break;
            }
        }

        // Check in the opposite direction
        for (int step = 1; step < 5; step++) {
            int nx = x - step * dx[dir];
            int ny = y - step * dy[dir];
            if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && board[nx][ny] == stone) {
                count++;
            } else {
                break;
            }
        }

        if (count >= 5) {
            return 1;  // Victory
        }
    }

    return 0;  // Victory condition not met
}

// Play the game
void playGame() {
    char col;
    int row, x, y;

    for(;;) {
        // clearScreen();  // Clear the screen
        printBoard();
        printf("Player %d's turn (enter column and row, e.g., A13): ", player);

        // Input processing
        scanf(" %c", &col); // ignore blank
        col = toupper(col);  // Convert to uppercase

        // Convert Player skin
        if (col == '^') { 
            scanf("%c", &row);
            screen[player-1] = row;
            clearScreen();
            continue;
        }
        scanf("%d", &row);

        // Convert input coordinates to board indices
        if (!convertInput(col, row, &x, &y)) {
            clearScreen();
            printf("Invalid move: Stone out of the board\n");
            continue;
        }

        // Override stone
        if (board[x][y] != 0b00) {
            clearScreen();
            printf("Invalid move: You cannot override stone\n");
            continue;
        }

        // Check 3x3 rule
        if (player == 1 && check33(x, y) >= 2) {
            clearScreen();
            printf("Invalid move: Rule of 3x3.\n");
            continue;
        }

        // Check 4x4 rule
        if (player == 1 && check44(x, y) >= 2) {
            clearScreen();
            printf("Invalid move: Rule of 4x4.\n");
            continue;
        }

        // CHeck 6pp rule
        if (player == 1 && check6pp(x, y)) {
            clearScreen();
            printf("Invalue move: Rule of 6pp.\n");
            continue;
        }

        // Place the stone
        board[x][y] = player;

        // Check for victory condition
        if (checkWin(x, y)) {
            clearScreen();
            printBoard();
            printf("Player %d wins!\n", player);
            break;
        }

        // Switch to the next player
        player = 3 - player;
        clearScreen();
    }
}

int main() {
    char ch[4];
    initBoard();
    playGame();
    gets(ch);
    return 0;
}
