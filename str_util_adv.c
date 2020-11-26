#include "gbk.h"

/**
 *_getline - gets a line of string from a file
 *@lnptr: variable to store string
 *@size: number of things stored
 *@fd: file stream to read from
 *Return: the lenghh of lnptr or -1 on failure
 */
int _getline(char **lnptr, size_t *size, int fd)
{
	char buffer[BUFFER_t + 1];
	int r = BUFFER_t, len = 0, mode = isatty(0);
	char *tmp;

	/*mode == 1 -> interactive mode , mode == 0 non-interavtive mdoe*/
	*lnptr = (char *)smalloc(4);
	**lnptr = '\0';
	while (r == BUFFER_t)
	{
		if (*lnptr && len > 0)/*if input exactly fits in the buffer*/
			if ((*lnptr)[len - 1] == '\n' && mode)
				break;
		r = read(fd, buffer, BUFFER_t);
		if (r < 0 && errno == EINTR)
		{
			**lnptr = '\n',	*(*lnptr + 1) = '\n', len = 2;
			return (len);
		}
		if (r < 0)/*cant be read for some reason*/
			exit(-1);
		/*Ctrl-D pressed in interactive || EOF reached in none mode*/
		if (r == 0 && mode)
			return (-1);
		tmp = srealloc(*lnptr, len + r + 4);
		*size = len + r + 4, *lnptr = tmp;
		buffer[r] = '\0', _strcat(*lnptr, buffer), len += r;
		if (!mode)/*rid of \n in non-interactive mode to handle multi-line cmds*/
			fnrep(lnptr, "\n", ";");
	}
	if (!mode)
	{
		tmp = srealloc(*lnptr, _strlen(*lnptr) + 3);
		*lnptr = tmp;
		(*lnptr)[len] = '\n', (*lnptr)[len + 1] = '\0';
	}
	return (len);
}

/**
 *_strcmpd - A special compare function that compares if one char
 * inside the sub string is equal to the first char of fstring
 *@fstring: fstring
 *@sub: subset
 *Return: 1 on success and -1 on faliur
 */
int _strcmpd(char *fstring, const char *sub)
{
	if (!fstring || !sub)
		return (-1);
	if (!*fstring)
		return (-1);
	if (_strlen(fstring) < _strlen(sub))
		return (-1);
	while (*sub)
	{
		if (*sub == *fstring)
			return (1);
		sub++;
	}
	return (-1);
}
/**
 *_strcmps - A special compare function that compares if part of a string
 * inside the sub char is equal to the second string
 *@fstring: fstring
 *@sub: subset
 *Return: 1 on success and -1 on faliur
 */
int _strcmps(char *fstring, const char *sub)
{
	if (!fstring || !sub)
		return (-1);
	if (!*fstring)
		return (-1);
	if (_strlen(fstring) < _strlen(sub))
		return (-1);
	while (*sub && *fstring)
	{
		if (*sub != *fstring)
			return (-1);
		sub++, fstring++;
	}
	return (1);
}

/**
 * _strtok - tokenizes a string according to a certain delimiter
 * it doesnt creat a new string to hold the tokens but rather creats a
 * a static variable that will directly copy str and puts the null
 * terminator everytime it finds the delimeter, the default str will be
 * manipulated so beware
 * for example if you have a string str = "helo; now; bo"
 * when _strtok is called for the first time (_strtok(str, ";")) it will put
 * \0 (a null terminator in the first location of the delimeter so the str
 * variable will be "helo\0 nowo; bo", ";" and it will be returned and the
 * static variable save would hold " nowo; bo" and when _strtok is called
 * for the second time _strtok(NULL, ";"); the same cyle continue
 * but this time instead of str save will be manipulated. This
 *cycle continues untile save == NULL
 * @str: the string to be tokenized
 * @delimeter: the delimiter to separate tokens
 * @whichf: which comparign funcion to use, 0 for strcmpd, and 1 for strcmps
 * Return: a character pointer to the current delimited token
 */
char *_strtok(char *str, const char *delimeter, int whichf)
{
	static char *save;
	char *_new = NULL;
	int i = 0, (*func)(char *, const char *), loc;

	func = (whichf == 0) ? _strcmpd : _strcmps;
	loc = (whichf) ? _strlen(delimeter) - 1 : 0;
	if (!str || !*str)
	{
		if (!save || !*save)
			return (NULL);

		while (func(save + i, delimeter) != 1 && *(save + i) != '\0')
			i++;
		if (*(save + i) == '\0')
		{
			_new = save, save = NULL;
			return (_new);
		}
		_new = save;
		*(save + i) = '\0';
		save = save + i + loc + 1;

		return (_new);

	}
	while (func(str + i, delimeter) != 1 && *(str + i) != '\0')
		i++;

	if (*(str + i) == '\0')
	{
		save = NULL;
		return (str);
	}
	save = str + i + loc + 1;
	*(str + i) = '\0';
	return (str);
}
