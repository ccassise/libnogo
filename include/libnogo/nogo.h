#ifndef NOGO_H_
#define NOGO_H_

#include <stddef.h>

#define NOGO_BOARD_EMPTY_SPACE '\0'

/**
 * @brief Represents a board object. A board is a square where the top left
 * corner is 0,0 (row, then column).
 * 
 */
typedef struct NogoBoard {
	size_t rows;
	size_t cols;
	char *board;
} NogoBoard;

/**
 * @brief A position on the board.
 * 
 */
typedef struct NogoBoardPos {
	size_t row;
	size_t col;
} NogoBoardPos;

/**
 * @brief Create a board object. 
 * The user must call nogo_board_free on the returned NogoBoard when done.
 * 
 * @param rows Number of rows the board will have.
 * @param cols Number of columns a board will have.
 * @return * NogoBoard The newly created board object.
 */
NogoBoard *nogo_board_create(size_t rows, size_t cols);

/**
 * @brief Free the memory that was used by board.
 * 
 * @param b The board to free.
 */
void nogo_board_free(NogoBoard *b);

/**
 * @brief Get the char at the given board position.
 * 
 * @param b The board from which to get the piece.
 * @param pos The board position to get the character from.
 * @return char The character at the given position.
 */
char nogo_board_get(NogoBoard *b, NogoBoardPos pos);

/**
 * @brief Places a given piece onto the board at given position.
 * 
 * @param NogoBoard The board in which to place a piece.
 * @param piece The piece represented as a character to place.
 * @param pos The position where to place the piece.
 */
void nogo_board_set(NogoBoard *b, char piece, NogoBoardPos pos);

/**
 * @brief Returns a string representation of the board. Allocates memory for the
 * string and can be freed with free.
 * 
 * @param b The board the stringify.
 * @return char* The string representation of a board. Blank spaces will be displayed as a dot.
 */
char *nogo_board_str(NogoBoard *b);

#define NOGO_PRO_ARG_SIZE 32

typedef enum {
	NOGO_PRO_ERROR,
	NOGO_PRO_GOTMOVE,
	NOGO_PRO_JOIN,
	NOGO_PRO_LEAVE,
	NOGO_PRO_LOGIN,
	NOGO_PRO_LOGOUT,
	NOGO_PRO_MOVE,
	NOGO_PRO_OK,
} NogoProtocolType;

typedef struct NogoProtocol {
	char arg1[NOGO_PRO_ARG_SIZE];
	char arg2[NOGO_PRO_ARG_SIZE];

	NogoProtocolType type;
} NogoProtocol;

/**
 * @brief Parses the given buffer and returns the parsed command and arguments
 * if any. If there are no arguments for the parsed command then the returned
 * struct will contain an empty string for the missing arg.
 * 
 * @param buf The buffer to parse.
 * @param buf_len The amount of bytes in the buffer.
 * @return NogoProtocol
 */
NogoProtocol nogo_parse(const char *buf, size_t buf_len);

#endif
