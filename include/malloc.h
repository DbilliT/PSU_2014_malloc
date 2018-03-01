/*
** malloc.h for malloc in /home/bechad_p/rendu/PSU_2014_malloc/include
** 
** Made by Pierre Bechade
** Login   <bechad_p@epitech.net>
** 
** Started on  Tue Feb 10 11:24:32 2015 Pierre Bechade
** Last update Sun Feb 15 14:43:12 2015 Pierre Bechade
*/

#ifndef MALLOC_H_
# define MALLOC_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct		s_block
{
  uint64_t		size;
  struct s_block	*next;
  struct s_block	*before;
  void			*ptr;
  char			data[1];
}			t_block;

# define INIT_MUT PTHREAD_MUTEX_INITIALIZER
# define STRUCT_SIZE sizeof(t_block)
# define TRUESIZE(x) (((((x) - 1) >> 3) << 3) + 8)
# define NEARPAGE(n) (((((n) - 1) >> 12) << 12) + 4096)
# define ERROR -1
# define FIRSTFIT 0
# define BESTFIT 1

/* IN SRC/MALLOC.C */
void	split(t_block **list, uint64_t size);
void	*add_block(t_block **list, uint64_t size);
void	*firstfit(t_block **list, uint64_t size);
void	*malloc(size_t taille);

/* IN SRC/REALLOC.C */
void	copy(t_block  *old, t_block *new);
void	*handle(void *ptr, t_block *tmp, uint64_t truesize);
void	*realloc(void *ptr, size_t taille);
void	*calloc(size_t number, size_t taille);
void	show_alloc_mem();

/* IN SRC/FREE.C */
void	block_fusion(t_block **list);
void	free(void *ptr);
t_block	*search_fit(t_block **list, int true_pos);
/* IN SRC/PARSE_CONF.C */
int	parse_conf();

extern void		*blocks;
extern uint64_t		mask;
extern pthread_mutex_t	mut1;
extern pthread_mutex_t	mut2;

#endif /* MALLOC_H */
