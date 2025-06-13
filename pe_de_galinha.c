#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define PLAYER_1 'X'
#define PLAYER_2 'O'
#define EMPTY '+'


#define NUM_PIECES 3
#define BOARD_SIZE 3

#define NUM_NODES (BOARD_SIZE * BOARD_SIZE)

#define MAX_ROUNDS 30

#define MAX_TREE_HEIGHT 4

// Max number of moves is the degree of the middle node
// There are 9 nodes in the board and 6 pieces
// So there can be at most 3 moves for each piece of the player
#define MAX_MOVES 9

typedef enum {
    id_player_1,
    id_player_2,
    id_empty
} PlayerIndex;

// Struct prototypes
typedef struct Position Position;
typedef struct Move Move;
typedef struct AdjacencyMatrix AdjacencyMatrix;
typedef struct Board Board;
typedef struct BoardState BoardState;

// Struct definitions
typedef struct Position {
    int row;
    int col;
} Position;

typedef struct Move {
    Position origin;
    Position destiny;
} Move;

typedef struct AdjacencyMatrix {
    bool** matrix;
    int size;
} AdjacencyMatrix;

typedef struct Board {
    int size;
    int num_pieces;
    int** state;
    int turn_player;
    int winner;
    AdjacencyMatrix* adj_matrix;
} Board;

typedef struct BoardState {
    Board* board;
    // Use "struct BoardState" here (since it's not fully defined yet)
    struct BoardState* parent;
    // Children of the board state
    BoardState** children;
    int num_children;
    // Child number (index) in the parent's children array
    int child_number;
    int height;
    int score;
} BoardState;


// Function prototypes
// Explanations are in the function definitions below

// Position functions
bool positions_are_equal(Position pos_a, Position pos_b);
int convert_position_to_node(Position pos, int size);

// Adjacency matrix functions
int size_of_adjacency_matrix();
void set_neighbor_edge(AdjacencyMatrix* adj_matrix, int node_a, int node_b);
void set_neigborhood(AdjacencyMatrix* adj_matrix);
AdjacencyMatrix* create_adjacency_matrix(int size);
void delete_adjacency_matrix(AdjacencyMatrix* adj_matrix);

// Board functions
int size_of_board_struct();
bool board_position_valid(Board* board, Position pos);
Board* create_board();
void delete_board(Board* board);
void copy_board(Board* board, Board* copy);
bool connected(Board* board, Position pos_node_a, Position pos_node_b);
int get_player(Position pos, Board* board);
void set_player(Position pos, int player, Board* board);
char get_player_from_symbol(char symbol);
char get_symbol_from_player(int player);
void print_board(Board* board);


// Move functions
bool is_valid_move_with_prints(Board* board, Move move);
void add_move_if_valid(
    Move** valid_moves, 
    int* move_count, 
    Position origin,
    Position destiny,
    Board* board
);
void add_moves_from_position(
    Move** valid_moves, 
    int* move_count, 
    Position origin,
    Board* board
);
void list_valid_moves(
    Board* board, 
    Move** valid_moves, 
    int* move_count
);
Move* get_computer_move(
    Board* board,
    int player_id,
    int computer_id
);
Move* get_player_move(int player);
void make_move(Board* board, Move move, bool* move_player);


// Board state functions
int size_of_board_state();
BoardState* create_board_state(Board* board, BoardState* parent);
void delete_board_state(BoardState* state);
bool is_winning_move(Board* board, Move* move);
Move* get_winning_move(BoardState* state, Move** moves, int num_moves);
int calculate_state_score(BoardState* state, int player, int computer_player);
void calculate_root_children_score(
    BoardState* root,
    int player_id,
    int computer_id,
    Move** moves,
    int num_moves
);
int get_move_with_highest_score_position(
    BoardState** children, 
    int num_children, 
    int computer_id
);
Move* get_best_move(BoardState* root, int player_id, int computer_id);

// Game functions
bool win_by_row(Board* board, int row);
bool win_by_column(Board* board, int col);
bool win_by_main_diagonal(Board* board);
bool win_by_anti_diagonal(Board* board);
bool is_winner_by_rows(Board* board);
bool is_winner_by_columns(Board* board);
bool is_winner_by_diagonals(Board* board);
bool player_is_winner(Board* board);
void play_user_turn(Board* board);
void play_computer_turn(Board* board);
void player_vs_player();
void player_vs_computer(bool player_starts);
void print_menu();
int get_menu_option();








// **********
// Position functions

// Check if two positions are equal
bool positions_are_equal(
    Position pos_a, 
    Position pos_b
) {
    // Check if two positions are equal
    return (pos_a.row == pos_b.row && pos_a.col == pos_b.col);
}


// Convert a position to an index in the adjacency matrix
// It is considered that the input position is valid
int convert_position_to_node(
    Position pos, 
    int size
) {
    return pos.row * size + pos.col;
}


// **********
// Adjacency matrix functions

// Calculate the size of the adjacency matrix
int size_of_adjacency_matrix() {
    return sizeof(bool**) + sizeof(int);
}


// Set the edge in the adjacency matrix
void set_neighbor_edge(
    AdjacencyMatrix* adj_matrix, 
    int node_a, 
    int node_b
) {
    adj_matrix->matrix[node_a][node_b] = true;
    adj_matrix->matrix[node_b][node_a] = true; // Undirected graph
}


// Set the edges for each node
void set_neigborhood(AdjacencyMatrix* adj_matrix) {
    // First row
    // ****

    // Node 0: (0, 0)
    // (0, 0) <-> (0, 1)
    set_neighbor_edge(adj_matrix, 0, 1);
    // (0, 0) <-> (1, 0)
    set_neighbor_edge(adj_matrix, 0, 3);
    // (0, 0) <-> (1, 1)
    set_neighbor_edge(adj_matrix, 0, 4);
    
    // Node 1: (0, 1)
    // (0, 1) <-> (0, 2)
    set_neighbor_edge(adj_matrix, 1, 2);
    // (0, 1) <-> (1, 1)
    set_neighbor_edge(adj_matrix, 1, 4);

    // Node 2: (0, 2)
    // (0, 2) <-> (1, 2)
    set_neighbor_edge(adj_matrix, 2, 5);
    // (0, 2) <-> (1, 1)
    set_neighbor_edge(adj_matrix, 2, 4);

    // Second row
    // ****

    // Node 3: (1, 0)
    // (1, 0) <-> (1, 1)
    set_neighbor_edge(adj_matrix, 3, 4);
    // (1, 0) <-> (2, 0)
    set_neighbor_edge(adj_matrix, 3, 6);
    
    // Node 4: (1, 1)
    // (1, 1) <-> (1, 2)
    set_neighbor_edge(adj_matrix, 4, 5);
    // (1, 1) <-> (2, 0)
    set_neighbor_edge(adj_matrix, 4, 6);
    // (1, 1) <-> (2, 1)
    set_neighbor_edge(adj_matrix, 4, 7);
    // (1, 1) <-> (2, 2)
    set_neighbor_edge(adj_matrix, 4, 8);
    
    // Third row
    // ***

    // Node 5: (1, 2)
    // (1, 2) <-> (2, 2)
    set_neighbor_edge(adj_matrix, 5, 8);
    
    // Node 6: (2, 0)
    // (2, 0) <-> (2, 1)
    set_neighbor_edge(adj_matrix, 6, 7);
    
    // Node 7: (2, 1)
    // (2, 1) <-> (2, 2)
    set_neighbor_edge(adj_matrix, 7, 8);
}


// Create an adjacency matrix of the given size
AdjacencyMatrix* create_adjacency_matrix(int size) {
    // Allocate memory for the structure
    AdjacencyMatrix* adj_matrix = (
        (AdjacencyMatrix*) malloc(size_of_adjacency_matrix())
    );
    adj_matrix->size = size;
    
    // Allocate memory for the matrix
    adj_matrix->matrix = (bool**) malloc(size * sizeof(bool*));
    for (int i = 0; i < size; i++) {
        adj_matrix->matrix[i] = (bool*) malloc(size * sizeof(bool));
        for (int j = 0; j < size; j++) {
            // Initialize the adjacency matrix with the neighborhood values
            adj_matrix->matrix[i][j] = false;
        }
    }
    
    // Set the neighborhood edges
    set_neigborhood(adj_matrix);

    // Return the adjacency matrix
    return adj_matrix;
}


// Free the allocated memory for the adjacency matrix
void delete_adjacency_matrix(AdjacencyMatrix* adj_matrix) {
    // Free each row of the matrix
    for (int i = 0; i < adj_matrix->size; i++) {
        free(adj_matrix->matrix[i]);
    }
    free(adj_matrix->matrix);
    // Free the adjacency matrix structure
    free(adj_matrix);
    return;
}


// **********
// Board functions

// Return the size of the board struct in bytes
int size_of_board_struct() {
    // integers: size, num_pieces, turn_player
    // pointers: adj_matrix, state
    return (
        sizeof(int) * 4 + sizeof(int**) + sizeof(AdjacencyMatrix*)
    );
}


// Check if a position is valid on the board
bool board_position_valid(Board* board, Position pos) {
    // Check if row is invalid
    if (pos.row < 0 || pos.row >= board->size) {
        return false;
    }
    // Check column is invalid
    if (pos.col < 0 || pos.col >= board->size) {
        return false;
    }
    // If both checks passed, the position is valid
    return true;
}


// Initialize a new board
Board* create_board() {
    // Allocate memory for the board struct
    Board* board = (Board*) malloc(size_of_board_struct());

    // Define the turn player as an invalid value
    board->turn_player = id_empty;
    // Define board size and number of pieces
    board->num_pieces = NUM_PIECES;
    board->size = BOARD_SIZE;

    // Initialize with no winner
    board->winner = id_empty;

    // Allocate memory for the board state
    board->state = (int**)malloc(board->size * sizeof(int*));
    for (int i = 0; i < board->size; i++) {
        board->state[i] = (int*)malloc(board->size * sizeof(int));
        for (int j = 0; j < board->size; j++) {
            // Initialize as empty values
            board->state[i][j] = id_empty;
        }
    }

    // Set the board pices at the initial positions
    for (int i = 0; i < board->num_pieces; i++) {
        board->state[0][i] = id_player_2;
        board->state[board->size-1][i] = id_player_1;
    }

    // Create the adjacency matrix
    board->adj_matrix = create_adjacency_matrix(NUM_NODES);

    return board;
}


// Free the allocated memory for the board struct
void delete_board(Board* board) {
    // Free the allocated memory for the board state
    for (int i = 0; i < board->size; i++) {
        free(board->state[i]);
    }
    free(board->state);
    
    // Free the allocated memory for the adjacency matrix
    delete_adjacency_matrix(board->adj_matrix);

    
    free(board);
    board = NULL;
    return;
}


// Copy a Board variable to another
void copy_board(Board* board, Board* copy) {
    copy->size = board->size;
    copy->num_pieces = board->num_pieces;
    
    copy->turn_player = board->turn_player;
    copy->winner = board->winner;

    // Allocate memory for the board state
    copy->state = (int**)malloc(copy->size * sizeof(int*));
    for (int i = 0; i < copy->size; i++) {
        copy->state[i] = (int*)malloc(copy->size * sizeof(int));
        for (int j = 0; j < copy->size; j++) {
            copy->state[i][j] = board->state[i][j];
        }
    }

    // Copy the adjacency matrix
    copy->adj_matrix = malloc(sizeof(AdjacencyMatrix));

    copy->adj_matrix->size = board->adj_matrix->size;
    copy->adj_matrix->matrix = (
        (bool**) malloc(copy->adj_matrix->size * sizeof(bool*))
    );
    
    for (int i = 0; i < copy->adj_matrix->size; i++) {
        copy->adj_matrix->matrix[i] = (
            (bool*) malloc(copy->adj_matrix->size * sizeof(bool))
        );
        for (int j = 0; j < copy->adj_matrix->size; j++) {
            copy->adj_matrix->matrix[i][j] = board->adj_matrix->matrix[i][j];
        }
    }
}

// Check if two nodes are connected in the adjacency matrix
bool connected(
    Board* board,
    Position pos_node_a,
    Position pos_node_b
) {
    // Check if positions are invalid
    if (!board_position_valid(board, pos_node_a)) {
        return false;
    }
    if (!board_position_valid(board, pos_node_b)) {
        return false;
    }

    // Get the index of the nodes in the adjacency matrix
    int node_a = convert_position_to_node(pos_node_a, board->size);
    int node_b = convert_position_to_node(pos_node_b, board->size);
    
    // Check if the indices are valid
    if (node_a < 0 || node_b < 0) {
        return false;
    }

    // Return the value of the adjacency matrix
    // Which indicates if the nodes are connected
    return board->adj_matrix->matrix[node_a][node_b];
}


// Get the player on the position
int get_player(Position pos, Board* board) {
    // Verify if the position is valid
    if (!board_position_valid(board, pos)) {
        return id_empty;
    }
    return board->state[pos.row][pos.col];
}


// Set the player on the position
void set_player(Position pos, int player, Board* board) {
    // Verify if the position is valid
    if (!board_position_valid(board, pos)) {
        printf("Posicao invalida.\n");
        return;
    }
    board->state[pos.row][pos.col] = player;
    return;
}


// Get the player using its symbol
char get_player_from_symbol(char symbol) {
    if (symbol == PLAYER_1) {
        return id_player_1;
    } else if (symbol == PLAYER_2) {
        return id_player_2;
    }
    return id_empty;
}


// Get the symbol from the player id
char get_symbol_from_player(int player) {
    if (player == id_player_1) {
        return PLAYER_1;
    } else if (player == id_player_2) {
        return PLAYER_2;
    }
    return EMPTY;
}


// Print the board state
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
    
    // Print the indices on top of the board
    printf("Tabuleiro:\n");
    printf("   |  ");
    for (int j = 0; j < board->size; j++) {
        printf("%d", j);
        if (j < board->size-1) {
            printf(" | ");
        }
    }
    printf("\n");
    printf("***************\n");
    printf("   |  ");
    printf("\n");
    printf(" 0 |  ");

    // Print first row
    for (int j = 0; j < board->size; j++) {
        printf("%c", get_symbol_from_player(board->state[0][j]));
        if (j < board->size-1) {
            printf("---");
        }
    }
    printf("\n");
    // Print connection patterns for middle rows
    for (int i = 1; i < board->size; i++) {
        printf("   |  ");
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
        printf(" %d |  ", i+1);
        // Print numbers row
        for (int j = 0; j < board->size; j++) {
            printf("%c", get_symbol_from_player(board->state[i][j]));
            if (j < board->size-1) {
                printf("---");
            }
        }
        printf("\n");
    }
    return;
}


// ***********
// Move functions

// Check if a move is valid with prints
bool is_valid_move_with_prints(Board* board, Move move) {
    // Check if the origin is valid
    if (!board_position_valid(board, move.origin)) {
        printf("\n** MOVIMENTO INVALIDO: Posicao de origem invalida. ");
        return false;
    }

    // Check if the destiny position is valid
    if (!board_position_valid(board, move.destiny)) {
        printf("\n** MOVIMENTO INVALIDO: Posicao de destino invalida. ");
        return false;
    }

    // Check if player is moving their own piece
    if (get_player(move.origin, board) != board->turn_player) {
        printf(
            "\n **MOVIMENTO INVALIDO: Nao ha peca do jogador na posicao "
            "escolhida. "
        );
        return false;
    }

    // Check if the origin is the same as the destiny
    if (positions_are_equal(move.origin, move.destiny)) {
        printf(
            "\n** MOVIMENTO INVALIDO: A posicao de origem e igual a "
            "posicao de destino. "
        );
        return false;
    }

    // Check if the new position is not adjacent to the old position
    if (!connected(board, move.origin, move.destiny)) {
        printf(
            "\n** MOVIMENTO INVALIDO: A posicao de destino nao e "
            "adjacente a posicao de origem. "
        );
        return false;
    }

    // Check if the chosen new position is valid
    if (get_player(move.destiny, board) != id_empty) {
        printf("\n** MOVIMENTO INVALIDO: Posicao ocupada. ");
        return false;
    }

    // If all checks passed, the move is valid
    return true;
}

// Check if a move is valid without prints
bool is_valid_move(Board* board, Move move) {
    // Check if the origin is valid
    if (!board_position_valid(board, move.origin)) {
        return false;
    }

    // Check if the destiny position is valid
    if (!board_position_valid(board, move.destiny)) {
        return false;
    }

    // Check if player is moving their own piece
    if (get_player(move.origin, board) != board->turn_player) {
        return false;
    }

    // Check if the origin is the same as the destiny
    if (positions_are_equal(move.origin, move.destiny)) {
        return false;
    }

    // Check if the origin is the same as the destiny
    if (positions_are_equal(move.origin, move.destiny)) {
        return false;
    }

    // Check if the new position is not adjacent to the old position
    if (!connected(board, move.origin, move.destiny)) {
        return false;
    }

    // Check if the chosen new position is valid
    if (get_player(move.destiny, board) != id_empty) {
        return false;
    }

    // If all checks passed, the move is valid
    return true;
}


// Add a move to the list of valid moves if it is valid
void add_move_if_valid(
    Move** valid_moves, 
    int* move_count, 
    Position origin,
    Position destiny,
    Board* board
) {
    // Initialize a new move
    Move* move = (Move*) malloc(sizeof(Move));
    move->origin = origin;
    move->destiny = destiny;

    // Check if the move is valid and add it to the valid moves
    if (is_valid_move(board, *move)) {
        valid_moves[*move_count] = move;
        (*move_count)++;
    } else {
        // Remove the move if it is not valid
        free(move);
    }
}

// Get the valid moves for the piece located on origin position
void add_moves_from_position(
    Move** valid_moves, 
    int* move_count, 
    Position origin,
    Board* board
) {
    Position destiny;
    
    // Possible moves are around the piece

    // i + 1, j (down)
    destiny.row = origin.row + 1;
    destiny.col = origin.col;
    add_move_if_valid(valid_moves, move_count, origin, destiny, board);
    
    // i - 1, j (up)
    destiny.row = origin.row - 1;
    destiny.col = origin.col;
    add_move_if_valid(valid_moves, move_count, origin, destiny, board);

    // i, j + 1 (right)
    destiny.row = origin.row;
    destiny.col = origin.col + 1;
    add_move_if_valid(valid_moves, move_count, origin, destiny, board);
    
    // i, j - 1 (left)
    destiny.row = origin.row;
    destiny.col = origin.col - 1;
    add_move_if_valid(valid_moves, move_count, origin, destiny, board);

    // i + 1, j + 1 (down-right)
    destiny.row = origin.row + 1;
    destiny.col = origin.col + 1;
    add_move_if_valid(valid_moves, move_count, origin, destiny, board);
    
    // i - 1, j - 1 (up-left)
    destiny.row = origin.row - 1;
    destiny.col = origin.col - 1;
    add_move_if_valid(valid_moves, move_count, origin, destiny, board);
    
    // i + 1, j - 1 (down-left)
    destiny.row = origin.row + 1;
    destiny.col = origin.col - 1;
    add_move_if_valid(valid_moves, move_count, origin, destiny, board);

    // i - 1, j + 1 (up-right)
    destiny.row = origin.row - 1;
    destiny.col = origin.col + 1;
    add_move_if_valid(valid_moves, move_count, origin, destiny, board);
}


// List all valid moves for a player
void list_valid_moves(
    Board* board, 
    Move** valid_moves, 
    int* move_count
) {
   
    // Allocate memory for the valid moves
    *move_count = 0;

    // Iterate through the board
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            // Check if the current position has a piece of the player
            if (board->state[i][j] == board->turn_player) {
            
                // Initialize a variable to store the move
                Position origin;
                origin.row = i;
                origin.col = j;

                // Add the valid moves from the current position
                add_moves_from_position(
                    valid_moves, 
                    move_count, 
                    origin, 
                    board
                );
            }
        }
    }

    return;
}


// Get the computer move by implementing a simple MIN-MAX algorithm
Move* get_computer_move(
    Board* board,
    int player_id,
    int computer_id
) {
    // Initialize the root state
    Board* board_copy = (Board*) malloc(size_of_board_struct());
    copy_board(board, board_copy);
    
    BoardState* root = create_board_state(board_copy, NULL);
    root->height = 0;

    // Set the turn player to the computer
    Move* best_move = get_best_move(root, player_id, computer_id);
    printf("Melhor movimento: (%d, %d) -> (%d, %d)\n",
        best_move->origin.row, best_move->origin.col,
        best_move->destiny.row, best_move->destiny.col
    );

    // Free the allocated memory for the root state and its children
    delete_board_state(root);

    // Return the best move found
    return best_move;
}

// Get the player's move from the user
Move* get_player_move(int player) {
    // Asks for the row and column of the piece to move
    printf(
        "Jogador %d, escolha uma peça (linha e coluna) para mover: ", 
        player
    );
    Position origin;
    scanf("%d", &(origin.row));
    scanf("%d", &(origin.col));
    // Asks for the row and column for the new position
    printf(
        "Jogador %d, escolha posicao (linha e coluna) para onde mover: ", 
        player
    );
    // Read the row and column from the user
    Position destiny;
    scanf("%d", &(destiny.row));
    scanf("%d", &(destiny.col));

    Move* move = malloc(sizeof(Move));
    move->origin = origin;
    move->destiny = destiny;

    return move;
}


void make_move(Board* board, Move move, bool* move_played) {
    // Make a move for the player
    if (is_valid_move_with_prints(board, move)) {
        set_player(move.destiny, board->turn_player, board);
        set_player(move.origin, id_empty, board);
        *move_played = true;
    } else {
        printf("Tente novamente.\n\n");
        *move_played = false;
    }
    return;
}

// ***********
// Board state functions


// Calculate the size of the board state
int size_of_board_state() {
    return (
        sizeof(Board*) + sizeof(BoardState*) + sizeof(BoardState**)
        + sizeof(int) * 4
    );
}


// Create a new BoardState
BoardState* create_board_state(Board* board, BoardState* parent) {
    // Allocate memory for the board state
    BoardState* new_state = (BoardState*) malloc(sizeof(BoardState));
    // Initialize the board
    new_state->board = board;
    
    // Set the parent node
    new_state->parent = parent;
    
    // Allocate memory for the children
    // The number of children is limited by the number of valid moves of
    // the turn player on the current state
    // The maximum number of moves is defined by MAX_MOVES
    new_state->children = (
        (BoardState**) malloc(sizeof(BoardState*) * MAX_MOVES)
    );
    for (int i = 0; i < MAX_MOVES; i++) {
        new_state->children[i] = NULL;
    }

    // Initialize the children
    new_state->num_children = 0;
    new_state->child_number = -1;
    
    // Initialize the height of the state as an invalid value
    new_state->height = -1;
    // Set the score of the state to a draw value and update it later
    new_state->score = 0;

    // Return the new state
    return new_state;
}


// Free the allocated memory for the board state
void delete_board_state(BoardState* state) {
    // Delete children recursively
    if (state->children != NULL) {
        for (int i = 0; i < state->num_children; i++) {
            if (state->children[i] != NULL) {
                delete_board_state(state->children[i]);
            }
        }
        free(state->children);
    }
    // Delete the board
    delete_board(state->board);

    // Free the allocated memory for the state
    free(state);
    return;
}


// Verify if a move is a winning move for the player
// Consider that the move is valid
bool is_winning_move(Board* board, Move* move) {

    // Set the player on the destiny position
    set_player(move->destiny, board->turn_player, board);
    set_player(move->origin, id_empty, board);

    // Check if the player has won
    bool winner = player_is_winner(board);

    // Undo the move
    set_player(move->origin, board->turn_player, board);
    set_player(move->destiny, id_empty, board);

    return winner;
}


// Verify if there is a winning move and returns it
// Returns NULL if no winning move is found
Move* get_winning_move(BoardState* state, Move** moves, int num_moves) {
    // Initialize variables to store the winning move
    bool win_found = false;
    int winning_move_pos = -1;
    
    // Search all moves for a winning move
    for (int i = 0; i < num_moves && !win_found; i++) {
        // Verify if a move is a winning move, and store the position if it is
        win_found = is_winning_move(state->board, moves[i]);
        if (win_found) {
            winning_move_pos = i; // Store the position of the winning move
        }
    }

    // Get the best move if a winning move was found and return it
    Move* best_move = (win_found) ? moves[winning_move_pos] : NULL;
    return best_move;
}


// Get the best move for the computer from the current state
// Return 0 if MAX_TREE_HEIGHT is reached
// Return 1 if wins from the current state
// Return -1 if results in loss
// Return a score representing the game result from the current state
int calculate_state_score(BoardState* state, int player, int computer_player) {
    // Get the valid moves for the computer
    if (state->height >= MAX_TREE_HEIGHT) {
        return 0; // Search limit reached
    }

    // Get all valid moves for the current player
    Move** moves = (Move**) malloc(sizeof(Move*) * MAX_MOVES);
    int num_moves = 0;
    
    list_valid_moves(
        state->board, 
        moves,
        &num_moves
    );

    // Verify if any move is a winning move
    Move* winning_move = get_winning_move(
        state, 
        moves, 
        num_moves
    );
    
    // If can win from this position
    if (winning_move != NULL) {
        // Free the allocated memory for the moves
        for (int i = 0; i < num_moves; i++) {
            free(moves[i]);
        }
        free(moves);
        
        double exp = ((double) MAX_TREE_HEIGHT) / ((double) state->height);
        int value = pow(9.0, exp); // Calculate the value of the winning move
        // Verify who is the player
        if (state->board->turn_player == computer_player) {
            return value; // Computer wins
        } else {
            return -value; // Player wins
        }
    }

    // If no winning move was found, make all moves
    // Sum winning, losing and not ended
    for (int i = 0; i < num_moves; i++) {
        // Create a new board state for the move
        Board* new_board = (Board*) malloc(size_of_board_struct());
        copy_board(state->board, new_board);
        
        // Create a new board state
        BoardState* new_state = create_board_state(new_board, state);
        new_state->height = state->height + 1;

        // Make the move
        set_player(moves[i]->destiny, player, new_board);
        set_player(moves[i]->origin, id_empty, new_board);

        // Check current player and switch turns
        if (new_board->turn_player == computer_player) {
            new_board->turn_player = player; // Switch to player turn
        } else {
            new_board->turn_player = computer_player; // Switch to computer turn
        }

        // Recursively check the game result from the new state
        int result = calculate_state_score(
            new_state, 
            player,
            computer_player
        );

        // Undo the move
        set_player(moves[i]->origin, player, new_board);
        set_player(moves[i]->destiny, id_empty, new_board);

        // Store the new state in the children
        state->children[state->num_children] = new_state;
        new_state->child_number = state->num_children;
        state->num_children++;
    }

    // Sum the results
    int children_total_score = 0;
    for (int i = 0; i < state->num_children; i++) {
        children_total_score += state->children[i]->score;
    }

    // Free the allocated memory for the moves
    for (int i = 0; i < num_moves; i++) {
        free(moves[i]);
    }
    free(moves);

    // Return the total result
    return children_total_score;
    
}


// Calculate the most profitable move for root
void calculate_root_children_score(
    BoardState* root,
    int player_id,
    int computer_id,
    Move** moves,
    int num_moves
) {
    // For each move, create a new board state and calculate the score
    for (int i = 0; i < num_moves; i++) {
        // Create a new board state for the move
        Board* new_board = (Board*) malloc(size_of_board_struct());
        copy_board(root->board, new_board);
        
        // Create a new board state
        BoardState* new_state = create_board_state(new_board, root);
        new_state->height = root->height + 1;

        // Make the move
        set_player(moves[i]->destiny, root->board->turn_player, new_board);
        set_player(moves[i]->origin, id_empty, new_board);

        // Calculate the game result from the new state
        int result = calculate_state_score(
            new_state, 
            player_id,
            computer_id
        );

        // Store the score in the new state
        new_state->score = result;

        // Store the new state in the children
        root->children[root->num_children] = new_state;
        new_state->child_number = root->num_children;
        root->num_children++;
    }

    return;
}

// Get the position of the move with the highest score
int get_move_with_highest_score_position(
    BoardState** children, 
    int num_children, 
    int computer_id
) {
 
    // Start with first score
    int best_score = -10000000; // A very low initial score
    int best_move_pos = -1;

    // Iterate through the children to find the best move
    for (int i = 0; i < num_children; i++) {
        // Get the score of the child state
        int score = children[i]->score;
        
        // If the score is better than the current best score, update it
        if (score > best_score) {
            best_score = score;
            best_move_pos = i;
        }
    }

    // Return the position of the best move
    return best_move_pos;
}


// Get the best move for the computer
Move* get_best_move(BoardState* root, int player_id, int computer_id) {
    // Get the valid moves for the computer
    Move** moves = (Move**) malloc(sizeof(Move*) * MAX_MOVES);
    int num_moves = 0;
    list_valid_moves(
        root->board, 
        moves,
        &num_moves
    );

    // Verify if there is a winning move
    Move* best_move = get_winning_move(root, moves, num_moves);
    if (best_move != NULL) {
        // If there is a winning move, return it
        return best_move;
    }

    // If no winning move was found, calculate each children score
    calculate_root_children_score(
        root, 
        player_id,
        computer_id,
        moves,
        num_moves
    );

    // Select the move with the best score
    int best_move_pos = get_move_with_highest_score_position(
        root->children, 
        root->num_children, 
        computer_id
    );
    best_move = moves[best_move_pos];

    // Free the allocated memory for the moves except the best move
    for (int i = 0; i < num_moves; i++) {
        if (i != best_move_pos) {
            free(moves[i]);
        }
    }

    // If there is a best move, return it
    return best_move;
}


// **********
// Game functions

// Check a win on a row
bool win_by_row(Board* board, int row) {
    // Check if all pieces in the row are the same as the turn player
    for (int j = 0; j < board->size; j++) {
        // If there is a position without the turn player, it is not a win
        if (board->state[row][j] != board->turn_player) {
            return false; 
        }
    }
    return true;
}


// Check a win on a column
bool win_by_column(Board* board, int col) {
    // Check if all pieces in the column are the same as the turn player
    for (int i = 0; i < board->size; i++) {
        // If there is a position without the turn player, it is not a win
        if (board->state[i][col] != board->turn_player) {
            return false; 
        }
    }
    // If all positions have the same player, it is a win
    return true;
}


// Check main diagonal
bool win_by_main_diagonal(Board* board) {
    // Check if all pieces in the main diagonal are the same as the turn player
    for (int i = 0; i < board->size; i++) {
        // If there is a position without the turn player, it is not a win
        if (board->state[i][i] != board->turn_player) {
            return false; 
        }
    }
    // If all positions have the same player, it is a win
    return true;
}


// Check anti diagonal
bool win_by_anti_diagonal(Board* board) {
    // Check if all pieces in the anti diagonal are the same as the turn player
    for (int i = 0; i < board->size; i++) {
        // If there is a position without the turn player, it is not a win
        if (board->state[i][board->size - 1 - i] != board->turn_player) {
            return false; 
        }
    }
    // If all positions have the same player, it is a win
    return true;
}


// Check a win on rows
bool is_winner_by_rows(Board* board) {
    // Check if player 1 wins on rows 1, 2
    // Check if player 2 wins on rows 0, 1
    int start = 0;
    int end = board->size - 1;

    // Check if the player is player 1 or player 2 to exclude the initial row
    if (board->turn_player == id_player_1) {
        end = board->size - 2;
    } else {
        start = 1;
    }
    
    // Check each row
    for (int i = start; i <= end; i++) {
        // If the player won on a row excluding initial, return true
        if (win_by_row(board, i)) {
            return true;
        }
    }
    // If no win is found, return false
    return false;
}


// Check a win on columns
bool is_winner_by_columns(Board* board) {
    // Check columns
    for (int j = 0; j < board->size; j++) {
        // If the player won on a column, return true
        if (win_by_column(board, j)) {
            return true;
        }
    }
    // If no win is found, return false
    return false;
}


// Check if the player has won on the diagonals
bool is_winner_by_diagonals(Board* board) {
    // Check main diagonal
    if (win_by_main_diagonal(board)) {
        return true;
    }
    // Check anti diagonal
    if (win_by_anti_diagonal(board)) {
        return true;
    }
    return false;
}


// Check for a winner
bool player_is_winner(Board* board) {
    // Check rows
    // Player 1 wins on rows 1, 2
    // Player 2 wins on rows 0, 1
    int start = 0;
    int end = board->size - 1;

    // Check rows
    if (is_winner_by_rows(board)) {
        return true;
    }

    // Check columns
    if (is_winner_by_columns(board)) {
        return true;
    }

    // Check diagonals
    if (is_winner_by_diagonals(board)) {
        return true;
    }

    // If no win is found, return false
    return false;
}


// Play a turn for the player
void play_user_turn(Board* board) {
    // Try to play a turn until a valid move is made
    bool turn_played = false;
    while (!turn_played) {
        // Get the player's move
        Position old_pos;
        Position new_pos;
        Move* move = get_player_move(board->turn_player);
        // Make the move if valid
        make_move(board, *move, &turn_played);
    }
    return;
}

void play_computer_turn(Board* board) {
    // Get the computer move
    Move* move = get_computer_move(
        board, 
        board->turn_player, 
        (board->turn_player == id_player_1) ? id_player_2 : id_player_1
    );

    // Make the move
    bool move_played = false;
    if (move != NULL) {
        make_move(board, *move, &move_played);
        free(move);
    } else {
        printf("Erro: Nenhum movimento valido encontrado.\n");
        exit(1);
    }
    return;
}


// Implement player vs player logic
void player_vs_player() {
    printf("Player vs Player\n");
    
    // Initialize board
    Board* board = create_board();

    // Show board
    print_board(board);
    
    // Initialize game related varibles
    bool winner_found = false;
    int num_rounds = MAX_ROUNDS;
    // While there is no winner and rounds are not over
    for (int i = 0; i < num_rounds && !winner_found; i++) {
        printf("Rodada %d\n", i + 1);
        // Player 1 plays on even rounds, Player 2 on odd rounds.
        board->turn_player = (i % 2 == 0) ? id_player_1 : id_player_2;
        printf(
            "Jogador %c, e sua vez de jogar.\n", 
            get_symbol_from_player(board->turn_player)
        );

        // Make the player's turn
        play_user_turn(board);
        
        // Check for winner
        if (player_is_winner(board)) {
            board->winner = board->turn_player;
            winner_found = true;
        }

        // Print the board after the turn
        print_board(board);
    }

    // Print the winner
    if (board->winner == id_empty) {
        printf("Empate!\n");
    } else {
        printf("Jogador %c venceu!\n", get_symbol_from_player(board->winner));
    }
    
    // Free the allocated memory for the board
    delete_board(board);
    return;
}


// Implement player vs computer logic
void player_vs_computer(bool player_starts) {
    printf("Player vs Computer\n");
    
    // Define player ids
    int player_id;
    int computer_id;

    // Set player ids based on who starts
    if (player_starts) {
        player_id = id_player_1;
        computer_id = id_player_2;
    } else {
        computer_id = id_player_1;
        player_id = id_player_2;
    }

    // Initialize board
    Board* board = create_board();

    // Show board
    print_board(board);
    
    // Initialize game related varibles
    bool winner_found = false;
    int num_rounds = MAX_ROUNDS;

    // While there is no winner and rounds are not over
    for (int i = 0; i < num_rounds && !winner_found; i++) {
        printf("Rodada %d\n", i + 1);
        // Player 1 plays on even rounds, Player 2 on odd rounds.
        
        board->turn_player = (i % 2 == 0) ? id_player_1 : id_player_2;

        bool is_player_turn = (board->turn_player == player_id);
        // Print the current player's turn
        if (is_player_turn) {
            printf(
                "Jogador %c, é sua vez de jogar.\n", 
                get_symbol_from_player(board->turn_player)
            );
        } else {
            printf("É a vez do computador.\n");
        }

        // Play the turn based on who is playing
        if (is_player_turn) {
            // Make the player's turn
            play_user_turn(board);
        } else {
            // Make the computer's turn
            play_computer_turn(board);
            sleep(3); // Sleep for 1 second to simulate computer thinking
        }
        // Check for winner
        if (player_is_winner(board)) {
            board->winner = board->turn_player;
            winner_found = true;
        }
        
        // Print the board after the turn
        print_board(board);
    }

    // Print the winner
    if (board->winner == id_empty) {
        printf("Empate!\n");
    } else {
        printf("Jogador %c venceu!\n", get_symbol_from_player(board->winner));
    }
    
    // Free the allocated memory for the board
    delete_board(board);
    return;
}


// Print the menu optionss
void print_menu() {
    printf("1. Jogador vs Jogador\n");
    printf("2. Jogador (P1) vs Computador (P2)\n");
    printf("3. Computador (P1) vs Jogador (P2)\n");
    printf("4. Sair\n");
    printf("Escolha uma opcao: ");
    return;
}


// Get the menu option from the user
int get_menu_option() {
    print_menu();
    int option;
    scanf("%d", &option);
    return option;
}


// Main function to run the game
int main() {
    // Print rules and welcome message
    bool valid_option;
    printf("Bem-vindo ao jogo do pe de galinha!\n");
    printf("O jogo consiste em 2 jogadores que jogam alternadamente.\n");
    printf("O objetivo do jogo e formar uma linha de 3 pecas.\n");
    printf(
        "O jogador 1 e representado pelo símbolo X "
        "e o jogador 2 pelo símbolo O.\n"
    );
    printf("O jogador 1 inicia o jogo.\n");

    // Main game loop
    do {
        // Get the game mode from the user
        int option = get_menu_option();
        switch (option) {
            // Player vs Player
            case 1:
                valid_option = true;
                player_vs_player();
                break;
            // Player vs Computer (Player 1 starts)
            case 2:
                valid_option = true;
                player_vs_computer(true);
                break;
            // Player vs Computer (Player 2 starts)
            case 3:
                valid_option = true;
                player_vs_computer(false);
                break;
            // Exit the game
            case 4:
                valid_option = true;
                break;
            // Invalid option
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