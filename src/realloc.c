/*
** realloc.c for realloc in /home/bechad_p/rendu/PSU_2014_malloc/src
** 
** Made by Pierre Bechade
** Login   <bechad_p@epitech.net>
** 
** Started on  Tue Feb 10 11:33:02 2015 Pierre Bechade
** Last update Sun Feb 15 13:51:24 2015 Pierre Bechade
*/

#include "malloc.h"

void		copy(t_block  *old, t_block *new)
{
  uint64_t	*olddata;
  uint64_t	*newdata;
  uint64_t	i;

  i = 0;
  olddata = old->ptr;
  newdata = new->ptr;
  while ((i * 8) < old->size && (i * 8) < new->size)
    {
      newdata[i] = olddata[i];
      i++;
    }
}

void		*handle(void *ptr, t_block *tmp, uint64_t truesize)
{
  void		*p;
  t_block	*new;

  if ((p = malloc(truesize)) == NULL)
    {
      pthread_mutex_unlock(&mut2);
      return (NULL);
    }
  new = blocks;
  while (new != NULL && new->data != p)
    new = new->next;
  copy(tmp, new);
  free(ptr);
  pthread_mutex_unlock(&mut2);
  return (p);
}

void		*realloc(void *ptr, size_t size)
{
  t_block	*tmp;
  uint64_t	truesize;

  pthread_mutex_lock(&mut2);
  if (ptr == NULL)
    return ((pthread_mutex_unlock(&mut2) != 0) ? NULL : malloc(size));
  tmp = blocks;
  while (tmp != NULL && tmp->ptr != ptr)
    tmp = tmp->next;
  if (tmp == NULL)
    return ((pthread_mutex_unlock(&mut2) != 0) ? NULL : NULL);
  if (tmp->size >= (truesize = TRUESIZE(size)))
    (tmp->size - truesize >= (STRUCT_SIZE + 8)) ? split(&tmp, truesize) : 0;
  else if (tmp->next && ((tmp->next->size & mask) == mask)
	   && ((tmp->size + tmp->next->size + STRUCT_SIZE) >= truesize))
    {
      block_fusion(&tmp);
      tmp->size = (tmp->size | 0x00) << 63;
      (tmp->size - truesize >= (STRUCT_SIZE + 8)) ? split(&tmp, truesize) : 0;
    }
  else
    return (handle(ptr, tmp, truesize));
  return ((pthread_mutex_unlock(&mut2) != 0) ? NULL : ptr);
}

void		*calloc(size_t number, size_t size)
{
  uint64_t	*ptr;
  uint64_t	s;
  uint64_t	i;

  pthread_mutex_lock(&mut2);
  i = 0;
  if (number == 0 || size == 0)
    {
      pthread_mutex_unlock(&mut2);
      return (NULL);
    }
  ptr = malloc(number * size);
  s = TRUESIZE((number * size));
  while ((i * 8) < s)
    {
      ptr[i] = 0;
      i++;
    }
  pthread_mutex_unlock(&mut2);
  return (ptr);
}

void		show_alloc_mem()
{
  t_block	*tmp;

  tmp = blocks;
  printf("break : %p\n", sbrk(0));
  while (tmp != NULL)
    {
      printf("%p - %p : %lu octets\n", tmp, tmp->data, tmp->size);
      tmp = tmp->next;
    }
}
