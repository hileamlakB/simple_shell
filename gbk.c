#include "gbk.h"

static void handlerc(int);
inline void cmdmv(cmdnode **, int);
inline void xcmd(char **, int, alias **);
/**
 *main - main loop for the gbk shell
 *@argc: number of arguments
 *@argv: script file names
 *@argp: envrionmental variables
 *Return: returns the index of 0 on sucess
 */
int main(int argc, char **argv, char **argp)
{
	char *cmd = NULL, **cmds = NULL;
	size_t cmdlen = 0;
	int mode = !isatty(0), i = 0, cmdnum = 1;
	struct sigaction sa;
	alias *head = NULL;

	environ = sarrdup(environ);
	if (argc > 1)
	{
		runscript(argv[1]);
		exit(0);
	}
	(void)argp;

	sa.sa_handler = handlerc, sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		cmd = NULL, cmds = NULL, getinput(&cmd, &cmdlen, &cmds);
		cmdnum = arlen(cmds), i = 0;
		free(cmd);
		while (i < cmdnum)
			xcmd(cmds, i, &head), i++;
		if (cmdnum > 0)
			freedp(cmds);
		if (mode)
			write(1, "#cisfun$ ", 9), exit(255);
	}
	free(environ);
	exit(EXIT_SUCCESS);
}

/**
 *handlerc - handlce SIGNUM
 *@signum: sig ID
 */
static void handlerc(int signum)
{
	write(1, "\n", 1);
	(void)signum;
}

/**
 *cmdmv - moves the command list to the next command
 *accorindign to the commands and the execute statues
 *of the current cmd
 *@head:a pointer to a pointer to the current commands
 *@childstat: the status of the execution of the current command
 *node
 */
inline void cmdmv(cmdnode **head, int childstat)
{
	cmdnode *tmp = NULL;

	if (!_strcmp((*head)->op, "||"))
	{
		if (!childstat)
		{
			free((*head)->op);
			(*head)->op = (*head)->next->op;

			tmp = (*head)->next->next;

			free((*head)->next->cmd);
			free((*head)->next);
			(*head)->next = tmp;
			(*head)->estat = 1;
		}
		else
			(*head) = (*head)->next;

	}
	else if (!_strcmp((*head)->op, "&&"))
	{
		if (!childstat)
			(*head)->estat = 1, (*head) = (*head)->next;
		else
		{
			free((*head)->op);
			(*head)->op = (*head)->next->op;
			tmp = (*head)->next->next;

			free((*head)->next->cmd);
			free((*head)->next);
			(*head)->next = tmp;
		}
	}
	else
		(*head) = (*head)->next;
}
/**
 *xcmd - parses, and uses the excute function
 *to creats a process and excute the cmd
 *@cmd_l: commands list
 *@index: current command index
 *@aliashead: head of the laias list
 */
inline void xcmd(char **cmd_l, int index, alias **aliashead)
{
	char **tmp = NULL, *cmds = cmd_l[index];
	int childid, *binstat, exitstat;
	static int childstat;
	cmdnode *head = NULL, *_head = NULL;

	_head = build_list(cmds), head = _head;
	while (head)
	{
		strexpand(&(head->cmd), childstat);
		parseargs(head->cmd, " ", &tmp, 0);
		whistory(cmds);
		binstat = handlebin(tmp, aliashead);
		if (!binstat[0])
		{
			if (binstat[1] != 266)
			{
				exitstat = binstat[1];
				freedp(environ), free(binstat), freedp(tmp);
				freedp(cmd_l), free_cmdlist(_head);
				exit(exitstat);
			}
			cmdmv(&head, 0), free(binstat);
			continue;
		}
		free(binstat);
		if (head->estat == 0)
		{
			fflush(stdout);
			childid = fork();
			if (childid < 0)
			{
				perror("ERROR: coudn't creat a child proccess");
				exit(-1);
			}
			if (childid == 0)
			{
				childstat = execute(tmp);
				freedp(environ), freedp(tmp), freedp(cmd_l), free_cmdlist(_head);
				exit(childstat);
			}
			else
			{
				wait(&childstat);
				cmdmv(&head, childstat);
			}
		}
		else
		{
			if (!_strcmp(head->op, "||"))
			{
				free(head->op);
				head->op = head->next->op;
				head->next = head->next->next;
			}
			else
				head = head->next;
		}
		freedp(tmp);
	}
	free_cmdlist(_head);
}

