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
	write(2, "\n", 1);
	exit (1);
}

command	*count_and_set_cmds(int argc, char **argv)
{
	int	n_cmds_counter = 1;
	command *cmds;

	for (int i = 1; i < argc; i++) {
		if (!strcmp("|", argv[i]) || !strcmp(";", argv[i]))
			n_cmds_counter++;
	}
	cmds = malloc(sizeof(command) * n_cmds_counter);
	N_CMDS = n_cmds_counter;
	int	j = 0;
	int	is_cmd = 1;
	for (int i = 1; i < argc; i++) {
		if (is_cmd) {
			cmds[j].cmd = &argv[i];
			cmds[j].has_pipe = 0;
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

int cd(char **cmd)
{
	int	i = 1;
	while (cmd[i])
		i++;
	if (i != 2)
		err("error: cd: bad arguments");
#ifdef __DEBUG__
	printf(" ====== CD =====\n");
	printf("old: %s\n", get_current_dir_name());
#endif
	if (chdir(cmd[1]) == -1)
		err("error: cd: cannot change directory to ");
#ifdef __DEBUG__
	printf("new: %s\n", get_current_dir_name());
	printf(" ====== =====\n");
#endif
	return 0;
}

void	exec(command cmds, char **envp)
{
	int		status;
	int		pipes[2];

	if (!cmds.has_pipe && !strcmp(*cmds.cmd, "cd") && !cd(cmds.cmd))
		return ;

	if (cmds.has_pipe) {
		if (pipe(pipes))
			err("pipe fail");
	}
	pid_t	pid = fork();
	if (!pid)
	{
		if (cmds.has_pipe) {
			dup2(pipes[1], 1);
			close(pipes[1]);
			close(pipes[0]);
		}
		if (!strcmp(*cmds.cmd, "cd") && !cd(cmds.cmd))
			return ;
		if (execve(*cmds.cmd, cmds.cmd, envp)){
		#ifdef __DEBUG__
			printf("execve: %s\n", *cmds.cmd);
		#endif
			err("execve fail");
			exit (1);
		}
	}
	else
		waitpid(pid, &status, 0);
	if (cmds.has_pipe) {
		dup2(pipes[0], 0);
		close(pipes[1]);
		close(pipes[0]);
	}
}

int	main(int argc, char *argv[], char **envp)
{
	command *cmds;

	cmds = count_and_set_cmds(argc, argv);

	for (int i = 0; i < N_CMDS; i++)
		exec(cmds[i], envp);
#ifdef __DEBUG__
	printf(" ====== CMDS DATA =====\n");
	for (int i = 0; i < N_CMDS; i++) {
		int j = 0;
			printf("%d) ", i+1);
		while (cmds[i].cmd[j]) {
			printf("%s ", cmds[i].cmd[j]);
			 j++;
		}
		printf("\n");
		printf("has pipe: %s\n", cmds[i].has_pipe? "TRUE" : "FALSE");;
	}
	printf(" ====== =====\n");
#endif
	free(cmds);
}