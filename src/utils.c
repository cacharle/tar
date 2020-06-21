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

int	header_write(int fd, char *file_name, struct stat *statbuf)
{
	t_header		header;
	struct group	*group;
	struct passwd	*passwd;
	unsigned int	sum;

	bzero(&header, sizeof(t_header));
	strncpy(header.file_name, file_name, sizeof(header.file_name));
	if (S_ISDIR(statbuf->st_mode))
		strncat(header.file_name, "/", sizeof(header.file_name) - 1);
	sprintf(header.file_mode, "%07o", statbuf->st_mode & 0777);
	sprintf(header.user_id,   "%07o", statbuf->st_uid);
	sprintf(header.group_id,  "%07o", statbuf->st_gid);

	if (S_ISDIR(statbuf->st_mode))
		sprintf(header.file_size, "%011lo", 0lu);
	else
		sprintf(header.file_size, "%011lo", statbuf->st_size);

	sprintf(header.last_time, "%011lo", statbuf->st_mtime);
	memset(header.checksum, ' ', sizeof(header.checksum));

	switch (statbuf->st_mode & S_IFMT)
	{
		case S_IFREG:  header.file_type[0] = '0'; break;
		/* case S_IFLNK:  header.file_type[0] = '0' break; */
		/* case S_IFBLK:  header.file_type[0] = '0' break; */
		/* case S_IFCHR:  header.file_type[0] = '0' break; */
		case S_IFDIR:  header.file_type[0] = '5'; break;
		/* case S_IFIFO:  header.file_type[0] = '0' break; */
		/* case S_IFSOCK: header.file_type[0] = '0' break; */
	}

	memcpy(header.ustar, "ustar ", sizeof(header.ustar));
	memcpy(header.ustar_version, " ", sizeof(header.ustar_version));

	if ((passwd = getpwuid(statbuf->st_gid)) == NULL)
	{
		perror("header_write getpwduid");
		return -1;
	}
	strncpy(header.user_name, passwd->pw_name, sizeof(header.user_name));
	if ((group = getgrgid(statbuf->st_gid)) == NULL)
	{
		perror("header_write getgrgid");
		return -1;
	}
	strncpy(header.group_name, group->gr_name, sizeof(header.group_name));

	sum = 0;
	for (size_t i = 0; i < sizeof(t_header); i++)
		sum += ((uint8_t*)&header)[i];
	sprintf(header.checksum,  "%06o", sum);

	return record_write(fd, (char*)&header, sizeof(t_header));
}

int header_parse(char record[RECORD_SIZE])
{
	t_header	header;
	mode_t		mode;
	gid_t		gid;
	uid_t		uid;

	memcpy(&header, record, sizeof(t_header));
	/* sscanf(header.file_mode, "% */

	/* sscanf(header.file_mode, "%07o", statbuf->st_mode); */
	/* sscanf(header.user_id,   "%07o", statbuf->st_uid); */
	/* sscanf(header.group_id,  "%07o", statbuf->st_gid); */
    /*  */
	/* sscanf(header.file_size, "%011lo", statbuf->st_size); */
    /*  */
	/* sscanf(header.last_time, "%011lo", statbuf->st_mtime); */
	return 0;
}
