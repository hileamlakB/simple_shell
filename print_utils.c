#include "gbk.h"

/**
 *_write - writes to a file descriptor once the file que is full
 * no matter when that is or if the caller demands it
 *eg _write(-1, NULL, 0);//initilize func, returns -100
 *_write(1, "HELLO", 5);//writes to que since it is inititilzed,
 *	 returns 100
 *_write(1, NULL, 0);//prints HELLO from que since demanded,
 * returns 0 on sucess
 *@fd: file discriptor to write to, should be a negative
 * number to initialize the printer
 *for the first time;
 *@inp: the string to be printed, should be NULL, when initiliazing
 * and when wanting to print
 *@len: the number of thigns to be printed;
 *Return: -100 when successfull set, 100 when sucessfully write to
 * que or to fd if que is full and
 *0 on demand print
 */
int _write(int fd, char *inp, int len)
{

	static char writeque[BUFFER_t];
	static int nque;
	int i;

	if (fd < 0)
	{
		nque = 0;
		return (-100);
	}
	if (inp)
	{
		if ((len + nque) >= BUFFER_t)
		{
			if (write(fd, writeque, BUFFER_t) == -1)
			{
				perr(NULL, NULL, "problem writing");
				exit(-1);
			}
			nque = 0;
		}
		i = 0;
		while (i < len)
		{
			writeque[nque + i] = inp[i];
			i++;
		}
		writeque[nque + i] = '\0';
		nque += len;
		return (100);
	}
	if (!inp)
		if (write(fd, writeque, nque) == -1)
		{
			perr(NULL, NULL, "problem writing");
			exit(-1);
		}
	return (0);
}
