#include <stdlib.h>
#include <string.h>

#include "libnogo/nogo.h"

#include "task.h"

static void test_board_create(void) {
	const size_t rows = 3;
	const size_t cols = 5;

	NogoBoard *b = nogo_board_create(rows, cols);

	for (size_t i = 0; i < rows * cols; i++) {
		assert(b->board[i] == NOGO_BOARD_EMPTY_SPACE);
	}

	nogo_board_free(b);
}

static void test_board_set(void) {
	NogoBoard *b = nogo_board_create(3, 5);

	nogo_board_set(b, 'O', (NogoBoardPos){ .row = 0, .col = 3 });
	nogo_board_set(b, 'X', (NogoBoardPos){ .row = 1, .col = 2 });
	nogo_board_set(b, 'O', (NogoBoardPos){ .row = 2, .col = 4 });

	char *actual = nogo_board_str(b);
	const char *expect = (
		". . . O .\n"
		". . X . .\n"
		". . . . O"
	);

	ASSERT(strcmp(actual, expect) == 0);

	free(actual);
	nogo_board_free(b);
}

static void test_board_get(void) {
	NogoBoard *b = nogo_board_create(3, 5);

	nogo_board_set(b, 'O', (NogoBoardPos){ .row = 0, .col = 3 });
	nogo_board_set(b, 'X', (NogoBoardPos){ .row = 1, .col = 2 });
	nogo_board_set(b, 'O', (NogoBoardPos){ .row = 2, .col = 4 });

	ASSERT(nogo_board_get(b, (NogoBoardPos){ .row = 0, .col = 0 }) == NOGO_BOARD_EMPTY_SPACE);
	ASSERT(nogo_board_get(b, (NogoBoardPos){ .row = 0, .col = 3 }) == 'O');
	ASSERT(nogo_board_get(b, (NogoBoardPos){ .row = 1, .col = 2 }) == 'X');
	ASSERT(nogo_board_get(b, (NogoBoardPos){ .row = 2, .col = 4 }) == 'O');

	nogo_board_free(b);
}

int main(void) {
	test_board_create();
	test_board_set();
	test_board_get();
}
