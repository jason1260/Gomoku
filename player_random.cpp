#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player, avail = 225;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            if (board[i][j] != EMPTY) avail--;
        }
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;
    if (avail == 225) {
        fout << 7 << " " << 7 << '\n';
        fout.flush();
        return;
    }
    //while(true) {
        x = (rand() % SIZE);
        y = (rand() % SIZE);
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << '\n';
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    //}
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}