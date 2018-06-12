/*
** my_str_to_wordtab.c for strace in /home/fisun_s/rendu_tek2/PSU_2015_strace
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sun Apr 10 15:39:50 2016 Fisun Sergii
** Last update Sun Apr 10 15:44:48 2016 Fisun Sergii
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

static int	get_nb_word(char *s, char sep)
{
  int		nb;
  int		is_char;

  nb = 0;
  while (s && *s)
  {
    is_char = 0;
    while (*s && *s == sep)
      s += 1;
    while (*s && *s != sep && ((is_char = 1)))
      s += 1;
    while (*s && *s == sep)
      s += 1;
    if (is_char == 1)
      nb += 1;
  }
  return (nb);
}

static char	*get_this_word(char *s, char sep)
{
  char		*new;
  char		*end;
  int		size;
  int		i;

  end = s;
  while (*end && *end != sep)
    end += 1;
  size = end - s;
  if ((new = (char *)malloc(size + 1)) == (char *)0)
    return ((char *)0);
  new[size] = 0;
  i = 0;
  while (i < size)
    new[i++] = *s++;
  new[i] = 0;
  return (new);
}

static int	get_words(char **tab, char *s, char sep)
{
  int		i;

  i = 0;
  while (s && *s)
  {
    while (*s && *s == sep)
      s += 1;
    if ((tab[i++] = get_this_word(s, sep)) == (char *)0)
      return (-1);
    while (*s && *s != sep)
      s += 1;
    while (*s && *s == sep)
      s += 1;
  }
  return (0);
}

char		**my_str_to_wordtab(char *s, char sep)
{
  char		**tab;
  int		nb_word;

  if (!s)
    return (NULL);
  nb_word = get_nb_word(s, sep);
  if ((tab = (char **)malloc(sizeof(char *) * (nb_word + 1))) == NULL
      || get_words(tab, s, sep) == -1)
  {
    return (NULL);
  }
  tab[nb_word] = NULL;
  return (tab);
}

void		free_wordtab(char ***wordtab)
{
  char		**tab;
  size_t	i;

  i = 0;
  tab = *wordtab;
  while (tab[i])
  {
    free(tab[i]);
    i += 1;
  }
  free(tab[i]);
  free(tab);
  *wordtab = (char **)0;
}
