// Connect 4 AI - Version 1.0 //

#include <iostream>
#include <vector>
#include <limits>
#include <thread>
#include <chrono>

using namespace std;

int turn = 0;

// Energy Matrix
const int energy[6][7] = {
    {3, 4, 5, 7, 5, 4, 3},
    {4, 6, 8, 10, 8, 6, 4},
    {5, 8, 11, 13, 11, 8, 5},
    {5, 8, 11, 13, 11, 8, 5},
    {4, 6, 8, 10, 8, 6, 4},
    {3, 4, 5, 7, 5, 4, 3}
};

void setupGame() {
    int turnChoice = -1;

    cout << "Please select your game piece.\n";
    cout << "0 = Player 0   1 = Player 1\n";

    while (true) {
        cout << "Enter your choice (0 or 1): ";

        if (cin >> turnChoice) {
            if (turnChoice == 0) {
                cout << "You have selected: Player 0. You will go first.\n";
                turn--;
                break;
            } else if (turnChoice == 1) {
                cout << "You have selected: Player 1. You will go second.\n";
                break;
            } else {
                cout << "Invalid option. Please select another game piece.\n";
            }
        } else {
            cout << "Invalid input. Please enter 0 or 1.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


// Initialize Game Board
void initializeBoard(char board[6][7]) {
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            board[row][col] = '.';
        }
    }
}

// Checking for full board
bool is_full(char board[6][7]) {
    for (int col = 0; col < 7; ++col) {
        if (board[0][col] == '.') {
            return false;
        }
    }
    return true;
}

// Printing the current board
void print_board(char board[6][7]) {
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            cout << board[row][col] << " ";
        }
        cout << endl;
    } 
}

// Insert a piece into the board
void insert_piece(char board[6][7], char gamePiece, int column) {
    int emptyRow = 0;

    while (true) {
        if (column >= 0 && column <= 6) {
            break;
        }

        cout << "Invalid column. Please select a column between 0 and 6.\n";
        cout << "Select a column (0-6) to place your game piece: ";

        if (!(cin >> column)) {
            cout << "Invalid input. Please enter a number between 0 and 6.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    if(board[0][column] != '.') {
        cout << "This column is full. Please select a different column: \n";
        turn--;
        print_board(board);
        return;
    }

    for (int row = 5; row >= 0; --row) {
        if (board[row][column] == '.') {
            emptyRow = row;
            break;
        }
    }

    board[emptyRow][column] = gamePiece;
    print_board(board);
    cout << endl;
}

// Function for performing a turn
void perform_turn(char board[6][7], int playerNum) {
    char gamePiece = (playerNum == 0) ? 'X' : 'O';
    int columnSelection;
    
    print_board(board);

    while (true) {
        cout << "Select a column (0-6) to place your game piece: ";
        
        if (cin >> columnSelection) {
            if (columnSelection >= 0 && columnSelection <= 6) {
                cout << "You selected column: " << columnSelection << endl;
                insert_piece(board, gamePiece, columnSelection);
                break;
            } else {
                cout << "Invalid column. Please select a column between 0 and 6.\n";
            }
        } else {
            cout << "Invalid input. Please enter an integer between 0 and 6.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Checking for Connect 4
bool check_for_Connect_4 (char board[6][7], int playerNumber) {
    char gamePiece = (playerNumber == 0) ? 'X' : 'O';

    for (int row = 5; row >= 0; --row) {
        for (int col = 6; col >= 0; --col) {

            // Case 1: check for straight in the same row (go from right to left)
            if (col >= 3) { 
                if (board[row][col] == gamePiece &&
                    board[row][col - 1] == gamePiece &&
                    board[row][col - 2] == gamePiece &&
                    board[row][col - 3] == gamePiece) {
                    return true;
                }
            }

            // Case 2: check for straight in same col (go from bottom to top)
            if (row >= 3) {
                if (board[row][col] == gamePiece &&
                    board[row - 1][col] == gamePiece &&
                    board[row - 2][col] == gamePiece &&
                    board[row - 3][col] == gamePiece) {
                    return true;
                }
            }

            // Case 3: Check for diagonal from bottom-left to top-right
            if (row >= 3 && col >= 3) {
                if (board[row][col] == gamePiece &&
                    board[row - 1][col - 1] == gamePiece &&
                    board[row - 2][col - 2] == gamePiece &&
                    board[row - 3][col - 3] == gamePiece) {
                    return true;
                }
            }

            // Case 4: Check for diagonal from top-left to bottom-right
            if (row >= 3 && col <= 3) { 
                if (board[row][col] == gamePiece &&
                    board[row - 1][col + 1] == gamePiece &&
                    board[row - 2][col + 2] == gamePiece &&
                    board[row - 3][col + 3] == gamePiece) {
                    return true;
                }
            }
        }
    }

    return false;
}

// Helper function - Find the next available row in a column
int get_next_available_row(char board[6][7], int col) {
    for (int row = 5; row >= 0; --row) {
        if (board[row][col] == '.') {
            return row;
        }
    }
    return -1; // Should not reach here if called correctly
}

int heuristicEvaluation(char board[6][7], char gamePiece) {
    int score = 0;

    // Scoring based on the energy matrix and piece alignments
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            if (board[row][col] == gamePiece) {
                score += energy[row][col];
            }
        }
    }
    return score;
}

int minimax(char board[6][7], int depth, bool isMaximizingPlayer) {
    if (check_for_Connect_4(board, 0)) {
        return 1000 - depth;
    }

    if (check_for_Connect_4(board, 1)) {
        return -1000 + depth;
    }

    if (is_full(board)) {
        return 0;
    }

    if (depth >= 6) {  // Add a depth limit for efficiency
        return heuristicEvaluation(board, isMaximizingPlayer ? 'X' : 'O');
    }

    if (isMaximizingPlayer) {
        int maxEval = INT16_MIN;
        for (int col = 0; col < 7; ++col) {
            if ((board[0][col]) == '.') {
                int row = get_next_available_row(board, col);
                board[row][col] = 'X';

                int eval = minimax(board, depth + 1, false);
                maxEval = max(maxEval, eval);

                board[row][col] = '.';
            }
        }

        return maxEval;
    } else {
        int minEval = INT16_MAX;
        for (int col = 0; col < 7; ++col) {
            if ((board[0][col]) == '.') {
                int row = get_next_available_row(board, col);
                board[row][col] = 'O';

                int eval = minimax(board, depth + 1, true);
                minEval = min(minEval, eval);

                board[row][col] = '.';
            }
        }

        return minEval;
    }

    // Create a decision board with weights for usefulness of each position on the board
    // base cases:
    // Case 1: if the current state leads to the AI winning, set its value to positive infinity
    // Case 2: if the current state leads to player winning, set its value to negative infinity
    // Case 3: if the current state leacs to a draw, set its value to 0

    // if it is AI's (maximizing player's) turn, set the current winning probability to negative infinity 
    // recursively call minimax to find the best resulting value from each play made.
    // if it is less than the current best, then prune the branch and do not move further. 

    // if it is the human player's (minimizing player's) turn, set the current value to positive infinity
    // recursively call minimax with the maximizing player to find the best resulting value.
    // if the score is worse than the current range, then prune the branch.
}

int get_best_move(char board[6][7]) {
    int bestScore = INT16_MIN;
    int bestCol = -1; 

    for (int col = 0; col < 7; ++col) {
        if (board[0][col] == '.') {
            int row = get_next_available_row(board, col);
            board[row][col] = 'X';
            int score = minimax(board, 0, false);
            board[row][col] = '.';
            if (score > bestScore) {
                bestScore = score;
                bestCol = col;
            }
        }
    }

    return bestCol;
}

int main() {

    int gameState = 0;
    
    while (gameState == 0) {
        setupGame();

        char board[6][7];

        initializeBoard(board);

        while (!is_full(board)) {
            int playerNum = 0;

            if (turn % 2 == 0) {
                playerNum = 0;
                cout << "Computer's Turn:\n";
                this_thread::sleep_for(chrono::seconds(1));
                int chosenCol = get_best_move(board);
                insert_piece(board, 'X', chosenCol);
            } else {
                playerNum = 1;
                perform_turn(board, playerNum);
            }

            if (turn >= 6) {
                if (check_for_Connect_4(board, playerNum)) {
                    cout << "Player " << playerNum << " wins!\n";
                    break;
                }
            }

            if(is_full(board)) {
                cout << "Result: Tie\n";
            }

            ++turn;
        }
        
        char playAgain;
        cout << "Would you like to play again? (Y/N)\n";
        cin >> playAgain;
        if (playAgain != 'Y' && playAgain != 'y') {
            gameState++;
        } 
    }

    return 0;
}