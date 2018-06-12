/*
** objdp64_format.c for my_objdump in /home/fisun_s/rendu_tek2/PSU_2015_nmobjdump
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sat Feb 27 06:05:55 2016 Fisun Sergii
** Last update Sun May  1 18:52:11 2016 fisun_s
*/

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "objdump.h"

extern char		*g_tab;

Elf64_Sym		**get_symtab_cpy(Elf64_Sym *symtab,
                                         unsigned int size)
{
  Elf64_Sym		**new;
  unsigned int		i;

  if ((new = malloc(sizeof(*new) * (size + 1))) == NULL)
    return (NULL);
  i = -1;
  while (++i < size)
    new[i] = &symtab[i];
  return (new);
}

static int		check_elf_format(Elf64_Ehdr *elf, char *path)
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

static Elf64_Shdr	*set_sym_tab(Elf64_Shdr **strsymsection,
				     char *strtab,
                                     Elf64_Ehdr *elf,
                                     Elf64_Shdr *shdr)
{
  int			i;
  Elf64_Shdr		*symsection;

  symsection = NULL;
  *strsymsection = NULL;
  i = -1;
  while (++i < elf->e_shnum)
    {
      if (shdr[i].sh_size)
        {
          if (!strcmp(&strtab[shdr[i].sh_name], ".symtab"))
            symsection = (Elf64_Shdr*)&shdr[i];
          if (!strcmp(&strtab[shdr[i].sh_name], ".strtab"))
            *strsymsection = (Elf64_Shdr*)&shdr[i];
        }
    }
  return (symsection);
}

void			print_line_sym(Elf64_Sym **symtab_cpy,
                                       char *strsymtab,
                                       Elf64_Shdr *shdr,
                                       int i)
{
  char			type;

  if (symtab_cpy[i]->st_name != 0
      && symtab_cpy[i]->st_name != 1)
    {
      if (symtab_cpy[i]->st_info != 4)
        {
          type = get_flag64(symtab_cpy[i], &shdr[symtab_cpy[i]->st_shndx],
			    &g_tab[(shdr[(symtab_cpy[i])->st_shndx]).sh_name]);
          if (type != 'U' && type != 'w')
            printf("%08x%08x ", (uint32_t)(symtab_cpy[i]->st_value >> 32),
                   (uint32_t)symtab_cpy[i]->st_value);
          else
            printf("%17c", ' ');
          printf("%c", type);
          printf(" %s\n", &strsymtab[symtab_cpy[i]->st_name]);
        }
    }
}

void			objdp64_format(void *data, char *filename,
				       int ac, int ok)
{
  Elf64_Ehdr		*elf;
  Elf64_Shdr		*shdr;
  Elf64_Sym		*symtab;
  Elf64_Shdr		*strsymsection;
  Elf64_Shdr		*symsection;
  char			*strsymtab;

  elf = (Elf64_Ehdr*)data;
  if (check_elf_format(elf, filename) == -1)
    return ;
  shdr = (Elf64_Shdr*)(data + elf->e_shoff);
  g_tab = (char*)(data + shdr[elf->e_shstrndx].sh_offset);
  symsection = set_sym_tab(&strsymsection, g_tab, elf, shdr);
  if (ac > 2 || ok == 1)
    printf("\n%s:\n", filename);
  if (strsymsection == NULL || symsection == NULL)
    {
      fprintf(stderr, "nm: %s: no symbols\n", filename);
      return ;
    }
  symtab = (Elf64_Sym*)((char*)data + symsection->sh_offset);
  strsymtab = (char*)((char*)data + strsymsection->sh_offset);
  print_sym64(symsection, symtab, strsymtab, shdr);
}
