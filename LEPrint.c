/*
** LEPrint.c for ftrace in /home/nicetwice/PSU_2015_ftrace
** 
** Made by fisun_s
** Login   <fisun_s@epitech.eu>
** 
** Started on  Sun May  1 18:40:50 2016 fisun_s
** Last update Sun May  1 19:38:15 2016 fisun_s
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

extern pid_t			g_pid;
extern t_list			*g_symbols;
extern t_list			*g_callList;
extern t_list			*g_lastCall;
extern size_t			g_mainAddr;
extern char			*g_name;
extern char			gotMain;
extern char			ordFile;

void				printFunctionNameLeave()
{
  t_list			*tmp;

  tmp = g_callList->prev;
  if (tmp->data != NULL)
    {
      printf("Leaving function %s\n", (char *)(tmp->data));
      if (strcmp(tmp->data, "main") == 0)
	gotMain = 0;
      rm_elem(&g_callList, list_size(g_callList) - 1);
    }
}

void				ordinaryFilePrint(char *str, size_t addr,
						  char *myBool)
{
  t_list			*tmp;

  if (gotMain == 1)
    {
      *myBool = 1;
      tmp = g_symbols->next;
      while (tmp->data != NULL && ((t_symbol*)tmp->data)->addr != addr)
	tmp = tmp->next;
      if (g_symbols != NULL && tmp->data != NULL)
	sprintf(str, "%s", ((t_symbol*)tmp->data)->name);
      else
	sprintf(str, "func_0x%lX@%s", addr, g_name);
      printf("Entering function %s at %lx\n", str, addr);
    }
}

void				printFunctionName(size_t addr)
{
  char				*str;
  char				*dox;
  char				myBool;

  myBool = 0;
  if ((str = malloc(sizeof(char) * 50)) == NULL ||
      (dox = malloc(sizeof(char) * 100)) == NULL)
    exit(1);
  if (g_symbols == NULL && (myBool = 1) == 1)
    {
      sprintf(str, "func_0x%lX@%s", addr, g_name);
      printf("Entering function func_0x%lX@%s at %lx\n", addr, g_name, addr);
    }
  else
    ordinaryFilePrint(str, addr, &myBool);
  if (myBool == 1 && (t_list*)(g_callList->prev)->data != NULL)
    {
      sprintf(dox, "\"%s\" -> \"%s\";\n",
	      ((char*)(t_list*)(g_callList->prev)->data), str);
      if (g_lastCall->prev->data == NULL ||
	  strcmp(g_lastCall->prev->data, dox) != 0)
	put_in_list_last(&g_lastCall, dox);
    }
  if (myBool == 1)
    put_in_list_last(&g_callList, str);
}

void				writeToDot()
{
  t_list			*tmp;
  int				fd;

  creat("./graph", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if ((fd = open("./graph", O_CREAT | O_RDWR)) == -1)
    return;
  tmp = g_lastCall->next;
  write(fd, "digraph G {\n", strlen("digraph G {\n"));
  while (tmp->data != NULL)
    {
      write(fd, (char*)(tmp->data), strlen((char*)(tmp->data)));
      tmp = tmp->next;
    }
  write(fd, "}\n", 2);
  close(fd);
}

size_t				getMainAddr()
{
  t_list			*tmp;

  if (g_symbols == NULL)
    return (0);
  tmp = g_symbols->next;
  while (tmp->data != NULL &&
	 strcmp(((t_symbol*)(tmp->data))->name, "main") != 0)
    {
      tmp = tmp->next;
    }
  if (tmp->data != NULL)
    return (((t_symbol*)(tmp->data))->addr);
  return (0);
}
