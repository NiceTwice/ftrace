/*
** test.c for test in /home/nicetwice/PSU_2015_ftrace
** 
** Made by fisun_s
** Login   <fisun_s@epitech.eu>
** 
** Started on  Sun May  1 19:17:32 2016 fisun_s
** Last update Sun May  1 19:31:06 2016 fisun_s
*/

#include	<unistd.h>

int		rec(int a)
{
  if (a > 0)
    rec(a - 1);
  return (0);
}

int		main()
{
  rec(5);
  write(1, "bite", 4);
  return (0);
}
