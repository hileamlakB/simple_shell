#ifndef GBK
#define GBK

/*necessary libs*/
#include <stdlib.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
/*Variables*/
#define BUFFER_t 1024

/*structs*/

/**
 *struct cmdnode - a node for one command in a command list
 *@cmd: command string
 *@op: operation between current command and next command
 *@estat: exit status (shows if the command has been excuted or not)
 *@next: a pointer to the next operand
 */
typedef struct cmdnode
{
	char *cmd;
	char *op;
	int estat;
	struct cmdnode *next;
} cmdnode;

/**
 *struct alias - a node for one alias
 *@key: the alais
 *@value: value of the alias
 *@next: a pointer to the next alias
 */
typedef struct alias
{
	char *key;
	char *value;
	struct alias *next;
} alias;

/*gbk*/

void cmdmv(cmdnode **, int);
int xcmd(char **, int, alias **);

/*gbk_utils*/

char *getfpath(char *);
void parseargs(char *, const char *, char ***, int);
int getinput(char **, size_t *, char ***, int);
void trims(char **, char *);

/*builtins*/

int *handlebin(char **, alias **);

/*memory_utils*/

void freedp(char **);
int arlen(char **);
void *smalloc(unsigned int);
void *srealloc(void *, unsigned int);


/*memory_utils*/
char **sarrdup(char **);
void printdp(char **);

/*string_advanced_utils*/

int _getline(char **, size_t *, int);
int _strcmpd(char *, const char *);
char *_strtok(char *, const char *, int);
int _strcmps(char *, const char *);


/*string_advanced_utils2*/

int findd(char *, char *);
int fnrep(char **, char *, char *);

/*string_basic_utils*/

int _strlen(const char *);
char *_strcat(char *, char *);
char *_strdup(char *);
int _strcmp(char *, char *);
char *_strcpy(char *, char *);

/*env_utils*/
int _putenv(char *);
int _setenv(const char *, const char *, int);
int _unsetenv(const char *);
char *_getenv(const char *);
void _printenv(void);

/*sys_utils*/
int _chdir(char *);
int runscript(char *);
int execute(char **);

/*expansion*/
int strexpand(char **, int);

/*alias*/
int add_alias(alias **, char *, char *);
int print_aliass(alias *);
int print_alias(alias *, char *);
int handle_alias(char **, alias **);
int freealias(alias *);

/*alas2*/
void aliasexpand(char **, alias *);

/*unalias*/

int remove_alias(alias **, char *);

/*cmd_struct*/
int add_node(cmdnode **, char *, char *);
cmdnode *build_list(char *);
int print_cmdnodes(cmdnode *);
void free_cmdlist(cmdnode *);
cmdnode *add_nodein(cmdnode **, char *, char *, int);

/*num_utils*/
char *itoa(int);
int _atoi(char *);

/*help*/
void phelp(char *arg);

/*history*/
int phistory(void);
int whistory(char *);

/*printing_utils*/
int _write(int, char *, int);

/*error_utils*/
void perr(char *, int *, char *);

/*externs*/
extern char **environ;

/*global variables*/

#endif
