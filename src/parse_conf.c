/*
** parse_conf.c for parse_conf in /home/bechad_p/rendu/PSU_2014_malloc/src
** 
** Made by Pierre Bechade
** Login   <bechad_p@epitech.net>
** 
** Started on  Tue Feb 10 13:43:08 2015 Pierre Bechade
** Last update Sun Feb 15 14:41:49 2015 Pierre Bechade
*/

#include "malloc.h"

int		parse_conf()
{
  static int	first = 0;
  int		fd;
  char		buff[4096];

  if (first == 0)
    {
      if ((fd = open("etc/malloc.conf", O_RDONLY)) == -1)
	return (ERROR);
      if (read(fd, buff, 4096) == -1)
	return (ERROR);
      if (close(fd) == -1)
	return (ERROR);
      first = 1;
    }
  if (strcmp("ALGO = BESTFIT;", buff) == 0)
    return (BESTFIT);
  return (FIRSTFIT);
}
