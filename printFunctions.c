/*
** printFunctions.c for strace in /home/fisun_s/rendu_tek2/PSU_2015_strace
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sat Apr  9 02:34:56 2016 Fisun Sergii
** Last update Sun Apr 10 21:31:23 2016 Pierre Debruyne
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/user.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include "my_strace.h"

void			print_struct_stat_p(unsigned long number, pid_t pid)
{
  struct stat		buf;
  unsigned int		i;
  long			s;
  char			*str;

  i = 0;
  str = (char*)number;
  while (i < sizeof(struct stat))
    {
       s = ptrace(PTRACE_PEEKTEXT, pid, str, NULL);
       ((short*)&buf)[i / 2] = (short)*(&s);
       i += 2;
       str += 2;
    }
  printf("{st_mode=%d, st_size=%d, ...}", buf.st_mode, (int)buf.st_size);
}

void		print_void(unsigned long number, pid_t pid)
{
  (void)number;
  (void)pid;
  printf("?");
}

static void	print_word(long text)
{
  if ((char)*(&text) < 32 || (char)*(&text) > 126)
    printf("\\%d", (char)*(&text));
  else
    printf("%c", (char)*(&text));
  if (((char *)&text)[1] != 0)
    {
      if (((char *)&text)[1] < 32 || ((char *)&text)[1] > 126)
        printf("\\%d", ((char *)&text)[1]);
      else
	printf("%c", (char)*(&((char *)&text)[1]));
    }
}

void		print_char_p(unsigned long value, pid_t pid)
{
  char		*s;
  long		text;
  char		*txt;
  int		nb;

  s = (char*)value;
  nb = 0;
  if (s)
    {
      txt = "   ";
      printf("\"");
      while (txt[0] != 0 && (nb += 2) < 50)
	{
	  text = ptrace(PTRACE_PEEKTEXT, pid, s, NULL);
	  txt = (char *)&text;
	  if (txt[0] != 0)
	    print_word(text);
	  s += 2;
	}
      if (nb >= 50)
	printf("\"...");
      else
	printf("\"");
    }
}
