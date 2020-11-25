#include "gbk.h"
/**
 *findd - finds the number occurance of tof in str
 *@str: string to be searched
 *@tof: string to fid
 *Return: the number of times teh delimeter ocures
 */
int findd(char *str, char *tof)
{
	char *tokenized = NULL, *new = NULL;
	int i = 0;

	if (_strlen(tof) > _strlen(str))
		return (0);
	new = _strdup(str);
	tokenized = _strtok(new, tof, 1);
	while (tokenized != NULL)
	{
		i++;
		tokenized = _strtok(NULL, tof, 1);
	}
	free(new);
	/*icase tofind is at th end*/
	if (_strcmps(str + _strlen(str) - _strlen(tof), tof) == 1)
		i++;
	return (i - 1);
}
/**
 *fnrep - finds and replaces part of a string
 *@str: manipulated string (must be malloced)
 *      this string should also be freed by the caller
 *@torep: subset of str to be replaced
 *@repwith: string to replace torep
 *Return: 0 on sucess and -1 on faliure
 */
int fnrep(char **str, char *torep, char *repwith)
{
	char *tokenized = NULL, *tmp, *new = smalloc(1);
	int rep = 0, tor = findd(*str, torep), newlen = 0;

	tokenized = _strtok(*str, torep, 1);
	*new = '\0';
	while (tokenized != NULL)
	{
		newlen = _strlen(new) + _strlen(tokenized) + _strlen(repwith);
		tmp = srealloc(new, newlen +  2);
		new = tmp;
		_strcat(new, tokenized);
		if (rep < tor)
			_strcat(new, repwith);
		rep++, tokenized = _strtok(NULL, torep, 1);
	}
	free(*str);
	*str = new;
	return (0);
}
