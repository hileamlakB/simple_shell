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
	char *currentdir = getenv("PWD"), *buf = NULL;
	size_t size = 0;

	if (!path || !_strcmp(path, "~"))
		status = chdir(getenv("HOME"));
	else if (!_strcmp(path, "-"))
		status = chdir(getenv("OLDPWD"));
	else
		status = chdir(path);
	if (status < -1)
	{

		perror("Couldn't change directory");
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
	char *path = NULL;

	path = realpath(name, NULL);
	if (!path)
	{
		perror("No such file or directory");
		return (-1);
	}
	system(path);
	free(path);

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
		perror("Command not found");
	}
	free(fpath);
	return (exitstat);
}

