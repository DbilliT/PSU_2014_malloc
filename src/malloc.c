/*
** malloc.c for malloc in /home/bechad_p/rendu/PSU_2014_malloc/src
** 
** Made by Pierre Bechade
** Login   <bechad_p@epitech.net>
** 
** Started on  Tue Feb 10 11:32:39 2015 Pierre Bechade
** Last update Sun Feb 15 14:42:35 2015 Pierre Bechade
*/

#include "malloc.h"

void		*blocks = NULL;
uint64_t	mask = 0;
pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

void		split(t_block **list, uint64_t size)
{
  t_block	*new;

  new = (t_block *)((*list)->data + size);
  new->size = (*list)->size - size - STRUCT_SIZE;
  new->size = (new->size | 0x01) << 63;
  new->ptr = new->data;
  new->next = (*list)->next;
  new->before = *list;
  if (new->next != NULL)
    new->next->before = new;
  (*list)->next = new;
  (*list)->size = size;
}

void		*add_block(t_block **list, uint64_t size)
{
  t_block	*new;
  t_block	*tmp;

  if (size == 0 || ((new = sbrk(0)) == (void *) - 1))
    return ((pthread_mutex_unlock(&mut1) != 0) ? NULL : NULL);
  if (sbrk(NEARPAGE(STRUCT_SIZE + size)) == (void *) - 1)
    return ((pthread_mutex_unlock(&mut1) != 0) ? NULL : NULL);
  new->size = size;
  new->ptr = new->data;
  new->next = NULL;
  new->before = NULL;
  if (list == NULL)
    blocks = new;
  else
    {
      tmp = *list;
      while (tmp->next != NULL)
      	tmp = tmp->next;
      tmp->next = new;
      new->before = tmp;
    }
  pthread_mutex_unlock(&mut1);
  return (new->data);
}

void		*firstfit(t_block **list, uint64_t size)
{
  t_block	*tmp;
  int		bool;

  bool = 0;
  tmp = *list;
  while (tmp != NULL && bool == 0)
    {
      if (((tmp->size & mask) == mask) && size <= (tmp->size ^ mask))
        {
          tmp->size = (tmp->size ^ mask);
          bool = 1;
        }
      else
	tmp = tmp->next;
    }
  if (tmp == NULL)
    return (add_block(list, size));
  if (tmp->size - size >= (STRUCT_SIZE + 8))
    split(&tmp, size);
  pthread_mutex_unlock(&mut1);
  return (tmp->data);
}

void		*bestfit(t_block **list, uint64_t size)
{
  t_block	*tmp;
  int		pos;
  int		true_pos;
  uint64_t	good;

  tmp = *list;
  good = 0;
  pos = 0;
  true_pos = -1;
  while (tmp != NULL)
    {
      if (((tmp->size & mask) == mask) && size <= (tmp->size ^ mask))
	(good > (tmp->size ^ mask)) ? good = tmp->size ^ mask, true_pos = pos : 0;
      tmp = tmp->next;
      pos++;
    }
  if (true_pos != -1)
    tmp = search_fit(list, true_pos);
  if (tmp == NULL)
    return (add_block(list, size));
  if (tmp->size - size >= (STRUCT_SIZE + 8))
    split(&tmp, size);
  pthread_mutex_unlock(&mut1);
  return (tmp->data);
}

void		*malloc(size_t size)
{
  t_block	*block;
  uint64_t	truesize;
  int		res;

  pthread_mutex_lock(&mut1);
  mask = (mask | 0x01) << 63;
  truesize = TRUESIZE(size);
  if (size == 0)
    {
      pthread_mutex_unlock(&mut1);
      return (NULL);
    }
  if (blocks == NULL)
    return (add_block(NULL, truesize));
  else
    {
      block = blocks;
      if ((res = parse_conf()) == ERROR)
       	return (NULL);
      else if (res == FIRSTFIT)
	return (firstfit(&block, truesize));
      return (bestfit(&block, truesize));
    }
}
