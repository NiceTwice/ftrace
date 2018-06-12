/*
** get_symbols.c for ftrace in /home/nicetwice/ftrace
** 
** Made by fisun_s
** Login   <fisun_s@epitech.eu>
** 
** Started on  Wed Apr 27 16:13:47 2016 fisun_s
** Last update Sun May  1 18:55:34 2016 fisun_s
*/

#include	<unistd.h>
#include	<string.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/wait.h>
#include	<sys/types.h>
#include	<math.h>
#include	"my_strace.h"

int		get_mul(char c, char *base)
{
  size_t	i;

  i = 0;
  while (i < strlen(base))
    {
      if (c == base[i])
	return (i);
      i++;
    }
  return (0);
}

size_t		get_size_t(char *str)
{
  size_t	ret;
  int		i;
  int		b;
  size_t	nb;

  b = 0;
  ret = 0;
  i = strlen(str) - 1;
  while (i >= 0)
    {
      nb = pow(16, b) * get_mul(str[i], "0123456789abcdef");
      ret += nb;
      b++;
      i--;
    }
  return (ret);
}

char		*get_exec_commande(char *str)
{
  char		*ret;
  size_t	a;

  a = 7;
  if ((ret = malloc(sizeof(char) * (strlen(str) + 15))) == NULL)
    return (NULL);
  ret[0] = '.';
  ret[1] = '/';
  ret[2] = 'm';
  ret[3] = 'y';
  ret[4] = '_';
  ret[5] = 'n';
  ret[6] = 'm';
  ret[7] = ' ';
  while ((++a - 8) < strlen(str))
    ret[a] = str[a - 8];
  ret[a] = ' ';
  ret[a + 1] = '>';
  ret[a + 2] = ' ';
  ret[a + 3] = 't';
  ret[a + 4] = 'm';
  ret[a + 5] = 'p';
  ret[a + 6] = 0;
  return (ret);
}

int		extractFromFile(t_list *l, FILE *file, char *tmp)
{
  t_symbol	*symb;
  char		*line;
  size_t	len;
  size_t	tour;

  tour = 0;
  line = NULL;
  len = 0;
  while (getline(&line, &len, file) != EOF)
    {
      tour++;
      if ((symb = malloc(sizeof(t_symbol))) == NULL)
	return (1);
      if (strlen(line) < 19)
	return (1);
      tmp = strncpy(tmp, line, 16);
      symb->addr = get_size_t(tmp);
      symb->type = line[17];
      line[strlen(line) - 1] = 0;
      symb->name = strdup(&(line[19]));
      put_in_list_last(&l, symb);
    }
  if (tour == 0)
    return (1);
  return (0);
}

t_list		*get_symbols(char *str)
{
  pid_t		pid;
  FILE		*file;
  char		*command;
  char		*tmp;
  t_list	*ret;

  ret = create_list();
  if ((tmp = malloc(sizeof(char) * 17)) == NULL)
    return (NULL);
  tmp[16] = 0;
  command = get_exec_commande(str);
  pid = fork();
  if (pid == 0)
    {
      execlp("/bin/sh", "bin/sh", "-c", command, NULL);
      exit(0);
    }
  else
    wait(NULL);
  file = fopen("tmp", "r+");
  if (file == NULL || extractFromFile(ret, file, tmp) == 1)
    return (NULL);
  free(command);
  return (ret);
}
