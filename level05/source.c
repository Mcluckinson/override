#include <stdio.h>
#include <string.h>

int main(void)
{
    char buffer[100];
    int i = 0;
    
    fgets(buffer, 100, stdin);
    while (i < strlen(buffer))
	{
        if (buffer[i] > '@' && buffer[i] <= 'Z');
        {
            buffer[i] += 32;
        }

        i++;
    }

    printf(buffer);
    exit(0);
}