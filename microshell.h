#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


typedef struct
{
	char	**cmd;
	int		has_pipe;
}	command;
