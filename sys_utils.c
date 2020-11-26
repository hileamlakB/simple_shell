#include "gbk.h"

/**
 *_chdir - changes current working directory to path
 *and handle all the problems
 *@path: the new path
 *Return: 0 on sucess and 1 on failure
 */
int _chdir(char *path)
{
	int status;
	char *currentdir = _getenv("PWD"), *buf = NULL;
	size_t size = 0;

	if (!path || !_strcmp(path, "~"))
		status = chdir(_getenv("HOME"));
	else if (!_strcmp(path, "-"))
		status = chdir(_getenv("OLDPWD"));
	else
		status = chdir(path);
	if (status < 0)
	{

		perr(NULL, NULL, "Couldn't change directory");
		return (-1);
	}

	_setenv("OLDPWD", currentdir, 1);
	_setenv("PWD", getcwd(buf, size), 1);
	return (0);
}


/**
 *runscript - runs a script file if it is possible
 *@name: name of the file
 *Return: 0 on sucess and -1 on faliure
 */
int runscript(char *name)
{
	char *path = NULL, *currentdir = NULL;
	int fd, i, cmdslen;
	size_t len = 0;
	char *input, **cmds;
	alias *head = NULL;

	if (name[0] != '.' && name[0] != '~' && name[0] != '/')
	{
		currentdir = _getenv("PWD");
		path = smalloc(_strlen(currentdir) + _strlen(name) + 4);
		_strcpy(path, currentdir);
		_strcat(path, "/");
		_strcat(path, name);
	}
	else
		path = name;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		free(path);
		perr(NULL, NULL, "Couln't open script");
		return (-1);
	}
	free(path);

	getinput(&input, &len, &cmds, fd);
	i = 0, cmdslen = arlen(cmds);
	free(input);
	while (i < cmdslen)
		xcmd(cmds, i, &head), i++;
	if (cmdslen > 0)
		freedp(cmds);
	return (0);

}

/**
 *execute- execute a process by creating a child
 *@tmp: a double pointer coanting all the arguments
 *Return: 0 on sucess and faliure number on failure
 */
inline int execute(char **tmp)
{
	struct stat cmdinfo;
	char *fpath = NULL;
	int exitstat = 0;

	fpath = getfpath(tmp[0]);
	if (stat(fpath, &cmdinfo) == 0 && cmdinfo.st_mode & S_IXUSR)
		exitstat = execve(fpath, tmp, environ);
	else
	{
		exitstat = -1;
		perr(NULL, NULL, "Command not found");
	}
	free(fpath);
	return (exitstat);
}

