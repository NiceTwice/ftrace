/*
** objdump.c for my_objdump in /home/fisun_s/rendu_tek2/PSU_2015_nmobjdump
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sat Feb 27 05:22:09 2016 Fisun Sergii
** Last update Sun May  1 18:29:34 2016 fisun_s
*/

#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <ar.h>
#include "objdump.h"

char			*get_name(char *str)
{
  char			*ret;
  int			i;

  i = 0;
  while (str[i] != '/')
    i++;
  ret = malloc(sizeof(char) * (i + 1));
  i = 0;
  while (str[i] != '/')
    {
      ret[i] = str[i];
      i++;
    }
  ret[i] = 0;
  return (ret);
}

void			exec_obj(void *ar_st, char *name, int ac, int ok)
{
  if (((Elf32_Ehdr*)ar_st)->e_ident[EI_CLASS] == ELFCLASS32)
    objdp32_format(ar_st, name, ac, ok);
  else if (((Elf32_Ehdr*)ar_st)->e_ident[EI_CLASS] == ELFCLASS64)
    objdp64_format(ar_st, name, ac, ok);
  else
    fprintf(stderr, "./my_nm: %s: File format not recognized\n", name);
}

int			gere_lib(void *data, long size, int ac)
{
  struct ar_hdr		*ar;
  char			*names;
  long			i;
  char			*name;

  if ((i = SARMAG) != -1 && strncmp(ARMAG, data, SARMAG) == 0)
    {
      while ((ar = (struct ar_hdr*)(data + i)) &&  i < size)
	{
	  if (strncmp((char*)ar, "//", 2) == 0)
	    names = (char*)((char*)ar + sizeof(struct ar_hdr));
	  else if ((strncmp((char*)ar, "/", 1) == 0 && ((char*)ar)[1] !=
		    ' ') || strncmp((char*)ar, "/", 1) != 0)
	    {
	      if (strncmp((char*)ar, "/", 1) == 0 && ((char*)ar)[1] != ' ')
		name = get_name(&names[atoi((char*)((char*)ar + 1))]);
	      else
		name = get_name((char*)ar);
	      exec_obj((void*)((char*)ar + sizeof(struct ar_hdr)), name, ac, 1);
	    }
	  i += sizeof(struct ar_hdr) + atoi(ar->ar_size);
	}
      return (1);
    }
  return (-1);
}

static void		my_objdump(char *filename, int ac)
{
  void			*data;
  int			fd;
  long			size;

  if ((fd = open(filename, O_RDONLY, 0)) == -1)
    {
      fprintf(stderr, "my_objdump: %s: no such file\n", filename);
      return ;
    }
  if ((size = lseek(fd, 0, SEEK_END)) == -1)
    {
      fprintf(stderr, "error: lseek: %s\n", strerror(errno));
      return ;
    }
  if ((data = set_data(fd, filename, size)) == MAP_FAILED)
    return ;
  if (gere_lib(data, size, ac) == 1);
  else
    exec_obj(data, filename, ac, 0);
  if (close(fd) == -1)
    fprintf(stderr, "error: close: %s\n", strerror(errno));
  if (munmap(data, size) == -1)
    fprintf(stderr, "error: munmap: %s\n", strerror(errno));
}

int			main(int ac, char **av)
{
  int			i;

  if (ac > 1)
    {
      i = 0;
      while (++i < ac)
	my_objdump(av[i], ac);
    }
  else
    my_objdump("./a.out", ac);
  return (1);
}
