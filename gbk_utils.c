#include "gbk.h"

/**
 *getfpath - gets the full path if possible of name from
 *the environment variable
 *@name: the name of comand
 *Return: a full path of name or name it self on failure
 */
char *getfpath(char *name)
{
	char *PATH = getenv("PATH"), *fpath = NULL;
	char *tokenized = _strtok(PATH, ":", 0);
	struct stat fileinfo;

	if (!name)
		return (NULL);
	/*incase it is alreay a full path*/
	/*
	 *3. handle commands like /bin, that are like paths but dont do anythin
	 */
	if (*name == '/' || *name == '.')
	{
		fpath = smalloc(_strlen(name) * sizeof(char) + 1);
		_strcpy(fpath, name);
		return (fpath);
	}
	/*incase it is found in one of the path locations*/
	while (tokenized)
	{
		fpath = smalloc((_strlen(tokenized) + _strlen(name) + 1) * sizeof(char) + 1);
		_strcpy(fpath, tokenized);
		_strcat(fpath, "/");
		_strcat(fpath, name);
		if (!stat(fpath, &fileinfo))
			return (fpath);
		free(fpath);
		tokenized = _strtok(NULL, ":", 0);
	}
	/*incase it couldnt be found any where*/
	fpath = smalloc(_strlen(name) * sizeof(char) + 1);
	_strcpy(fpath, name);
	return (fpath);
}

/**
 *parseargs - returns an array of arguments from a string
 *@cmd: the command string to tokenize
 *@del: delimeter to parse the arguments with
 *@args: a pointer to an array for the the arguments
 *@mod: delimeter method 0-character 1-string
 */
void parseargs(char *cmd, const char *del, char ***args, int mod)
{
	char *tokenized = NULL;
	int index = 0;

	*args = NULL;
	tokenized = _strtok(cmd, del, mod);
	while (tokenized != NULL)
	{
		/*1 for the new string, 1 for the NULL*/
		*args = srealloc(*args, (arlen(*args) +  2) * sizeof(char *));
		(*args)[index] = NULL;
		/*trim tokenized and store it in *args[index]*/
		trims(&((*args)[index]), tokenized);
		(*args)[index + 1] = NULL;
		tokenized = _strtok(NULL, del, mod);
		index++;
	}
}

/**
 *getinput - gets input from the command line and parses it in
 *to sub commands if any
 *@input: pointer to the input string
 *@inputlen: pointer to the len of input
 *@cmds: pointer to the array of the parsed input
 *Return: lenght of input
 */
int getinput(char **input, size_t *inputlen, char ***cmds)
{
	int len;

	write(1, "#cisfun$ ", 9);
	len = _getline(input, inputlen, 0);
	if (len == -1)
	{
		free(*input);
		exit(-1);
	}
	/*replace the ending new line with \0*/
	(*input)[len - 1] = '\0';
	parseargs(*input, ";", cmds, 0);
	return (len - 1);
}
