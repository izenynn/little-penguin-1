/*
 * miniuint.h
 * by: izenynn<me@izenynn.com>
 * commit: 67c2f3a9ce778b6bedacded8d7ab9396f47152d4
 */

#ifndef MINIUNIT_MINIUNIT_H_
#define MINIUNIT_MINIUNIT_H_

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define C_OFF	"\033[0m"
#define C_GREEN	"\033[0;32m"
#define C_RED	"\033[0;31m"

/*
 * __FILE__ is part of the C PreProcessor standard... but who knows.
 * also, this way you can also change the FILENAME displayed on the output just
 * be defining FILENAME on the command line with `-DFILENAME=...`
 */
#ifndef FILENAME
#define FILENAME ((__FILE__) ? (__FILE__) : ("unknown file"))
#endif // FILENAME

/*
 * If you define this in your own Makefile, make sure its declared as a string,
 * the quotes are very important:
 * CFLAGS += -DRELATIVE_PATH="\"$(RELATIVE_PATH)\""
 */
#ifndef RELATIVE_PATH
#define RELATIVE_PATH_LEN (int)0
#else
#define RELATIVE_PATH_LEN ((int)strlen(RELATIVE_PATH) + 1)
#endif // RELATIVE_PATH

static struct {
	unsigned int	total_tests;
	unsigned int	passed_tests;
	unsigned int	failed_tests;
	bool		current_test_success;
	const char*	current_test_name;
} test_stats = { 0, 0, 0, true, NULL };

#define ASSERT(test, message) do {					\
		if (!(test)) {						\
			printf(C_RED "FAIL" C_OFF ": %s: %s\n",		\
			       test_stats.current_test_name,		\
			       message);				\
			test_stats.current_test_success = false;	\
		}							\
} while (0)

#define RUN_TEST(test) do {						\
		++test_stats.total_tests;				\
		test_stats.current_test_success = true;			\
		test_stats.current_test_name = #test;			\
		test();							\
		if (test_stats.current_test_success)			\
			++test_stats.passed_tests;			\
		else							\
			++test_stats.failed_tests;			\
} while (0)


/*
 * For the actual width we'll substract:
 * - the __FILE__ len (with RELATIVE_PATH substracted)
 * - 2 for the spaces on the sides
 * - 2 for the "ok/ko" text
 */
#define WIDTH (int)50
#define END() do {							\
	char* ptr = FILENAME;						\
	int file_width =						\
		(int)strlen(ptr) - RELATIVE_PATH_LEN >= WIDTH - 4	\
		? WIDTH - 4						\
		: (int)strlen(ptr) + RELATIVE_PATH_LEN;			\
	int dots_width =						\
		(int)strlen(ptr) - RELATIVE_PATH_LEN >= WIDTH - 4	\
		? 0							\
		: WIDTH - (int)strlen(ptr) + RELATIVE_PATH_LEN - 4;	\
	printf("%.*s %.*s",						\
	      file_width,						\
	      ptr + RELATIVE_PATH_LEN,					\
	      dots_width,						\
	      "..................................................");	\
	if (test_stats.failed_tests == 0)				\
		printf(" " C_GREEN "ok" C_OFF);				\
	else								\
		printf(" " C_RED "ko" C_OFF);				\
	printf("     (" C_GREEN "%d" C_OFF "/" C_RED "%d" C_OFF ")\n",	\
	      test_stats.passed_tests,					\
	      test_stats.failed_tests);					\
	return ((test_stats.failed_tests == 0) ? 0 : 1);		\
} while (0)

#define TEST_CASE(test) \
	static void test(void)

#define TEST_MAIN \
	int main(void)

#endif // MINIUNIT_MINIUNIT_H_
