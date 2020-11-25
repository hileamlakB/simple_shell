#include "gbk.h"

/**
 *sarrdup - safe array duplicate
 *@src: array to be duplicated
 *Return: a double pointer to the newly allocated array
 */
char **sarrdup(char **src)
{
	char **tmp = smalloc(sizeof(char *) * (arlen(src) + 2));
	char **_tmp = tmp;

	while (*src)
		*_tmp = _strdup(*src), _tmp++, src++;
	*_tmp = NULL;
	return (tmp);

}
