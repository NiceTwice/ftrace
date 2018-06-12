/*
** objdump.h for my_objdump in /home/fisun_s/rendu_tek2/PSU_2015_nmobjdump
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sat Feb 27 05:24:24 2016 Fisun Sergii
** Last update Sun May  1 18:58:12 2016 fisun_s
*/

#ifndef		_OBJDUMP_H_
# define	_OBJDUMP_H_

#include <elf.h>

char			*g_tab;

void			objdp32_format(void *data, char *filename, int ac, int);
void			objdp64_format(void *data, char *filename, int ac, int);
char			by_shdr_name(const char *);
char			by_shdr_type_and_flags(Elf32_Shdr*);
char			shndx_and_bind_and_type32(Elf32_Sym *, int, int);
char			shndx_and_bind_and_type64(Elf64_Sym *, int, int);
char			by_shdr_type_and_flags64(Elf64_Shdr *);
void			print_sym64(Elf64_Shdr *, Elf64_Sym *, char *, Elf64_Shdr *);
void			print_sym32(Elf32_Shdr *, Elf32_Sym *, char *, Elf32_Shdr *);
char			get_flag64(Elf64_Sym *, Elf64_Shdr *, char *);
char			get_flag32(Elf32_Sym *, Elf32_Shdr *, char *);
Elf64_Sym		**get_symtab_cpy(Elf64_Sym *, unsigned int);
Elf32_Sym		**get_symtab_cpy32(Elf32_Sym *, unsigned int);
void			print_line_sym(Elf64_Sym **, char *, Elf64_Shdr *, int);
void			print_line_sym32(Elf32_Sym **, char *, Elf32_Shdr *, int);
void			*set_data(int, char *, unsigned int);

#endif /* !_OBJDUMP_H_ */
