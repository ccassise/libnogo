#include <stdlib.h>
#include <string.h>

#include "libnogo/nogo.h"

#include "task.h"

static void test_pro_parse_fail(void) {
	const char *inputs[] = {
		"     ",
		"\n\n\n\t\t\t\r\n",
		"\r\n",
		"TEST\r\n",
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_ERROR);
	}

}

static void test_pro_parse_join(void) {
	const char *inputs[] = {
		"JOIN\r\n",
		"   \t\t  JOIN\t\t\r\n",
		"JOIN",
		" JOIN\r\n",
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_JOIN);
	}
}

static void test_pro_parse_join_fail(void) {
	const char *inputs[] = {
		"JOIN oh no\r\n",
		"h JOIN\r\n",
		"JOIN 1 2\r\n",
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_ERROR);
	}
}

static void test_pro_parse_leave(void) {
	const char *inputs[] = {
		"LEAVE\r\n",
		"   \t\t  LEAVE\t\t\r\n",
		"LEAVE",
		" LEAVE\r\n"
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_LEAVE);
	}
}

static void test_pro_parse_leave_fail(void) {
	const char *inputs[] = {
		"LEAVE oh no\r\n",
		"h LEAVE\r\n",
		"LEAVE 1 2\r\n",
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_ERROR);
	}
}

static void test_pro_parse_move(void) {
	const char *inputs[] = {
		"MOVE 1 23\r\n",
		"MOVE 1 1",
		"MOVE 12345679 123746895\r\n",
		"\t\t\t\t\t MOVE \t\t\n\n 123\t456\r\n"
	};

	const char *expect[][2] = {
		{ "1", "23" },
		{ "1", "1" },
		{ "12345679", "123746895" },
		{ "123", "456" },
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_MOVE);
		ASSERT(strcmp(expect[i][0], actual.arg1) == 0);
		ASSERT(strcmp(expect[i][1], actual.arg2) == 0);
	}
}

static void test_pro_parse_move_fail(void) {
	const char *inputs[] = {
		"MOVE\r\n",
		"  MOVE  abc  ",
		"MOVE 123 ab\r\n",
		"MOVE a 1\r\n",
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_ERROR);
	}
}

static void test_pro_parse_login(void) {
	const char *inputs[] = {
		"LOGIN Alice\r\n",
		"LOGIN Bob",
		"\t\t\t\t\t LOGIN \t\t\n\n john\r\n",
		"  LOGIN  abc  ",
	};

	const char *expect[] = {
		"Alice",
		"Bob",
		"john",
		"abc",
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_LOGIN);
		ASSERT(strcmp(expect[i], actual.arg1) == 0);
	}
}

static void test_pro_parse_login_fail(void) {
	const char *inputs[] = {
		"LOGIN\r\n",
		"  LOGIN  abc  def",
		"LOGIN 123 ab\r\n",
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_ERROR);
	}
}

static void test_pro_arg_limit_test(void) {
	char *arg = malloc(sizeof *arg * (NOGO_PRO_ARG_SIZE));
	memset(arg, 'a', sizeof *arg * NOGO_PRO_ARG_SIZE);
	arg[NOGO_PRO_ARG_SIZE - 1] = '\0';

	for (size_t i = 0; i < NOGO_PRO_ARG_SIZE - 1; i++) {
		ASSERT(arg[i] == 'a');
	}
	ASSERT(arg[NOGO_PRO_ARG_SIZE - 1] == '\0');

	char input[256 + 1] = { '\0' };
	strncat(input, "LOGIN ", 256);
	strncat(input, arg, 256);
	
	NogoProtocol actual = nogo_parse(input, strlen(input));

	ASSERT(actual.type == NOGO_PRO_LOGIN);
	ASSERT(strlen(actual.arg1) == NOGO_PRO_ARG_SIZE - 1);

	free(arg);
}

static void test_pro_parse_logout(void) {
	const char *inputs[] = {
		"LOGOUT\r\n",
		"   \t\t  LOGOUT\t\t\r\n",
		"LOGOUT",
		" LOGOUT\r\n",
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_LOGOUT);
	}
}

static void test_pro_parse_logout_fail(void) {
	const char *inputs[] = {
		"LOGOUT please\r\n",
		"LOGOUTL\r\n",
		"LLOGOUT\r\n",
		"EXIT\r\n",
		"LOGOUTLOGOUTLOGOUTLOGOUT\r\n",
	};

	const size_t inputs_len = sizeof inputs / sizeof *inputs;

	for (size_t i = 0; i < inputs_len; i++) {
		NogoProtocol actual = nogo_parse(inputs[i], strlen(inputs[i]));

		ASSERT(actual.type == NOGO_PRO_ERROR);
	}
}


int main(void) {
	test_pro_parse_fail();
	test_pro_parse_join();
	test_pro_parse_join_fail();
	test_pro_parse_leave();
	test_pro_parse_leave_fail();
	test_pro_parse_move();
	test_pro_parse_move_fail();
	test_pro_parse_login();
	test_pro_parse_login_fail();
	test_pro_arg_limit_test();
	test_pro_parse_logout();
	test_pro_parse_logout_fail();
}
