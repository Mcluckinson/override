#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <linux/ptrace.h>

int	main(void)
{
	int		child_pid = fork();
	
	char	buffer[32];
	int		ptrace_ret = 0;
	int		status = 0;

	if (child_pid == 0)
	{
		prctl(1, 1);
		ptrace(0, 0, NULL, NULL);
		puts("Give me some shellcode, k");
		gets(buffer);

		return 0;
	}

	while (ptrace_ret = ptrace(PTRACE_PEEKUSR, child_pid, 44, NULL) != 11)
	{
		wait(&status);

		if (WIFEXITED(status) || WIFSIGNALED(status))
		{
			puts("child is exiting...");
			return 0;
		}
	}

	puts("no exec() for you");
	kill(child_pid, 9);

	return 0;
}
