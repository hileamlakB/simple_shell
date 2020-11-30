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
	char *currentdir = _getenv("PWD"), *buf = NULL, *cdir, *errmsg;
	size_t size = 0;

	if (!path || !_strcmp(path, "~"))
		status = chdir(_getenv("HOME"));
	else if (!_strcmp(path, "-"))
		status = chdir(_getenv("OLDPWD"));
	else
		status = chdir(path);
	if (status < 0)
	{
		errno = -3;
		errmsg = smalloc(_strlen("cd: can't cd to ") + _strlen(path) + 4);
		_strcpy(errmsg, "cd: can't cd to ");
		_strcat(errmsg, path);
		perr(NULL, NULL, errmsg);
		free(errmsg);
		return (-1);
	}

	_setenv("OLDPWD", currentdir, 1);
	cdir = getcwd(buf, size);
	_setenv("PWD", cdir, 1);
	free(buf), free(cdir);
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
	int fd, i, cmdlen;
	size_t len = 0;
	char *input, **cmds, *errmsg, **cmds2;
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
		errno = -3;
		errmsg = smalloc(_strlen("Can't open ") + _strlen(name) + 4);
		_strcpy(errmsg, "Can't open ");
		_strcat(errmsg, name);
		perr(NULL, NULL, errmsg);
		free(errmsg);
		free(path);
		return (127);
	}
	free(path);

	getinput(&input, &len, &cmds, fd), fnrep(&(cmds[0]), "\n", ";");
	parseargs(cmds[0], ";", &cmds2, 0), free(input), freedp(cmds);

	i = 0, cmdlen = arlen(cmds2);
	while (i < cmdlen)
		xcmd(cmds2, i, &head), i++;
	freedp(cmds2);
	return (0);
}

/**
 *execute- execute a process by creating a child
 *@tmp: a double pointer coanting all the arguments
 *Return: 0 on sucess and faliure number on failure
 */
int execute(char **tmp)
{
	struct stat cmdinfo;
	char *fpath = NULL;
	int exitstat = 0;

	if (!tmp)
		return (exitstat);
	fpath = getfpath(tmp[0]);
	if (stat(fpath, &cmdinfo) == 0 && cmdinfo.st_mode & S_IXUSR)
		exitstat = execve(fpath, tmp, environ);
	else
	{
		exitstat = 127;
		errno = -4;
		perr(NULL, NULL, "not found");
	}
	free(fpath);
	return (exitstat);
}

