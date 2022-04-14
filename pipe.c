#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 3) {  // check number of args
		printf("too few args!\n");
	}

	int fd[2];
	pid_t childpid;

	if(pipe(fd) < 0)  // create pipe
		printf("Create Pipe Error!\n");

	if((childpid = fork()) == -1)  // create child process
	{
		perror("fork");
		exit(1);
	}

	if(childpid == 0)
	{
		/* Child process closes up input side of pipe */
		close(fd[0]);
		// open file for reading
		FILE *frptr = fopen(argv[1], "r");
		if(frptr == NULL)
		{
			printf("Load file %s Error!", argv[1]);
			exit(1);
		}
		char c;
		while( (c = fgetc(frptr)) != EOF ) {  // write to fd if data exist
			write(fd[1], &c, sizeof(c));
		}
		fclose(frptr);
	}
	else
	{
		/* Parent process closes up output side of pipe */
		close(fd[1]);
		// open file for writing
		FILE *fwptr = fopen(argv[2], "w");
		if(fwptr == NULL)
		{
			printf("Load file %s Error!", argv[2]);
			exit(1);
		}
		int len = 1;
		char buf;
		while((len = read(fd[0], &buf, sizeof(buf))) > 0) {  // read from fd if data exist
			fputc(buf, fwptr);
		}
		fclose(fwptr);
	}
	return 0;
}
