#include "microshell.h"
#include <stdio.h>

// #define __DEBUG__

int	N_CMDS = 0;

void	err(char *str)
{
	while (*str) {
		write(2, str, 1);
		str++;
	}
}

command	*count_and_set_cmds(int argc, char **argv)
{
	int	n_cmds_counter = 1;
	command *cmds;

	for (int i = 1; i < argc; i++) {
		if (!strcmp("|", argv[i]) || !strcmp(";", argv[i]))
			n_cmds_counter++;
	}
	cmds = malloc(sizeof(cmds) * n_cmds_counter);
	N_CMDS = n_cmds_counter;
	int	j = 0;
	int	is_cmd = 1;
	for (int i = 1; i < argc; i++)
	{
		if (is_cmd) {
			cmds[j].cmd = &argv[i];
			is_cmd = 0;
		}
		if (!strcmp("|", argv[i]) || !strcmp(";", argv[i])) {
			if (!strcmp("|", argv[i]))
				cmds[j].has_pipe = 1;
			argv[i] = NULL;
			j++;
			is_cmd = 1;
		}
	}
	return (cmds);
}

void	exec(command cmds, char **envp)
{
	int		status;
	pid_t	pid = fork();

	if (cmds.has_pipe){
		//open pipe and dup write side
	}
	if (execve(*cmds.cmd, cmds.cmd, envp)) {
		err("execve fail\n");
	}
	waitpid(pid, &status, -1);
	if (cmds.has_pipe){
		//close write side
		//open dup read side
	}
}

int	main(int argc, char *argv[], char **envp)
{
	command *cmds;

	cmds = count_and_set_cmds(argc, argv);

	exec(cmds[0], envp);
	#ifdef __DEBUG__
		for (int i = 0; i < N_CMDS; i++)
			printf("%s\n", *cmds[i].cmd);
	#endif
	//free cmds
}