/*
** paths.c for strace in /home/debruy_p/projects/tek2/unixSystem/PSU_2015_strace
** 
** Made by Pierre Debruyne
** Login   <debruy_p@epitech.eu>
** 
** Started on  Sun Apr 10 21:29:52 2016 Pierre Debruyne
** Last update Sun May  1 18:56:24 2016 fisun_s
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
#include "syscalls.h"
#include "types.h"

size_t		my_strlen(char *s)
{
  return ((s) ? (strlen(s)) : (0));
}

char		*my_strcat(char *dest, char *src)
{
  return ((dest && src) ? (strcat(dest, src)) : (dest));
}

char		*concat_path_file(char *s1, char *s2)
{
  char		*str;
  size_t	size;

  size = my_strlen(s1) + my_strlen(s2);
  if ((str = (char *)malloc(size + 2)) == NULL)
    return (NULL);
  if (str)
  {
    memset(str, 0, size + 2);
    my_strcat(str, s1);
    strcat(str, "/");
    my_strcat(str, s2);
  }
  return (str);
}

char		*get_path(char *file)
{
  size_t	i;
  char		**path_tab;
  char		*tmp;

  if (file[0] == '.' || file[0] == '/')
    return (strdup(file));
  else if ((path_tab = my_str_to_wordtab(getenv("PATH"), ':')))
    {
      i = 0;
      while (path_tab[i])
	{
	  tmp = concat_path_file(path_tab[i], file);
	  if (tmp && access(tmp, X_OK) != -1)
	    {
	      free_wordtab(&path_tab);
	      return (tmp);
	    }
	  free(tmp);
	  i += 1;
	}
      free_wordtab(&path_tab);
    }
  return ((char *)0);
}
