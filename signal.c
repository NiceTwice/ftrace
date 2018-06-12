/*
** signal.c for strace in /home/fisun_s/rendu_tek2/PSU_2015_strace
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sun Apr 10 21:11:06 2016 Fisun Sergii
** Last update Sun Apr 10 21:24:41 2016 Pierre Debruyne
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

extern	pid_t	g_pid;

int				signal_continue(int status)
{
  return (WIFSTOPPED(status)
          && (WSTOPSIG(status) == SIGTRAP
              || WSTOPSIG(status) == SIGSTOP));
}

void				check_signal(int status)
{
  if (WIFSTOPPED(status)
      && WSTOPSIG(status) != SIGTRAP)
    printf("--- %s ---\n", g_signames[WSTOPSIG(status)]);
}

void				trace_pid_handler()
{
  ptrace(PTRACE_DETACH, g_pid, 0, 0);
  ptrace(PTRACE_CONT, g_pid, 0, 0);
  exit(EXIT_FAILURE);
}
