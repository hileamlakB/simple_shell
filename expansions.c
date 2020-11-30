#include "gbk.h"

/**
*signexpand - expand commo signs
*@str: string to be manipulated
*@stat: last exit stat
*/
void signexpand(char **str, int stat)
{

	char *pid = itoa(getpid()), *stats = itoa(stat);

	if (**str != '\n')
		fnrep(str, "$$", pid), fnrep(str, "$?", stats);

	free(pid), free(stats);
}

/**
 *strexpand - expands a variabless like the shell does
 *gets rid of commented commands also expands
 *$$, $?
 *@str: string containging variables to be expanded
 *@childstat: the last exit status of the child process
 *Return: 0 on sucess and -1 on error
 */
int strexpand(char **str, int childstat)
{
	char *var = smalloc(2), *tmp = NULL, *rep = NULL, *_str = NULL;
	int i = 0, j = 0;

	signexpand(str, childstat);
	var[0] = 'k', var[1] = '\0';
	while (*(*str + i))/*replace any invironmental variables if any*/
	{
		if (*(*str + i) == '$' && *str + i + 1)
		{
			if (*(*str + i + 1) != '$' && *(*str + i + 1) != '?')
			{
				j = 0;
				while (*(*str + i + j) != ' ')
				{
					var = srealloc(var, _strlen(var) + 2);
					*(var + j) = *(*str + i + j), *(var + j + 1) = '\0';
					if (!(*str + i + j + 1))
						break;
					j++;
				}
				tmp = smalloc(_strlen(var)), _strcpy(tmp, var + 1), rep = _getenv(tmp);
				if (rep)
					fnrep(str, var, rep);
				free(var), free(tmp);
				var = (char *)smalloc(2), var[0] = ' ', var[1] = '\0', tmp = NULL;
			}
		}
		i++;
	}
	_str = *str;
	while (*_str)
	{
		if (*_str == '#')
			break;
		_str++;
	}
	if (**str != '\n')
		*_str = '\0';
	free(var);
	return (0);
}
/**
 *aliasexpand - expand's aliases inside a string
 *@str:string to be manipulated
 *@head:head of the alias list
 */
void aliasexpand(char **str, alias *head)
{

	while (head)
	{
		fnrep(str, head->key, head->value);
		head = head->next;
	}
}
