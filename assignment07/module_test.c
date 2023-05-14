#include "miniunit.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>

#define DEVICE_PATH "/sys/kernel/debug/fortytwo/id"
#define TEST_STUDENT_LOGIN "dpoveda"
#define BAD_STUDENT_LOGIN "badlogin"
#define OVERSIZE_LOGIN "dpovedadpovedadpovedadpovedadpoveda"

int fd = -1;

TEST_CASE(open_device) {
	fd = open(DEVICE_PATH, O_RDWR);
	ASSERT(fd != -1, "Failed to open device");
}

TEST_CASE(write_correct_login) {
	ssize_t bytes_written = write(fd, TEST_STUDENT_LOGIN, strlen(TEST_STUDENT_LOGIN));
	ASSERT(bytes_written == (ssize_t)strlen(TEST_STUDENT_LOGIN), "Failed to write correct student login");
}

TEST_CASE(write_incorrect_login) {
	errno = 0; // Reset errno
	ssize_t bytes_written = write(fd, BAD_STUDENT_LOGIN, strlen(BAD_STUDENT_LOGIN));
	ASSERT(bytes_written == -1 && errno == EINVAL, "Incorrect student login did not return -1 or errno is not EINVAL");
}

TEST_CASE(write_oversize_login) {
	errno = 0; // Reset errno
	ssize_t bytes_written = write(fd, OVERSIZE_LOGIN, strlen(OVERSIZE_LOGIN));
	ASSERT(bytes_written == -1 && errno == EINVAL, "Oversize student login did not return -1 or errno is not EINVAL");
}

TEST_CASE(read_login) {
	char buffer[10];
	lseek(fd, 0, SEEK_SET); // Reset file position
	ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	buffer[bytes_read] = '\0'; // Null-terminate the string
	ASSERT(strcmp(buffer, TEST_STUDENT_LOGIN) == 0, "Read student login does not match expected login");
}

TEST_CASE(read_past_end) {
	char buffer[10];
	lseek(fd, 0, SEEK_SET); // Reset file position
	ssize_t bytes_read = read(fd, buffer, sizeof(TEST_STUDENT_LOGIN)); // Read exactly the student login
	bytes_read = read(fd, buffer, 1); // Try to read one more byte
	ASSERT(bytes_read == 0, "Reading past end did not return 0");
}

TEST_CASE(llseek_set) {
	off_t res = lseek(fd, 0, SEEK_SET); // Seek to the beginning
	ASSERT(res == 0, "Failed to seek to the beginning");
}

TEST_CASE(llseek_cur) {
	off_t res = lseek(fd, 1, SEEK_CUR); // Seek one byte forward from the current position
	ASSERT(res == 1, "Failed to seek one byte forward");
}

TEST_CASE(llseek_end) {
	off_t res = lseek(fd, -1, SEEK_END); // Seek to the last byte
	ASSERT(res == (off_t)strlen(TEST_STUDENT_LOGIN) - 1, "Failed to seek to the last byte");
}

TEST_CASE(llseek_past_end) {
	errno = 0; // Reset errno
	off_t res = lseek(fd, 100, SEEK_SET); // Try to seek past the end of the student login
	ASSERT(res == -1 && errno == EINVAL, "Seeking past end did not return -1 or errno is not EINVAL");
}

TEST_CASE(close_device) {
	int res = close(fd);
	ASSERT(res == 0, "Failed to close device");
}

TEST_MAIN {
	// Open tests
	RUN_TEST(open_device);

	// Write tests
	RUN_TEST(write_correct_login);
	RUN_TEST(write_incorrect_login);
	RUN_TEST(write_oversize_login);

	// Read tests
	RUN_TEST(read_login);
	RUN_TEST(read_past_end);

	// llseek tests
	RUN_TEST(llseek_set);
	RUN_TEST(llseek_cur);
	RUN_TEST(llseek_end);
	RUN_TEST(llseek_past_end);

	// Close tests
	RUN_TEST(close_device);

	END();
}

