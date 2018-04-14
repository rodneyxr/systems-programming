#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>


struct cmd
{
	int redirect_in;     /* Any stdin redirection?         */
	int redirect_out;    /* Any stdout redirection?        */
	int redirect_append; /* Append stdout redirection?     */
	int background;      /* Put process in background?     */
	int piping;          /* Pipe prog1 into prog2?         */
	char *infile;        /* Name of stdin redirect file    */
	char *outfile;       /* Name of stdout redirect file   */
	char *argv1[10];     /* First program to execute       */
	char *argv2[10];     /* Second program in pipe         */
};

int cmdscan(char *cmdbuf, struct cmd *com);
int plumber(struct cmd *cmd);
void sys_err(const char* msg);

int main(int nargs, char** argv) {
	char buf[1024];
	struct cmd command;
	pid_t cpid;
	printf("\n>> "); /* prompt */
	while( ( gets(buf) != NULL ) ){
		if (cmdscan(buf,&command)){
			printf("Illegal Format: \n");
			printf("\n>> "); /* prompt */
			continue;
		}
		if (strcmp(command.argv1[0], "exit") == 0) { break; }

		if (command.background) {
			switch (cpid = fork()) {
				case -1:
					sys_err("fork");
				case 0:
					plumber(&command);
					exit(0);
				default:
					waitpid(cpid, NULL, 0);
			}
		} else
			plumber(&command);

		printf("\n>> "); /* prompt */
	}
	return 0;
}

int plumber(struct cmd *cmd) {
	int fdin = STDIN_FILENO;
	int fdout = STDOUT_FILENO;
	pid_t cpid[2];
	switch (cpid[0] = fork()) {
		case -1:
			sys_err("fork");
		case 0:
			if (cmd->redirect_in) {
				fdin = open(cmd->infile, O_RDONLY);
				if (fdin == -1) sys_err(cmd->infile);
				dup2(fdin, STDIN_FILENO);
				close(fdin);
			}

			if (cmd->redirect_append) {
				fdout = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (fdout == -1) sys_err(cmd->outfile);
				dup2(fdout, STDOUT_FILENO);
				close(fdout);
			} else if (cmd->redirect_out) {
				fdout = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fdout == -1) sys_err(cmd->outfile);
				dup2(fdout, STDOUT_FILENO);
				close(fdout);
			}

			int pipefd[2];
			if (cmd->piping) {
				if (pipe(pipefd))
					sys_err("pipe");
				switch(cpid[1] = fork()) {
					case -1:
						sys_err("fork");
					case 0:
						close(pipefd[0]);
						dup2(pipefd[1], STDOUT_FILENO);
						execvp(cmd->argv1[0], cmd->argv1);	
					default:
						close(pipefd[1]);
						dup2(pipefd[0], STDIN_FILENO);
						waitpid(cpid[1], NULL, 0);
				}
			}

			if (cmd->piping)
				execvp(cmd->argv2[0], cmd->argv2);
			else
				execvp(cmd->argv1[0], cmd->argv1);
			sys_err("exec");
		default:
			if (!(cmd->background))
				waitpid(cpid[0], NULL, 0);
	}
	return 0;
}

void sys_err(const char* msg) {
	perror(msg);
	exit(-1);
}
