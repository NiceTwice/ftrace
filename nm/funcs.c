/*
** funcs.c for nm in /home/fisun_s/rendu_tek2/PSU_2015_nmobjdump/nm
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sun Feb 28 17:31:10 2016 Fisun Sergii
** Last update Sun Feb 28 18:20:21 2016 Fisun Sergii
*/

#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <ctype.h>
#include "objdump.h"

char			by_shdr_type_and_flags64(Elf64_Shdr *shdr)
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

char			get_flag32(Elf32_Sym *sym, Elf32_Shdr *shdr, char *name)
{
  char			c;
  int			bind;
  int			type;

  bind = ELF32_ST_BIND(sym->st_info);
  type = ELF32_ST_TYPE(sym->st_info);
  if ((c = shndx_and_bind_and_type32(sym, bind, type)) != '?')
    return (c);
  else if (sym->st_shndx == SHN_ABS)
    c = 'a';
  else if (shdr)
    {
      c = by_shdr_name(name);
      if (c == '?')
        c = by_shdr_type_and_flags(shdr);
    }
  return (bind == STB_GLOBAL ? toupper(c) : c);
}

void			print_sym32(Elf32_Shdr *symsection,
                                    Elf32_Sym *symtab,
                                    char *strsymtab,
                                    Elf32_Shdr *shdr)
{
  unsigned int		i;
  unsigned int		size;
  Elf32_Sym		**symtab_cpy;

  size = symsection->sh_size / sizeof(*symtab);
  if ((symtab_cpy = get_symtab_cpy32(symtab, size)) == NULL)
    {
      fprintf(stderr, "error: malloc: failed.\n");
      return ;
    }
  i = - 1;
  while (++i < size)
    print_line_sym32(symtab_cpy, strsymtab, shdr, i);
  free(symtab_cpy);
}

char			get_flag64(Elf64_Sym *sym, Elf64_Shdr *shdr, char *name)
{
  char		c;
  int		bind;
  int		type;

  bind = ELF64_ST_BIND(sym->st_info);
  type = ELF64_ST_TYPE(sym->st_info);
  if ((c = shndx_and_bind_and_type64(sym, bind, type)) != '?')
    return (c);
  if (sym->st_shndx == SHN_ABS)
    c = 'a';
  else if (shdr)
    {
      c = by_shdr_name(name);
      if (c == '?')
        c = by_shdr_type_and_flags64(shdr);
    }
  return (bind == STB_GLOBAL ? toupper(c) : c);
}

void			print_sym64(Elf64_Shdr *symsection,
                                    Elf64_Sym *symtab,
                                    char *strsymtab,
                                    Elf64_Shdr *shdr)
{
  unsigned int		i;
  unsigned int		size;
  Elf64_Sym		**symtab_cpy;

  size = symsection->sh_size / sizeof(*symtab);
  if ((symtab_cpy = get_symtab_cpy(symtab, size)) == NULL)
    {
      fprintf(stderr, "error: malloc: failed.\n");
      return ;
    }
  i = - 1;
  while (++i < size)
    print_line_sym(symtab_cpy, strsymtab, shdr, i);
  free(symtab_cpy);
}
