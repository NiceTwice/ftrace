/*
** xfunc.c for my_objdump in /home/fisun_s/rendu_tek2/PSU_2015_nmobjdump
**
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
**
** Started on  Sat Feb 27 05:21:57 2016 Fisun Sergii
** Last update Sun Feb 28 18:26:08 2016 Fisun Sergii
*/

#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include "objdump.h"

char	by_shdr_name(const char *name)
{
  if (strcmp(name, ".bss") == 0)
    return ('b');
  else if (strcmp(name, ".code") == 0 || strcmp(name, ".fini") == 0 ||
	   strcmp(name, ".init") == 0 || strcmp(name, ".text") == 0 ||
	   strcmp(name, ".fini_array") == 0 || strcmp(name, ".init_array") == 0)
    return ('t');
  else if (strcmp(name, ".data") == 0)
    return ('d');
  if (strcmp(name, ".debug") == 0)
    return ('N');
  else if (strcmp(name, ".drectve") == 0 || strcmp(name, ".idata") == 0)
    return ('i');
  else if (strcmp(name, ".edata") == 0)
    return ('e');
  if (strcmp(name, ".pdata") == 0)
    return ('p');
  else if (strcmp(name, ".rdata") == 0 || strcmp(name, ".rodata") == 0)
    return ('r');
  else if (strcmp(name, ".sbss") == 0)
    return ('s');
  else if (strcmp(name, ".scommon") == 0)
    return ('c');
  if (strcmp(name, ".sdata") == 0)
    return ('g');
  return ('?');
}

char	by_shdr_type_and_flags(Elf32_Shdr *shdr)
{
  if ((shdr->sh_flags & SHF_WRITE) != SHF_WRITE)
    {
      if (shdr->sh_type == SHT_GROUP)
        return ('n');
      return ('r');
    }
  else if (shdr->sh_type == SHT_PROGBITS || shdr->sh_type == SHT_DYNAMIC)
    return ('d');
  return ('X');
}

char	shndx_and_bind_and_type32(Elf32_Sym *sym, int bind, int type)
{
  if (sym->st_shndx == SHN_COMMON)
    return ('C');
  else if (sym->st_shndx == SHN_UNDEF)
    {
      if (bind == STB_WEAK)
        {
          if (type == STT_OBJECT)
            return ('v');
          return ('w');
        }
      else
        return ('U');
    }
  if (bind == STB_WEAK)
    {
      if (type == STT_OBJECT)
        return ('V');
      return ('W');
    }
  else if (bind == STB_GNU_UNIQUE)
    return ('u');
  return ('?');
}

char	shndx_and_bind_and_type64(Elf64_Sym *sym, int bind, int type)
{
  if (sym->st_shndx == SHN_COMMON)
    return ('C');
  else if (sym->st_shndx == SHN_UNDEF)
    {
      if (bind == STB_WEAK)
	{
          if (type == STT_OBJECT)
            return ('v');
          return ('w');
        }
      else
        return ('U');
    }
  if (bind == STB_WEAK)
    {
      if (type == STT_OBJECT)
        return ('V');
      return ('W');
    }
  else if (bind == STB_GNU_UNIQUE)
    return ('u');
  return ('?');
}

void	*set_data(int fd, char *filename, unsigned int size)
{
  void	*data;

  if ((data = mmap(NULL, size,
		   PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
      close(fd);
      fprintf(stderr, "my_objdump: Warning: '%s' is not an ordinary file\n",
	      filename);
      return (MAP_FAILED);
    }
  return (data);
}
