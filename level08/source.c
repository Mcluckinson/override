#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void	log_wrapper(FILE *log_file, char *message, char *filename)
{
	char	log[264];
	strcpy(log, message);
	snprintf(&log[strlen(log)], 254 - strlen(log), filename);
	log[strcspn(log, "\n")] = 0;

	fprintf(log_file, "LOG: %s\n", log);
	return;
}

int	main(int argc, char **argv)
{
	FILE	*target_file;
	FILE	*log_file;
	char	filename[100];
	int		fd;
	int		symbol;

	if (argc != 2)
		printf("Usage: %s filename\n", argv[0]);

	log_file = fopen("./backups/.log", "w");
	if (log_file == 0)
	{
		printf("ERROR: Failed to open %s\n", "./backups/.log");
		exit(1);
	}
	log_wrapper(log_file, "Starting back up: ", argv[1]);

	target_file = fopen(argv[1], "r");
	if (target_file == 0)
	{
		printf("ERROR: Failed to open %s\n", argv[1]);
		exit(1);
	}

	strcpy(filename, "./backups/");
	strncat(filename, argv[1], 99 - (strlen(filename)) - 1);

	fd = open(filename, 0xc1, 0x1b0);
	if (fd < 0)
	{
		printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
		exit(1);
	}

	while ((symbol = fgetc(target_file)) != EOF)
	{
		write(fd, &symbol, 1);
	}

	log_wrapper(log_file, "Finished back up ", argv[1]);

	fclose(target_file);
	close(fd);

	return 0;
}
