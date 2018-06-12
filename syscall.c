/*
** syscall.c for strace in /home/fisun_s/rendu_tek2/PSU_2015_strace
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sun Apr 10 21:16:13 2016 Fisun Sergii
** Last update Sun May  1 17:39:07 2016 fisun_s
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
#include "signames.h"

extern t_list			*g_callList;
extern t_list			*g_lastCall;

void				printSyscall(struct user_regs_struct *regs,
					     pid_t pid)
{
  int				i;
  char				*dox;

  if ((t_list*)(g_callList->prev)->data != NULL
      && (dox = malloc(sizeof(char) * 100)) != NULL)
    {
      sprintf(dox, "\"%s\" -> \"%s\";\n", (char*)g_callList->prev->data,
	      g_syscalls[regs->rax].name);
      put_in_list_last(&g_lastCall, dox);
    }
  if ((i = 0) == 0 && (regs->rax > 313 || g_syscalls[regs->rax].man_found == 0))
    return;
  printf("%s(", g_syscalls[regs->rax].name);
  if (i++ < g_syscalls[regs->rax].nparams)
    (g_syscalls[regs->rax].param1)(regs->rdi, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    (g_syscalls[regs->rax].param2)(regs->rsi, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    (g_syscalls[regs->rax].param3)(regs->rdx, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    (g_syscalls[regs->rax].param4)(regs->rcx, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    (g_syscalls[regs->rax].param5)(regs->r8, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    (g_syscalls[regs->rax].param6)(regs->r9, pid);
}

void				printSyscallHex(struct user_regs_struct *regs,
						pid_t pid)
{
  int				i;

  i = 0;
  if (regs->rax > 313 || g_syscalls[regs->rax].man_found == 0)
    return;
  printf("%s(", g_syscalls[regs->rax].name);
  if (i++ < g_syscalls[regs->rax].nparams)
    print_hexa(regs->rdi, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    print_hexa(regs->rsi, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    print_hexa(regs->rdx, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    print_hexa(regs->rcx, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    print_hexa(regs->r8, pid);
  if (i++ < g_syscalls[regs->rax].nparams && printf(", ") != 0)
    print_hexa(regs->r9, pid);
}

t_all				*checkParams(int ac, char **av)
{
  t_all				*all;

  if (ac < 2)
    return (NULL);
  if ((all = malloc(sizeof(t_all))) == NULL)
    return (NULL);
  all->isS = 0;
  all->isP = 0;
  all->str = NULL;
  if (strcmp(av[1], "-s") == 0)
    all->isS = 1;
  if (all->isS == 1 && ac < 3)
    return (NULL);
  if (strcmp(av[1 + all->isS], "-p") == 0)
    {
      all->isP = 1;
      if (ac <= (1 + all->isS + 1))
        return (NULL);
      all->str = av[1 + all->isS + 1];
    }
  else
    all->str = av[1 + all->isS];
  return (all);
}

void			printReturnValue(struct user_regs_struct *regs,
					 struct user_regs_struct *return_regs,
					 pid_t pid)
{
  printf(")\t= ");
  if (g_syscalls[regs->rax].type_return == VOID)
    printf("?");
  else
    {
      (g_syscalls[regs->rax].type_return)(return_regs->rax, pid);
    }
  printf("\n");
}

int			print_exit_status(int status)
{
  if (status == -1)
    return (-1);
  if (WIFEXITED(status))
    printf("+++ exited with %d +++\n", WEXITSTATUS(status));
  else if (WIFSTOPPED(status))
    printf("+++ Killed by %s +++\n", g_signames[WSTOPSIG(status)]);
  return (0);
}
