/*
** double_list.c for  in /home/fisun_s
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Sat Dec 27 19:02:16 2014 Fisun Sergii
** Last update Wed Apr 27 16:48:01 2016 fisun_s
*/

#include	<unistd.h>
#include	<stdlib.h>
#include	<string.h>
#include	"my_strace.h"

void		rm_elem(t_list **list, int a)
{
  int		b;
  t_list	*tmp;

  b = 0;
  tmp = *list;
  tmp = tmp->next;
  while (b != a)
    {
      b++;
      tmp = tmp->next;
    }
  tmp->prev->next = tmp->next;
  tmp->next->prev = tmp->prev;
  free(tmp);
}

int		list_size(t_list *list)
{
  int		a;

  a = 0;
  list = list->next;
  while (list->data != NULL)
    {
      a++;
      list = list->next;
    }
  return (a);
}

void		my_show_list(t_list *list)
{
  list = list->next;
  while (list->data != NULL)
    {
      write(1, list->data, strlen(list->data));
      write(1, "\n", 1);
      list = list->next;
    }
}

int		put_in_list_last(t_list **list, void *data)
{
  t_list	*elem;

  if ((elem = malloc(sizeof(*elem))) == NULL)
    return (1);
  elem->data = data;
  elem->prev = (*list)->prev;
  elem->next = *list;
  (*list)->prev->next = elem;
  (*list)->prev = elem;
  return (0);
}

t_list		*create_list()
{
  t_list	*list;

  if ((list = malloc(sizeof(*list))) == NULL)
    return (NULL);
  list->data = NULL;
  list->next = list;
  list->prev = list;
  return (list);
}
