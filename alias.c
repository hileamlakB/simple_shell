#include "gbk.h"

/**
 *add_alias - add a node to a linked list
 *@head: the key of the node to be removed
 *@key: the key to add in the node
 *@value: the value to be added with the key
 *Return: 0 on sucess -1 on faliure
 */
int add_alias(alias **head, char *key, char *value)
{
	alias *node, *_head;

	if (!head)
		return (-1);

	node = smalloc(sizeof(alias));
	node->key = _strdup(key);
	node->value = _strdup(value);
	node->next = NULL;
	if (!*head)
	{
		*head = node;
		return (0);
	}
	_head = *head;
	while (_head->next)
	{
		if (!_strcmp(_head->key, key))
		{
			free(_head->value);
			_head->value = _strdup(value);
			freealias(node);
			return (0);
		}
		_head = _head->next;

	}
	_head->next = node;
	return (0);
}


/**
 *print_aliass - print contents of a linked list
 *@head: pointer to the first node of the list
 *Return: 0 on sucess -1 on faliure
 */
int print_aliass(alias *head)
{
	if (!head)
		return (-1);
	while (head)
	{


		_write(-1, NULL, 0);
		_write(1, head->key, _strlen(head->key));
		_write(1, "='", 2);
		_write(1, head->value, _strlen(head->value));
		_write(1, "'\n", 2);
		_write(1, NULL, 0);

		head = head->next;
	}
	return (0);
}
/**
 *print_alias - prints contents of a node whose key is key
 *@head: pointer to the first node of the list
 *@key: the key of the node to be printed
 *Return: 0 on sucess -1 on faliure
 */
int print_alias(alias *head, char *key)
{
	if (!head)
		return (-1);
	while (head)
	{
		if (!_strcmp(head->key, key))
		{
			_write(-1, NULL, 0);
			_write(1, head->key, _strlen(head->key));
			_write(1, "='", 2);
			_write(1, head->value, _strlen(head->value));
			_write(1, "'\n", 2);
			_write(1, NULL, 0);
			return (0);
		}
		head = head->next;
	}
	errno = -5;
	perr(NULL, NULL, "Alias not found");
	return (-1);
}

/**
 *handle_alias - takes a list of arguments and acts accordingly
 * assumeing they are part of an alias flag
 *@arg: the argument vector
 *@aliashead: pointer to the first node of the list
 *Return: 0 on sucess -1 on faliure
 */
int handle_alias(char **arg, alias **aliashead)
{
	int argc = arlen(arg);
	int i = 0;
	char **tmp = NULL;

	if (argc == 0)
		return (-1);

	if (argc == 1)
		print_aliass(*aliashead);

	if (argc > 1)
	{
		i += 1;
		while (arg[i])
		{
			parseargs(arg[i], "=", &tmp, 0);
			if (arlen(tmp) > 1)
				add_alias(aliashead, tmp[0], tmp[1]);
			else
				print_alias(*aliashead, tmp[0]);
			freedp(tmp);
			i++;
		}
	}
	return (0);
}

/**
 *freealias - free the alias linked list
 *@head: pointer to the first node of the list
 *Return: 0 on sucess -1 on faliure
 */
int freealias(alias *head)
{
	alias *tmp;

	while (head)
	{
		tmp = head->next;
		free(head->key);
		free(head->value);
		free(head);
		head = tmp;
	}
	return (0);
}
