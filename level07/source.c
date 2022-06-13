#include <stdio.h>
#include <string.h>

void	clear_stdin()
{
	char	x = 0;

	while (1)
	{
		x = getchar();
		if (x == '\n' || x == EOF)
			break;
	}
}

unsigned int	get_unum()
{
	unsigned int	input;

	fflush(stdout);
	scanf("%u", &input);
	clear_stdin();

	return input;
}

int	store_number(int *data_table)
{
	unsigned int	input = 0;
	unsigned int	index = 0;

	printf(" Number: ");
	input = get_unum();

	printf(" Index: ");
	index = get_unum();

	if (index % 3 == 0 || (input >> 24) == 183)
	{
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");

		return 1;
	}

	data_table[index] = input;

	return 0;
}

int	read_number(int *data_table)
{
	unsigned int	input = 0;

	printf(" Index: ");
	input = get_unum();

	printf(" Number at data[%u] is %u\n", input, data_table[input]);
	
	return 0;
}

int	main(int argc, char **argv, char **env)
{
	int		ret = 0;
	char	buffer[20] = {0};
	int		data_table[100] = {0};

	for (int i = 0; argv[i] != 0; i++)
		memset(argv[i], 0, strlen(argv[i]) - 1);
	for (int i = 0; env[i] != 0; i++)
		memset(env[i], 0, strlen(env[i] - 1));

	puts("----------------------------------------------------\n"\
		 "  Welcome to wil's crappy number storage service!   \n"\
		 "----------------------------------------------------\n"\
		 " Commands:                                          \n"\
		 "    store - store a number into the data storage    \n"\
		 "    read  - read a number from the data storage     \n"\
		 "    quit  - exit the program                        \n"\
		 "----------------------------------------------------\n"\
		 "   wil has reserved some storage :>                 \n"\
		 "----------------------------------------------------\n"\
		 "\n");

	while (1)
	{
		printf("Input command: ");
		fgets(buffer, 20, stdin);
		
		buffer[strlen(buffer) - 1] = 0;

		ret = 1;
		if (strncmp("store", buffer, 5) == 0)
			ret = store_number(data_table);
		else if (strncmp("read", buffer, 4) == 0)
			ret = read_number(data_table);
		else if (strncmp("quit", buffer, 4) == 0)
			return (0);
		if (ret != 0)
			printf(" Failed to do %s command\n", buffer);
		else
			printf(" Completed %s command successfully\n", buffer);

		bzero(buffer, 20);
	}

	return 0;
}
