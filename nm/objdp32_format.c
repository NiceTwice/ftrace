/*
** objdp32_format.c for my_objdump in /home/fisun_s/rendu_tek2/PSU_2015_nmobjdump
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sat Feb 27 06:05:55 2016 Fisun Sergii
** Last update Sun May  1 18:52:59 2016 fisun_s
*/

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "objdump.h"

Elf32_Sym		**get_symtab_cpy32(Elf32_Sym *symtab,
					   unsigned int size)
{
  Elf32_Sym		**new;
  unsigned int		i;

  if ((new = malloc(sizeof(*new) * (size + 1))) == NULL)
    return (NULL);
  i = -1;
  while (++i < size)
    new[i] = &symtab[i];
  return (new);
}

static int		check_elf_format(Elf32_Ehdr *elf, char *path)
{
  if (elf->e_ident[EI_MAG0] != ELFMAG0
      || elf->e_ident[EI_MAG1] != ELFMAG1
      || elf->e_ident[EI_MAG2] != ELFMAG2
      || elf->e_ident[EI_MAG3] != ELFMAG3
      )
    {
      fprintf(stderr, "my_objdump: %s: File format not recognized\n", path);
      return (-1);
    }
  return (1);
}

static Elf32_Shdr	*set_sym_tab(Elf32_Shdr **strsymsection,
                                     char *strtab,
                                     Elf32_Ehdr *elf,
                                     Elf32_Shdr *shdr)
{
  int			i;
  Elf32_Shdr		*symsection;

  symsection = NULL;
  *strsymsection = NULL;
  i = -1;
  while (++i < elf->e_shnum)
    {
      if (shdr[i].sh_size)
        {
	  if (!strcmp(&strtab[shdr[i].sh_name], ".symtab"))
            symsection = (Elf32_Shdr*)&shdr[i];
	  if (!strcmp(&strtab[shdr[i].sh_name], ".strtab"))
            *strsymsection = (Elf32_Shdr*)&shdr[i];
        }
    }
  return (symsection);
}

void		print_line_sym32(Elf32_Sym **symtab_cpy,
				 char *strsymtab,
				 Elf32_Shdr *shdr,
				 int i)
{
  char		type;

  if (symtab_cpy[i]->st_name != 0
      && symtab_cpy[i]->st_name != 1)
    {
      if (symtab_cpy[i]->st_info != 4)
        {
          type = get_flag32(symtab_cpy[i], &shdr[symtab_cpy[i]->st_shndx],
			    &g_tab[(shdr[(symtab_cpy[i])->st_shndx]).sh_name]);
	  if (type != 'U' && type != 'w')
	    printf("%08x ", symtab_cpy[i]->st_value);
	  else
	    printf("%9c", ' ');
          printf("%c", type);
          printf(" %s\n", &strsymtab[symtab_cpy[i]->st_name]);
        }
    }
}

void			objdp32_format(void *data, char *filename,
				       int ac, int ok)
{
  Elf32_Ehdr		*elf;
  Elf32_Shdr		*shdr;
  Elf32_Sym		*symtab;
  Elf32_Shdr		*strsymsection;
  Elf32_Shdr		*symsection;
  char			*strsymtab;

  elf = (Elf32_Ehdr*)data;
  if (check_elf_format(elf, filename) == -1)
    return ;
  shdr = (Elf32_Shdr*)(data + elf->e_shoff);
  g_tab = (char*)(data + shdr[elf->e_shstrndx].sh_offset);
  symsection = set_sym_tab(&strsymsection, g_tab, elf, shdr);
  if (ac > 2 || ok == 1)
    printf("\n%s:\n", filename);
  if (strsymsection == NULL || symsection == NULL)
    {
      fprintf(stderr, "nm: %s: no symbols\n", filename);
      return ;
    }
  symtab = (Elf32_Sym*)((char*)data + symsection->sh_offset);
  strsymtab = (char*)((char*)data + strsymsection->sh_offset);
  print_sym32(symsection, symtab, strsymtab, shdr);
}
