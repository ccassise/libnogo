#include <stdlib.h>
#include <string.h>

#include "libnogo/nogo.h"

/**
 * @brief Gets the 1D index from a position.
 * 
 * @param b The board instance.
 * @param pos The position to convert.
 * @return size_t The array index of the given position.
 */
static size_t pos_index(size_t cols, NogoBoardPos pos) {
	return pos.row * cols + pos.col; 
}

NogoBoard *nogo_board_create(size_t rows, size_t cols) {
	NogoBoard *b = malloc(sizeof *b);
	b->rows = rows;
	b->cols = cols;

	b->board = calloc(b->rows * b->cols, sizeof *b->board);

	return b;
}

void nogo_board_free(NogoBoard *b) {
	free(b->board);
	free(b);
}

char nogo_board_get(NogoBoard *b, NogoBoardPos pos) {
	return b->board[pos_index(b->cols, pos)];
}

void nogo_board_set(NogoBoard *b, char piece, NogoBoardPos pos) {
	b->board[pos_index(b->cols, pos)] = piece;
}

char *nogo_board_str(NogoBoard *b) {
	// Allocate enough space so that each position on the board can be separated
	// by a space and each line can have a newline. The string will have a
	// terminating null character where the last newline would be.
	const size_t len = (b->rows * b->cols * 2);
	char *result = malloc((sizeof *result) * len);

	for (size_t i = 0, j = 0; i < b->rows * b->cols; i++) {
		if (b->board[i] == NOGO_BOARD_EMPTY_SPACE) {
			result[j++] = '.';
		} else {
			result[j++] = b->board[i];
		}

		if (i % b->cols == b->cols - 1) {
			result[j++] = '\n';
		} else {
			result[j++] = ' ';
		}
	}

	result[len - 1] = '\0';

	return result;
}
