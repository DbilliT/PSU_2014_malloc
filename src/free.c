/*
** free.c for free in /home/bechad_p/rendu/PSU_2014_malloc/src
** 
** Made by Pierre Bechade
** Login   <bechad_p@epitech.net>
** 
** Started on  Tue Feb 10 11:32:26 2015 Pierre Bechade
** Last update Sun Feb 15 14:42:25 2015 Pierre Bechade
*/

#include "malloc.h"

void	block_fusion(t_block **list)
{
  (*list)->size = ((*list)->size ^ mask)
    + ((*list)->next->size ^ mask) + STRUCT_SIZE;
  (*list)->size = ((*list)->size | 0x01) << 63;
  (*list)->next = (*list)->next->next;
  if ((*list)->next != NULL)
    (*list)->next->before = (*list);
}

void		free(void *ptr)
{
  t_block	*tmp;

  pthread_mutex_lock(&mut1);
  tmp = blocks;
  while (tmp != NULL && tmp->data != ptr)
    tmp = tmp->next;
  if (tmp != NULL)
    {
      tmp->size = tmp->size | mask;
      if (tmp->next != NULL && ((tmp->next->size & mask) == mask))
	block_fusion(&tmp);
      if (tmp->before != NULL && ((tmp->before->size & mask) == mask))
	{
	  tmp = tmp->before;
	  block_fusion(&tmp);
	}
      else if (tmp->next == NULL)
	{
	  (tmp->before != NULL) ? tmp->before->next = NULL : (blocks = NULL);
	  brk(tmp);
	}
    }
  pthread_mutex_unlock(&mut1);
}

t_block		*search_fit(t_block **list, int true_pos)
{
  int		pos;
  t_block	*tmp;

  pos = 0;
  tmp = *list;
  while (pos != true_pos)
    tmp = tmp->next;
  tmp->size = (tmp->size ^ mask);
  return (tmp);
}
