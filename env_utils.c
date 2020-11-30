#include "gbk.h"
#define l(x) _strlen((x))
#define cch const char

/**
 *_putenv - addes es to environ
 *@es: environmnetal varibale value pair
 *Return: 0 on secuss and -1 on faliure
 */
int _putenv(char *es)
{
	char **_environ = environ, **newenviron, **_newenviron;
	int len = 0;

	while (*_environ)
		len++, _environ++;
	/*one for the new variable and the other for the null 2 for saftey*/
	newenviron = smalloc(sizeof(char *) * (len + 4));
	_newenviron = newenviron;
	_environ =  environ;
	while (len)
	{
		*_newenviron = smalloc(sizeof(char) * _strlen(*_environ) + 4);
		_strcpy(*_newenviron, *_environ);
		_newenviron++, _environ++, len--;
	}
	*_newenviron = smalloc(sizeof(char) * _strlen(es) + 4);
	_strcpy(*_newenviron, es);
	free(es);
	_newenviron++;
	*_newenviron = NULL;
	freedp(environ);
	environ = newenviron;
	return (0);
}

/**
 *_setenv - addes es to environ if it doesn't already
 *exist or overwrite is enabled
 *@name: variable name
 *@value: value
 *@overwrite: overwrite status
 *Return: 0 on secuss and -1 on faliure
 */
int _setenv(cch *name, cch *value, __attribute__((unused))int overwrite)
{
	char *es, **ep, *var;
	int i = 0;

	if (name == NULL || name[0] == '\0' || value == NULL)
		return (-1);

	/*check  if the var already exists and overwrite is allowed*/
	/**
	 *if (_getenv(name) != NULL && overwrite == 0)
	 *		return (0);
	 *
	 */
	if (environ)
	{
		ep = sarrdup(environ);
		while (ep[i])
		{

			var = _strtok(ep[i], "=", 0);
			if (!_strcmp(var, (char *)name))
			{
				free(environ[i]);
				environ[i] = smalloc(l(name) + l(value) + 4);
				_strcpy(environ[i], (char *)name);
				_strcat(environ[i], "=");
				_strcat(environ[i], (char *)value);
				freedp(ep);
				return (0);
			}
			i++;
		}
		freedp(ep);
	}
	es = smalloc(_strlen(name) + _strlen(value) + 2);
	/* +2 for '=' and null terminator */
	if (es == NULL)
		return (-1);
	_strcpy(es, (char *)name), _strcat(es, "="), _strcat(es, (char *)value);
	return ((_putenv(es) != 0) ? -1 : 0);
}

/**
 *_unsetenv - removes a variable from the environ list
 *@name: name of varaible
 *Return: 0 on secuss and -1 on faliure
 */
int _unsetenv(const char *name)
{
	char **ep, **sp, *var, *value;

	if (name == NULL || name[0] == '\0')
		return (-1);
	ep = sarrdup(environ);
	freedp(environ);
	environ = smalloc(sizeof(char *));

	for (sp = ep; *sp != NULL; )
	{

		var = _strtok(*sp, "=", 0);
		if (_strcmp(var, (char *)name))
		{
			value = _strtok(NULL, "=", 0);
			_setenv(var, value, 1);
		}
		sp++;
	}
	freedp(ep);
	return (0);
}

/**
 *_printenv - prints environmental variables
 **/
void _printenv(void)
{
	char **_env = environ;

	if (!_env)
		return;
	_write(-1, NULL, 0);
	while (*_env)
	{
		_write(1, *_env, _strlen(*_env));
		_write(1, "\n", 1);
		_env++;
	}
	_write(1, NULL, 0);
}



/**
 * _getenv - Returns pointer to value associated with name, if any, else NULL.
 *@name: the key
 *Return: the value.
 */
char *_getenv(const char *name)
{
	int len, i;
	const char *np;
	char **p, *cp;

	if (name == NULL || environ == NULL)
		return (NULL);
	for (np = name; *np && *np != '='; ++np)
		;
	len = np - name;
	for (p = environ; (cp = *p) != NULL; ++p)
	{
		for (np = name, i = len; i && *cp; i--)
			if (*cp++ != *np++)
				break;
		if (i == 0 && *cp++ == '=')
			return (cp);
	}
	return (NULL);
}
