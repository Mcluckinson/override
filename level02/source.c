#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(int argc, char **argv)
{
	char	username[100];
	char	password[48];
	char	user_pass[112];
	int		pass_len;
	FILE	*file;

	file = fopen("/home/users/level03/.pass", "r");
	if (!file)
	{
		fwrite("ERROR: failed to open password file\n", 1, 36, stderr);	
		exit(1);
	}

	pass_len = fread(&password, 1, 41, file);
	password[strcspn(password, "\n")] = 0;
	if (pass_len != 41)
	{
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
	
		exit(1);
	}
	fclose(file);

	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\\**************************************/");

	printf("--[ Username: ");
	fgets(username, 100, stdin);
	username[strcspn(username, "\n")] = 0;
	
	printf("--[ Password: ");
	fgets(user_pass, 100, stdin);
	user_pass[strcspn(user_pass, "\n")] = 0;

	puts("*****************************************");

	if (strncmp(password, user_pass, 41))
	{
		printf(username);
		puts(" does not have access!");
	
		exit(1);
	}

	printf("Greetings, %s!\n", username);
	system("/bin/sh");
	return 0;
}
