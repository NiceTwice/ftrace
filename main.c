/*
** main.c for strace in /home/fisun_s/rendu_tek2/PSU_2015_strace
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Wed Apr  6 11:18:50 2016 Fisun Sergii
** Last update Sun May  1 19:44:03 2016 fisun_s
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/user.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include "my_strace.h"
#include "syscalls.h"
#include "types.h"

pid_t				g_pid = 0;
void				(*g_printFunction)(struct user_regs_struct *,
						   pid_t) = NULL;
t_list				*g_symbols;
t_list				*g_callList;
t_list				*g_lastCall;
size_t				g_mainAddr;
char				*g_name;
char				gotMain;
char				ordFile;

int				checkInstruction(long instr,
						 struct user_regs_struct *regs,
						 pid_t child,
						 int *status)
{
  if ((instr & 0xFFFF) == 0x050F &&
      (ordFile == 0 || (ordFile == 1 && gotMain == 1)))
    {
      printf("Syscall ");
      g_printFunction(regs, child);
    }
  else if ((instr & 0xFF) == 0xe8 || (ordFile == 1 && regs->rip == g_mainAddr))
    {
      if (ordFile == 1 && regs->rip == g_mainAddr)
	{
	  gotMain = 1;
	  printFunctionName(regs->rip);
	  return (0);
	}
      if (ptrace(PTRACE_SINGLESTEP, child, 0, 0) == -1)
	return (-1);
      wait4(child, status, 0, NULL);
      ptrace(PTRACE_GETREGS, child, 0, regs);
      printFunctionName(regs->rip);
    }
  else if ((instr & 0xFF) == 0xC3)
    printFunctionNameLeave();
  return (0);
}

int				traceIt(pid_t child)
{
  int				status;
  long				instr;
  struct user_regs_struct	regs;
  struct user_regs_struct	regs_return;

  wait4(child, &status, 0, NULL);
  while (signal_continue(status))
    {
      ptrace(PTRACE_GETREGS, child, 0, &regs);
      instr = ptrace(PTRACE_PEEKTEXT, child, regs.rip, 0);
      if (checkInstruction(instr, &regs, child, &status) == -1)
	return (-1);
      if (ptrace(PTRACE_SINGLESTEP, child, 0, 0) == -1)
	return (-1);
      wait4(child, &status, 0, NULL);
      ptrace(PTRACE_GETREGS, child, 0, &regs_return);
      if ((instr & 0xFFFF) == 0x050F && regs.rax <= 313 &&
      	  g_syscalls[regs.rax].man_found != 0 &&
	  (ordFile == 0 || (ordFile == 1 && gotMain == 1)))
      	printReturnValue(&regs, &regs_return, child);
      check_signal(status);
    }
  kill(child, SIGINT);
  return (status);
}

int				ForkIt(char **env,
				       char **av)
{
  pid_t				child;
  char				*path;

  if ((path = get_path(av[1])) == NULL)
    return (printf("No such file\n"));
  g_symbols = get_symbols(path);
  ordFile = (g_symbols == NULL) ? 0 : 1;
  g_mainAddr = getMainAddr();
  g_name = path;
  if ((child = fork()) == -1)
    return (-1);
  if (child == 0)
    {
      if (ptrace(PTRACE_TRACEME, 0, 0, 0, 0) == -1)
	return (printf("Ptrace error\n"));
      if (execve(path, &av[1], env) == -1)
	return (printf("Execve error\n"));
    }
  else
    {
      g_pid = child;
      print_exit_status(traceIt(child));
    }
  return (0);
}

void				suppDoubles(t_list *list, int a)
{
  char				*tmp;

  tmp = list->data;
  while (list->data != NULL)
    {
      a++;
      if (list->next->data && strcmp(tmp, (char*)(list->next->data)) == 0)
	rm_elem(&g_lastCall, a);
      list = list->next;
    }
}

void				noDoubles()
{
  t_list			*tmp;
  int				a;

  a = 0;
  tmp = g_lastCall->next;
  while (tmp->data != NULL)
    {
      suppDoubles(tmp, a);
      a++;
      tmp = tmp->next;
    }
}

int				main(int ac, char **av, char **env)
{
  if (*env == NULL)
    return (printf("No env detected\n"));
  else if (ac  < 2)
    return (printf("Usage : ./ftrace [PATH]\n"));
  g_callList = create_list();
  g_lastCall = create_list();
  g_printFunction = &printSyscall;
  ForkIt(env, av);
  noDoubles();
  writeToDot();
  return (0);
}
