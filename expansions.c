#include "gbk.h"

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
	char *pid = itoa(getpid()), *stat = itoa(childstat);
	char *var = smalloc(2), *tmp = NULL, *rep = NULL, *_str = NULL;
	int i = 0, j = 0;

	if (**str != '\n')
		fnrep(str, "$$", pid), fnrep(str, "$?", stat);
	free(pid), free(stat);
	var[0] = 'k', var[1] = '\0';
	while (*(*str + i))
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
				tmp = smalloc(_strlen(var)), _strcpy(tmp, var + 1), rep = getenv(tmp);
				if (rep)
					fnrep(str, var, rep);
				free(var), free(tmp);
				var = (char *)smalloc(2), var[0] = ' ', var[1] = '\0', tmp = NULL;
			}
		}
		i++;
	}
	_str = *str;
	while (*_str++)
		if (*_str == '#')
			break;
	if (**str != '\n')
		*_str = '\0';
	free(var);
	return (0);
}
/*
 *int main(void)
 *{
 *	char *str = calloc(26, sizeof(char));
 *
 *	_strcpy(str, "echo $coled $$ $PATH $HOME $? # lotss\0");
 *	strexpand(&str, 90);
 *	printf("%s\n", str);
 *	free(str);
 *	return (0);
 *}
 */
