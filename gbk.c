#include "gbk.h"
#include <stdio.h>

static void handlerc(int);
void cmdmv(cmdnode **, int);
int xcmd(char **, int, alias **);
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
	int mode = !isatty(0), i = 0, cmdnum = 1, inputs = 0, ret;
	alias *head = NULL;

	/*initialize erro function*/
	perr(argv[0], &inputs, NULL);
	/*initialize environmental variables*/
	environ = sarrdup(argp);
	if (argc > 1)
	{
		ret = runscript(argv[1]);
		freedp(environ);
		exit(ret);
	}
	/*initialize signal handler*/
	signal(SIGINT, handlerc);
	while (1)
	{
		/*get command and parse based on ;*/
		cmd = NULL, cmds = NULL, getinput(&cmd, &cmdlen, &cmds, 0);
		cmdnum = arlen(cmds), i = 0;
		free(cmd);
		inputs++;/*increase the number of commands accepted*/
		/*execute all the commands one by one*/
		while (i < cmdnum)
			ret = xcmd(cmds, i, &head), i += 1;
		if (cmdnum > 0)
			freedp(cmds);
		if (mode)
		{
			freedp(environ), freealias(head);
			return (ret);
		}
	}
	freedp(environ), freealias(head);
	return (ret);
}

/**
 *handlerc - handlce SIGNUM
 *@signum: sig ID
 */
static void handlerc(int signum)
{
	write(1, "\n$ ", 3);
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
void cmdmv(cmdnode **head, int childstat)
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
			while ((*head))
			{
				if (!((*head)->next))
				{
					*head = (*head)->next;
					break;
				}
				if (!_strcmp((*head)->next->op, "||"))
				{
					(*head) = (*head)->next;
					break;
				}
				*head = (*head)->next;
			}
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
 *Return: the exetstatus of the execution
 */
int xcmd(char **cmd_l, int index, alias **aliashead)
{
	char **tmp = NULL, *cmds = cmd_l[index];
	int childid, *binstat;
	static int childstat, exitstat;
	cmdnode *head = NULL, *_head = NULL, *tmp_n = NULL;


	/*write the command to history*/
	whistory(cmds);
	_head = build_list(cmds), head = _head;
	while (head)
	{
		strexpand(&(head->cmd), exitstat);
		aliasexpand(&(head->cmd), *aliashead);
		parseargs(head->cmd, " ", &tmp, 0);
		binstat = handlebin(tmp, aliashead);
		if (!binstat[0])
		{
			if (binstat[1] != 266)
			{
				exitstat = binstat[1];
				freedp(environ), free(binstat), freedp(tmp);
				freedp(cmd_l), free_cmdlist(_head), freealias(*aliashead);
				exit(exitstat);
			}
			cmdmv(&head, 0), free(binstat);
			continue;
		}
		free(binstat);
		if (head->estat == 0)
		{
			childid = fork();
			if (childid < 0)
			{
				perr(NULL, NULL, "Coudn't creat a child proccess");
				exit(127);
			}
			if (childid == 0)
			{
				childstat = execute(tmp);
				freedp(environ), freealias(*aliashead);
				freedp(tmp), freedp(cmd_l), free_cmdlist(_head);
				exit(childstat);
			}
			else
			{
				wait(&childstat);
				if (WIFEXITED(childstat))
					exitstat = WEXITSTATUS(childstat);
				cmdmv(&head, exitstat);
			}
		}
		else if (head->estat == 1)
		{
			if (!_strcmp(head->op, "||"))
			{
				free(head->op);
				head->op = head->next->op;
				tmp_n = head->next->next;
				free(head->next->cmd);
				free(head->next);
				head->next = tmp_n;
			}
			else
				head = head->next;
		}
		freedp(tmp);
	}
	free_cmdlist(_head);
	return (exitstat);
}

