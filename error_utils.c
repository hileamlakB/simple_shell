#include "gbk.h"
#include "errlist.h"
/**
 *perr - prints error message
 *@prog: name of program
 *@cmdr: pointer to number of commands run including this one
 *@msg: message to be printed
 */
void perr(char *prog, int *cmdr, char *msg)
{
	char *numcmd;
	static int *cmdsrun;
	static char *progname;

	if (!msg)
	{
		cmdsrun =  cmdr;
		progname = prog;
		return;
	}
	if (!prog && !cmdr)
	{
		_write(-1, NULL, 0);
		_write(2, progname, _strlen(progname));
		_write(2, ": ", 2);
		numcmd = itoa(*cmdsrun);
		_write(2, numcmd, _strlen(numcmd));
		free(numcmd);
		_write(2, ": ", 2);
		if (errno < 0)
			_write(2, msg, _strlen(msg));
		else
			_write(2, (char *)errlist[errno], _strlen((char *)errlist[errno]));
		_write(2, "\n", 1);
		_write(2, NULL, 0);
	}
}
