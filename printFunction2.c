/*
** printFunction2.c for strace in /home/fisun_s/rendu_tek2/PSU_2015_strace
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sun Apr 10 21:09:03 2016 Fisun Sergii
** Last update Sun Apr 10 21:40:41 2016 Fisun Sergii
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

void		print_number(unsigned long number, pid_t pid)
{
  (void)pid;
  printf("%lld", (long long int)number);
}

void		print_addr(unsigned long number, pid_t pid)
{
  (void)pid;
  if (number == 0)
    printf("NULL");
  else
    printf("Ox%llx", (long long unsigned int)number);
}

void		print_int(unsigned long number, pid_t pid)
{
  (void)pid;
  printf("%d", (int)number);
}

void		print_hexa(unsigned long number, pid_t pid)
{
  (void)pid;
  if ((long)number < 0)
    printf("Ox-%lx", ((long)number) * -1);
  else
    printf("Ox%lx", number);
}
