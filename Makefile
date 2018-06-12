##
## Makefile for PSU_2015_ftrace
##
## Made by	Antoine Maciazek
## Login	maciaz_a
##
## Started on	Sun May 01 19:48:25 2016 Antoine Maciazek
## Last update	Sun May 01 19:48:35 2016 Antoine Maciazek
##

NAME	= ftrace

NM_SRCS	=	./nm/funcs.c \
		./nm/objdp32_format.c \
		./nm/objdp64_format.c \
		./nm/objdump.c \
		./nm/xfunc.c

SRCS	=	main.c \
		printFunctions.c \
		printFunction2.c \
		my_str_to_wordtab.c \
		paths.c \
		signal.c \
		syscall.c \
		get_symbols.c \
		double_list.c \
		LEPrint.c

OBJS	= $(SRCS:.c=.o)
NM_OBJS	= $(NM_SRCS:.c=.o)

CFLAGS	= -I include -Wall -W -Wextra -lm

all: $(NAME)

$(NAME): $(NM_OBJS) $(OBJS)
	gcc -o my_nm $(NM_OBJS)
	gcc -lm -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(NM_OBJS)

fclean: clean
	rm -f $(NAME)
	rm -f ./my_nm

re: fclean all
