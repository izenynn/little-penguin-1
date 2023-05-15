#include "miniunit.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define TEST_DEVICE "/dev/reverse"
#define MAX_BUFFER_SIZE 4096 // Assuming PAGE_SIZE is 4096
#define OVERFLOW_STR_SIZE (MAX_BUFFER_SIZE + 10)

TEST_CASE(test_write_to_device) {
	char test_str[] = "Hello, World!";
	ssize_t bytes_written;

	int fd = open(TEST_DEVICE, O_WRONLY);
	ASSERT(fd >= 0, "Failed to open device for writing");

	bytes_written = write(fd, test_str, strlen(test_str));
	ASSERT(bytes_written == strlen(test_str), "Mismatch in number of written bytes");

	close(fd);
}

TEST_CASE(test_read_from_device) {
	char buffer[MAX_BUFFER_SIZE] = {0};
	ssize_t bytes_read;

	int fd = open(TEST_DEVICE, O_RDONLY);
	ASSERT(fd >= 0, "Failed to open device for reading");

	bytes_read = read(fd, buffer, MAX_BUFFER_SIZE - 1);
	ASSERT(bytes_read >= 0, "Failed to read from device");

	close(fd);
}

TEST_CASE(test_reverse_string) {
	char test_str[] = "Hello, World!";
	char reverse_str[] = "!dlroW ,olleH";
	char buffer[MAX_BUFFER_SIZE] = {0};

	int fd = open(TEST_DEVICE, O_RDWR);
	ASSERT(fd >= 0, "Failed to open device");

	write(fd, test_str, strlen(test_str));
	lseek(fd, 0, SEEK_SET);
	read(fd, buffer, MAX_BUFFER_SIZE - 1);

	ASSERT(strcmp(buffer, reverse_str) == 0, "String not reversed correctly");

	close(fd);
}

TEST_CASE(test_overflow) {
	char overflow_str[OVERFLOW_STR_SIZE];
	memset(overflow_str, 'A', OVERFLOW_STR_SIZE);
	ssize_t bytes_written;

	int fd = open(TEST_DEVICE, O_WRONLY);
	ASSERT(fd >= 0, "Failed to open device for writing");

	bytes_written = write(fd, overflow_str, OVERFLOW_STR_SIZE);
	ASSERT(bytes_written == -1, "Overflow condition not detected");

	close(fd);
}

TEST_CASE(test_write_and_read)
{
	int fd = open(TEST_DEVICE, O_RDWR);
	ASSERT(fd >= 0, "Failed to open device");

	const char *write_str1 = "Hello, ";
	const char *write_str2 = "World! ";
	const char *write_str3 = "This is a test.";
	char read_buffer[50] = {0}; // Make sure this is large enough

	ssize_t write_result = write(fd, write_str1, strlen(write_str1));
	ASSERT(write_result == strlen(write_str1), "Failed to write string 1");

	write_result = write(fd, write_str2, strlen(write_str2));
	ASSERT(write_result == strlen(write_str2), "Failed to write string 2");

	write_result = write(fd, write_str3, strlen(write_str3));
	ASSERT(write_result == strlen(write_str3), "Failed to write string 3");

	// Reset the file offset to the beginning of the file
	off_t lseek_result = lseek(fd, 0, SEEK_SET);
	ASSERT(lseek_result != (off_t)-1, "Failed to reset file offset");

	ssize_t read_result = read(fd, read_buffer, sizeof(read_buffer) - 1);
	ASSERT(read_result >= 0, "Failed to read from device");

	// The read buffer should contain the written strings in reverse
	const char *expected_result = ".tset a si sihT !dlroW ,olleH";
	ASSERT(strcmp(read_buffer, expected_result) == 0, "Read buffer does not match expected result");

	close(fd);
}


TEST_MAIN {
	RUN_TEST(test_write_to_device);
	RUN_TEST(test_read_from_device);
	RUN_TEST(test_reverse_string);
	RUN_TEST(test_overflow);
	RUN_TEST(test_write_and_read);

	END();
}
