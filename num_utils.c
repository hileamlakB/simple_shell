#include "gbk.h"


/**
 *powB - raises the number base to power power
 *@base : the base
 *@power : the power
 *Return: return the answer
 */
int powB(unsigned int base, int power)
{
	int i, prod = 1;

	for (i = 0; i < power; i++)
	{
		prod = prod * base;
	}
	return (prod);
}

/**
 *numLength - returns the lenth of string
 *@num : operand number
 *Return: number of digits
 */
int numLength(unsigned int num)
{
	int length = 0;

	if (!num)
		return (1);

	while (num)
	{
		num = num / 10;
		length += 1;
	}

	return (length);
}

/**
 *itoa - Print a number of any digit
 *@n : takes an input number
 *Return: a string for of the nuber
 */
char *itoa(int n)
{
	int length, j, k, digit1, tmp2, i = 0;
	unsigned int num;
	char *nums = smalloc(numLength((n > 0) ? n : -1 * n) + 2);

	if (!nums)
		return (NULL);
	if (n < 0)
		n *= -1, *(nums + i) = '-', i++;
	num = n, length = numLength(num), j = length - 1;
	if (num == 0)
		nums[i] = 48, i++;
	else
	{
		while (j >= 0)
		{
			if (num % powB(10, j) == 0 && j != 0)
			{
				nums[i] = (48 + num / powB(10, j)), i++;
				for (k = j; k > 0; k--)
					nums[i] = 48, i++;
				j = -1;
			}
			else
			{
				digit1 = num / powB(10, j);
				nums[i] = digit1 + 48, i++;

				tmp2 = num;
				num -= powB(10, j) * digit1;
				if (numLength(tmp2) - numLength(num) == 2)
					nums[i] = 48, i++, j--;
				j--;
			}
		}
	}
	return (nums);
}

/**
 * _atoi - converts a string to integer
 * @s: string s
 * Return: returns parsed integer
 */
int _atoi(char *s)
{
	unsigned int num = 0, sign = 1, started = 0;

	while (*s)
	{
		if (started && !(*s >= '0' && *s <= '9'))
			break;
		if (*s == '-')
			sign *= -1;
		if ((*s >= '0' && *s <= '9'))
		{
			started = 1;
			num =  num * 10 + (int)*s - 48;
		}
		s++;
	}
	return (sign * num);
}
