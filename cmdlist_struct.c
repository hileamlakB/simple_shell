#include "gbk.h"

/**
 * add_node - adds a new node at the end of a cmdnode
 * @head: double pointer to head of list
 * @cmd: the command
 * @op: the operator between  the current command the next one
 * Return: 0 on sucess -1 on failure
 */
int add_node(cmdnode **head, char *cmd, char *op)
{
	cmdnode *newNode = smalloc(sizeof(cmdnode));
	cmdnode *current;

	if (!head)
		return (-1);
	/*handle stdup failures*/
	newNode->cmd = _strdup(cmd);
	newNode->op = _strdup(op);
	newNode->estat = 0;
	newNode->next = NULL;
	if (!*head)
		*head = newNode;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = newNode;
	}
	return (0);
}
/**
 *build_list - builds a a command list based on cmds
 * - parse the cmds if it has any ||
 * -look for any &&'s and connecet the commands in the previous created list
 *@cmds: the command to be prepared into a list
 *Return: a pointer to the head of the created list
 */
cmdnode *build_list(char *cmds)
{
	char **parsedcmd = NULL, **tmp = NULL, **_parsedcmd = NULL;
	cmdnode *head = NULL, *_head = NULL, *tmp2 = NULL, *current, *previous = NULL;
	int len, i = 0;

	parseargs(cmds, "||", &parsedcmd, 1), _parsedcmd = parsedcmd;
	len = arlen(parsedcmd);
	if (len > 0)
	{
		while (*_parsedcmd)
			add_node(&head, *_parsedcmd, (len > 1) ? "||" : ""), _parsedcmd++;
		freedp(parsedcmd), _head = head;
		while (_head)
		{
			parseargs(_head->cmd, "&&", &tmp, 1), len = arlen(tmp);
			if (len > 1)
			{
				i = 0;/*if && is found creat new list & put it in the middle*/
				while (tmp[i])
					add_node(&tmp2, tmp[i], "&&"), i++;
				current = _head->next;
				if (previous)
					previous->next = tmp2;
				else
					head = tmp2;
				while (tmp2->next)
				{
					if (!(tmp2->next->next))
						previous = tmp2;
					tmp2 = tmp2->next;
				}
				tmp2->next = current, free(tmp2->op), tmp2->op = _strdup(_head->op);
				free(_head->cmd), free(_head->op), free(_head), _head = tmp2, tmp2 = NULL;
			}
			else
				previous = _head, _head = _head->next;
			freedp(tmp);
		}
		free(previous->op), previous->op = _strdup("");
	}
	return (head);
}

/**
 *print_cmdnodes - print contents of a linked list
 *@head: pointer to the first node of the list
 *Return: 0 on sucess -1 on faliure
 */
int print_cmdnodes(cmdnode *head)
{
	if (!head)
		return (-1);
	while (head)
	{
		printf("cmd %s, op'%s'\n", head->cmd, head->op);
		head = head->next;
	}
	return (0);
}

/**
 * free_cmdlist - frees a cmdnode list
 * @head: pointer to head of list
 */
void free_cmdlist(cmdnode *head)
{
	cmdnode *tmp;

	if (!head)
		return;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->cmd);
		free(tmp->op);
		free(tmp);
	}
	head = NULL;
}
/**
 * add_nodein - adds a new node at the end of a cmdnode
 * @head: double pointer to head of list
 * @cmd: the command and arguments arry to add
 * @operator: the operator between  the current command the next one
 * @pos:position to insert the node
 * Return: address of node added
 */
cmdnode *add_nodein(cmdnode **head, char *cmd, char *operator, int pos)
{
	cmdnode *newNode = smalloc(sizeof(cmdnode));
	cmdnode *current = NULL, *tmp = NULL;
	int i = 1;

	if (!head)
		return (NULL);
	newNode->cmd = cmd;
	newNode->op = operator;
	newNode->next = NULL;
	if (!*head)
		*head = newNode;
	else
	{
		current = *head;
		while (current->next && i < pos)
			current = current->next, i++;
		tmp = current->next;
		current->next = newNode;
		newNode->next = tmp;
	}
	return (newNode);
}
/*
 *int main(void)
 *{
 *	char *cmd = malloc(_strlen("ls||cat file && grep io *||op
 *&& pop && kl") + 3);
 *	cmdnode *none;
 *	char *cmd2 = malloc(_strlen("ls||none||cat file&&grep||io *
 *||op&&pop&&kl||pol&&kl||po") + 3);
 *	cmdnode *none2;
 *
 *
 *	printf("ls||cat file && grep io *||op && pop && kl\n");
 *	_strcpy(cmd, "ls||cat file && grep io *||op && pop && kl");
 *	none = build_list(cmd);
 *	print_cmdnodes(none);
 *	free_cmdlist(none);
 *
 *	printf("\n\nls||none||cat file&&grep||io *||op&&pop&&kl
 *||pol&&kl||po\n");
 *	_strcpy(cmd2, "ls||none||cat file&&grep||io *||op&&pop&&kl
 *||pol&&kl||po");
 *	none2 = build_list(cmd2);
 *	print_cmdnodes(none2);
 *	free_cmdlist(none2);
 *
 *	free(cmd), free(cmd2);
 *
 *	return (0);
 *}
 */
