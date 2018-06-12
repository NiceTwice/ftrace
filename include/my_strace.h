/*
** my_strace.h for strace in /home/fisun_s/rendu_tek2/PSU_2015_strace
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Thu Apr  7 16:50:17 2016 Fisun Sergii
** Last update Sun May  1 18:45:56 2016 fisun_s
*/

#ifndef		MY_STRACE_H_
# define	MY_STRACE_H_

# include	<unistd.h>
# include	<sys/user.h>

typedef struct  s_list
{
  void          *data;
  struct s_list *next;
  struct s_list *prev;
}               t_list;

typedef struct	s_symbol
{
  size_t	addr;
  char		type;
  char		*name;
}		t_symbol;

typedef struct	s_all
{
  char		isS;
  char		isP;
  char		*str;
}		t_all;

typedef struct	s_syscalls
{
  unsigned	num;
  char		*name;
  char		man_found;
  int		nparams;
  void		(*type_return)(unsigned long, pid_t);
  void		(*param1)(unsigned long, pid_t);
  void		(*param2)(unsigned long, pid_t);
  void		(*param3)(unsigned long, pid_t);
  void		(*param4)(unsigned long, pid_t);
  void		(*param5)(unsigned long, pid_t);
  void		(*param6)(unsigned long, pid_t);
}		t_syscalls;

void	rm_elem(t_list **, int);
int	list_size(t_list *);
int	put_in_list_last(t_list **, void *);
t_list	*create_list();
void	print_number(unsigned long, pid_t);
void	print_addr(unsigned long, pid_t);
void	print_void(unsigned long, pid_t);
void	print_char_p(unsigned long, pid_t);
void	print_hexa(unsigned long, pid_t);
void	print_int(unsigned long, pid_t);
void	print_struct_stat_p(unsigned long, pid_t);
void	free_wordtab(char ***);
char	**my_str_to_wordtab(char *, char);
char	*get_path(char *);
int	signal_continue(int);
void	check_signal(int);
void	printSyscall(struct user_regs_struct *regs, pid_t pid);
void	printSyscallHex(struct user_regs_struct *regs, pid_t pid);
t_all	*checkParams(int ac, char **av);
void	printReturnValue(struct user_regs_struct *regs,
			 struct user_regs_struct *return_regs, pid_t pid);
int	traceIt(pid_t child);
int	print_exit_status(int status);
void	trace_pid_handler();
int	ForkIt(char **env, char **av);
t_list	*get_symbols(char *str);
void	printFunctionNameLeave();
void	ordinaryFilePrint(char *, size_t, char *);
void	printFunctionName(size_t);
void	writeToDot();
size_t	getMainAddr();

#endif	/* !MY_STRACE_H_ */
