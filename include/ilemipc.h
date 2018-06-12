/*
** ilemipc.h for lemipc in /home/fisun_s/rendu_tek2/PSU_2015_lemipc
** 
** Made by Fisun Sergii
** Login   <fisun_s@epitech.net>
** 
** Started on  Thu Mar 24 16:00:37 2016 Fisun Sergii
** Last update Fri Apr  1 14:51:00 2016 Fisun Sergii
*/

#ifndef		ILEMIPC_H_
# define	ILEMIPC_H_

# include	<sys/sem.h>
# include	"../lemipc.h"

# define	LENGTH	50
# define	HEIGHT	50

typedef struct	my_msg
{
  long		mtype;
  char		str[32];
}		t_msg;

typedef struct	s_list
{
  void		*data;
  struct s_list	*next;
  struct s_list	*prev;
}		t_list;

typedef struct	s_game
{
  int		shm_id;
  int		sem_id;
  int		msg_id;
  int		x;
  int		y;
  int		id;
  int		nTeam;
  struct s_case	*map;
  t_msg		msg;
  t_list	*teamList;
  struct sembuf	sops;
}		t_game;

typedef struct	s_pos
{
  int		x;
  int		y;
}		t_pos;

typedef struct	s_player
{
  t_pos		pos;
  int		id;
  int		nTeam;
}		t_player;

void		rm_elem(t_list **, int);
int		list_size(t_list *);
int		put_in_list_last(t_list **, void *);
t_list		*create_list();
void		defineMoveAdv(t_game *, t_player *, t_pos *, t_pos *);
void		defineMove(t_game *, t_player *, t_pos *, t_pos *);
void		getMove(t_game *, t_player *, t_pos *);
int		movePlayer(t_game *, t_player *);
void		init_map(struct s_case *, int);
int		getRandomPosition(t_game *, t_player *);
void		printMap(t_game *);
void		removePlayerFromMap(t_game *, t_player *);
int		getNextId(t_game *);
void		addPlayer(t_game *, int, int);
int		isEnoughtPlayers(t_list *);
int		checkForNewPlayers(t_game *);
int		checkForNearPlayers(t_game *, t_player *);
int		isNum(char *);
int		checkDoublesInIntTab(int *, int, int);
void		waitForPlayers(t_game *);
void		gameLoop(t_game *);
void		destroyGame(t_game *);
t_game		*initHostGame(key_t, int);
void		hostGame(key_t, int);
void		hostSigInt();
void		connectedSigInt();
void		connectGame(int, int, int);
void		sendEmptyMsg(int, int);
void		destroyIPCS(t_game *);
void		antiSegfaultTool(t_game *, t_player *, t_pos *);
void		attemptForMessage(int, t_game *, t_msg *);

#endif
