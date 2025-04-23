#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int size;
    int num_pieces;
    int** state;

    int player_1;
    int player_2;
    int empty;

    int turn_player;

} Board;

typedef struct {
    int row;
    int col;
} Position;

// Return the size of the board struct in bytes
int size_of_board_struct() {
    // integers: size, num_pieces, player_1, player_2, empty
    // pointers: state
    return (
        sizeof(int) * 5 + sizeof(int**)
    );
}

// Allocate memory for the board struct
Board* create_board() {
    Board* board = (Board*) malloc(size_of_board_struct());
    // Define player pieces and empty space 
    board->player_1 = 1;
    board->player_2 = 2;
    board->empty = 0;
    // Define the turn player
    board->turn_player = 0;
    // Define board size and number of pieces
    board->num_pieces = 3;
    board->size = 3;

    // Allocate memory for the board state
    board->state = (int**)malloc(board->size * sizeof(int*));
    for (int i = 0; i < board->size; i++) {
        board->state[i] = (int*)malloc(board->size * sizeof(int));
        for (int j = 0; j < board->size; j++) {
            board->state[i][j] = board->empty;
        }
    }

    // Initialize the board with pieces
    for (int i = 0; i < board->num_pieces; i++) {
        board->state[0][i] = board->player_2;
        board->state[board->size-1][i] = board->player_1;
    }
    
    return board;
}

// Free the allocated memory for the board struct
void delete_board(Board* board) {
    // Free the allocated memory for the board state
    for (int i = 0; i < board->size; i++) {
        free(board->state[i]);
    }
    free(board->state);
    free(board);
    return;
}


void print_board(Board* board) {
    // Format
    // 1 -- 2 -- 3
    // | \  |  / |
    // |  \ | /  |
    // 4 -- 5 -- 6
    // |  / | \  |
    // | /  |  \ |
    // 7 -- 8 -- 9
    
    // Print the board state
    // Print top numbers row
    for (int j = 0; j < board->size; j++) {
        printf("%d", board->state[0][j]);
        if (j < board->size-1) {
            printf("---");
        } else {
            printf(" // Linha %d", 0);
        }
    }
    printf("\n");

    // Print connection patterns for middle rows
    for (int i = 1; i < board->size; i++) {
        // Print vertical connections
        for (int j = 0; j < board->size; j++) {
            printf("|");
            if (j < board->size-1) {
                // Determine connection direction
                if ((i + j) % 2 == 0) {
                    printf(" / ");
                }
                else {
                    printf(" \\ ");
                }
            }
        }
        printf("\n");

        // Print numbers row
        for (int j = 0; j < board->size; j++) {
            printf("%d", board->state[i][j]);
            if (j < board->size-1) {
                printf("---");
            } else {
                printf(" // Linha %d", i);
            }
        }
        printf("\n");
    }
}


bool valid_moves(Board* board, Position old_pos, Position new_pos) {
    // Check if the chosen position is valid
    if (
        old_pos.row < 0 || old_pos.row >= board->size 
        || old_pos.col < 0 || old_pos.col >= board->size) {
        printf("\n** MOVIMENTO INVALIDO: Posicao inexistente. ");
        return false;
    }
    if (
        new_pos.row < 0 || new_pos.row >= board->size 
        || new_pos.col < 0 || new_pos.col >= board->size) {
        printf("\n** MOVIMENTO INVALIDO: Posicao inexistente. ");
        return false;
    }
    // Check if player is moving their own piece
    if (board->state[old_pos.row][old_pos.col] != board->turn_player) {
        printf(
            "\n **MOVIMENTO INVALIDO: Nao ha peca do jogador na posicao "
            "escolhida. "
        );
        return false;
    }
    // Check if the piece can move
    if ((old_pos.row == new_pos.row) && (old_pos.col == new_pos.col)) {
        printf(
            "\n** MOVIMENTO INVALIDO: A posicao de origem e igual a "
            "posicao de destino. "
        );
        return false;
    }
    // Check if the new position is not adjacent to the old position

    // Is a middle element on side ([0,1], [1,0], [1,2], [2,1])
    bool is_edge_middle = (
        (old_pos.row == 0 && old_pos.col == 1)
        || (old_pos.row == 1 && old_pos.col == 0)
        || (old_pos.row == 1 && old_pos.col == 2)
        || (old_pos.row == 2 && old_pos.col == 1)
    );
    // Positions are adjacent diagonally
    bool is_adjacent_diagonally = (
        (old_pos.row == new_pos.row - 1 && old_pos.col == new_pos.col - 1)
        || (old_pos.row == new_pos.row -1 && old_pos.col == new_pos.col + 1)
        || (old_pos.row == new_pos.row + 1 && old_pos.col == new_pos.col - 1)
        || (old_pos.row == new_pos.row + 1 && old_pos.col == new_pos.col + 1)
    );
    // Positions are adjacent vertically or horizontally
    bool is_adjacent_sides = (
        (old_pos.row == new_pos.row - 1 && old_pos.col == new_pos.col)
        || (old_pos.row == new_pos.row + 1 && old_pos.col == new_pos.col)
        || (old_pos.row == new_pos.row && old_pos.col == new_pos.col - 1)
        || (old_pos.row == new_pos.row && old_pos.col == new_pos.col + 1)
    );

    // Consider all 8 adjacent positions
    // Except diagonals for middle elements on border 
    // Since [0,1], [1,0], [1,2], [2,1] are not adjacent to any of them
    bool is_adjacent = (
        is_adjacent_sides || is_adjacent_diagonally
    );
    bool is_adjacent_middle_border = (
        is_adjacent_diagonally && is_edge_middle
    );
    if (!is_adjacent || is_adjacent_middle_border) {
        printf(
            "\n** MOVIMENTO INVALIDO: A peca so pode se mover para uma "
            "posicao adjacente. "
        );
        return false;
    }
    // Check if the new position is adjacent to the old position
    // Check if the chosen position is valid
    if (board->state[new_pos.row][new_pos.col] != board->empty) {
        printf("\n** MOVIMENTO INVALIDO: Posicao ocupada. ");
        return false;
    }
    return true;
}

// Check for a winner
bool is_winner(Board* board) {
    // Check rows
    // Player 1 wins on rows 1, 2
    // Player 2 wins on rows 0, 1
    int start = 0;
    int end = board->size - 1;
    if (board->turn_player == board->player_1) {
        end = board->size - 2;
    } else {
        start = 1;
    }
    for (int i = start; i <= end; i++) {
        bool different_on_line = false;
        for (int j = 0; j < board->size && !different_on_line; j++) {
            different_on_line = (board->state[i][j] != board->turn_player);
        }
        if (!different_on_line) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < board->size; j++) {
        bool different_on_column = false;
        for (int i = 0; i < board->size && !different_on_column; i++) {
            different_on_column = (board->state[i][j] != board->turn_player);
        }
        if (!different_on_column) {
            return true;
        }
    }

    // Check diagonals

    // Check main diagonal
    bool different_on_main_diagonal = false;
    for (int i = 0; i < board->size && !different_on_main_diagonal; i++) {
        different_on_main_diagonal = (
            board->state[i][i] != board->turn_player
        );
    }
    if (!different_on_main_diagonal) {
        return true;
    }

    // Check anti diagonal
    bool different_on_anti_diagonal = false;
    for (int i = 0; i < board->size && !different_on_anti_diagonal; i++) {
        different_on_anti_diagonal = (
            board->state[i][board->size - 1 - i] != board->turn_player
        );
    }
    if (!different_on_anti_diagonal) {
        return true;
    }
    // No winner
    return false;
}

void make_move(
    Board* board, 
    Position old_pos,
    Position new_pos,
    bool* move_played
) {
    // Make a move for the player
    if (valid_moves(board, old_pos, new_pos)) {
        board->state[old_pos.row][old_pos.col] = board->empty;
        board->state[new_pos.row][new_pos.col] = board->turn_player;
        *move_played = true;
    } else {
        printf("Tente novamente.\n\n");
        *move_played = false;
    }
    return;
}

void get_player_move(
    int player, 
    Position* old_pos,
    Position* new_pos
) {
    // Get the player's move
    // Prompt the player for a move
    // Asks for the row and column of the piece to move
    printf(
        "Jogador %d, escolha uma peça (linha e coluna) para mover: ", 
        player
    );
    scanf("%d", &(old_pos->row));
    scanf("%d", &(old_pos->col));
    // Asks for the row and column for the new position
    printf(
        "Jogador %d, escolha posicao (linha e coluna) para onde mover: ", 
        player
    );
    // Read the row and column from the user
    scanf("%d", &(new_pos->row));
    scanf("%d", &(new_pos->col));
}

void play_turn(Board* board) {
    // Play a turn for the player
    bool turn_played = false;
    while (!turn_played) {
        // Get the player's move
        Position old_pos;
        Position new_pos;
        get_player_move(board->turn_player, &old_pos, &new_pos);
        // Make the move if valid
        make_move(board, old_pos, new_pos, &turn_played);

    }
}

// Implement player vs player logic
void player_vs_player() {
    printf("Player vs Player\n");
    
    // Initialize board
    Board* board = create_board();
    print_board(board);
    // Implement game logic here
    int winner = 0;
    int num_rounds = 10;
    for (int i = 0; i < num_rounds && winner == 0; i++) {
        board->turn_player = i % 2 + 1;
        play_turn(board);
        // Check for winner
        if (is_winner(board)) {
            winner = board->turn_player;
        }
        print_board(board);
    }
    // Print the winner
    if (winner == 0) {
        printf("Empate!\n");
    } else {
        printf("Jogador %d venceu!\n", winner);
    }
    // Free the allocated memory for the board
    delete_board(board);
}

void print_menu() {
    printf("1. Jogador vs Jogador\n");
    printf("2. Jogador (P1) vs Computador (P2)\n");
    printf("2. Jogador (P2) vs Computador (P1)\n");
    printf("4. Sair\n");
    printf("Escolha uma opcao: ");
}

int get_menu_option() {
    print_menu();
    int option;
    scanf("%d", &option);
    return option;
}

// Implement player vs computer logic
void player_vs_computer(bool player_starts) {
    printf("Player vs Computer\n");
    printf("Not done yet.\n");
}





int main() {
    // Initial menu
    bool valid_option;
    printf("Bem-vindo ao jogo do pe de galinha!\n");
    printf("O jogo consiste em 2 jogadores que jogam alternadamente.\n");
    printf("O objetivo do jogo e formar uma linha de 3 pecas.\n");
    printf(
        "O jogador 1 e representado pelo numero 1 "
        "e o jogador 2 pelo numero 2.\n"
    );
    printf("O jogador 1 inicia o jogo.\n");

    do {
        int option = get_menu_option();
        switch (option) {
            case 1:
                valid_option = true;
                player_vs_player();
                break;
            case 2:
                valid_option = true;
                player_vs_computer(true);
                break;
            case 3:
                valid_option = true;
                player_vs_computer(false);
                break;
            case 4:
                valid_option = true;
                break;
            default:
                valid_option = false;
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (!valid_option);
    // Create the board
    printf("Programa finalizado.\n");
    return 0;
}