#include "gbk.h"


/**
 *handlebin - handles builtin commands
 *@cmd: command arguements
 *@head: head of the alias list
 *Return: 0 if builting commond executed or 1 if not
 */
int *handlebin(char **cmd, alias **head)
{
	int exitstatus = 0;
	int *ret = smalloc(2 * sizeof(int));

	ret[0] = 1, ret[1] = 266;
	if (!_strcmp(cmd[0], "exit"))
	{
		if (cmd[1])
			exitstatus = atoi(cmd[1]) % 256;
		ret[0] = 0, ret[1] = exitstatus;
	}
	else if (!_strcmp(cmd[0], "env") || !_strcmp(cmd[0], "printenv"))
		_printenv(), ret[0] = 0;
	else if (!_strcmp(cmd[0], "\n"))
		ret[0] = 0;
	else if (!_strcmp(cmd[0], "setenv"))
	{
		if (arlen(cmd) != 3)
			perror("Too few or too many arguements");
		_setenv(cmd[1], cmd[2], 0), ret[0] = 0;
	}
	else if (!_strcmp(cmd[0], "unsetenv"))
	{
		if (arlen(cmd) != 2)
			perror("Too few or too many arguements");
		_unsetenv(cmd[1]), ret[0] = 0;
	}
	else if (!_strcmp(cmd[0], "cd"))
		_chdir(arlen(cmd) > 1 ? cmd[1] : NULL),	ret[0] = 0;
	else if (!_strcmp(cmd[0], "history"))
		phistory(), ret[0] = 0;
	else if (!_strcmp(cmd[0], "help"))
		phelp(arlen(cmd) > 1 ? cmd[1] : NULL),	ret[0] = 0;
	else if (!_strcmp(cmd[0], "alias"))
		handle_alias(cmd, head), ret[0] = 0;

	if (!ret[0] && ret[1] == 266)
		freedp(cmd);
	return (ret);
}
