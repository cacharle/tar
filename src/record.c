#include "tar.h"

int	record_write(int fd, char *s, size_t size)
{
	char buf[RECORD_SIZE];

	while (size > RECORD_SIZE)
	{
		memcpy(buf, s, RECORD_SIZE);
		s += RECORD_SIZE;
		size -= RECORD_SIZE;
		if (write(fd, buf, RECORD_SIZE) == -1)
		{
			perror(NULL);
			return -1;
		}
	}
	bzero(buf, RECORD_SIZE);
	memcpy(buf, s, size);
	if (write(fd, buf, RECORD_SIZE) == -1)
	{
		perror(NULL);
		return -1;
	}
	return (0);
}

int	record_write_blank(int fd, size_t count)
{
	char buf[RECORD_SIZE] = {0};
	while (count-- > 0)
	{
		if (write(fd, buf, RECORD_SIZE) == -1)
		{
			perror(NULL);
			return -1;
		}
	}
	return 0;
}

int	record_read(int fd, char record[RECORD_SIZE])
{
	int ret = read(fd, record, RECORD_SIZE);
	if (ret == -1)
	{
		perror(NULL);
		return -1;
	}
	if (ret != RECORD_SIZE)
		return -1;
	return 0;
}

static char		g_record_blank[RECORD_SIZE] = {0};

bool	record_is_blank(char record[RECORD_SIZE])
{
	return memcmp(record, g_record_blank, RECORD_SIZE) == 0;
}
