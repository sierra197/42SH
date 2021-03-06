/*
** option_red_right.c for option_red_right in /home/dovan_n//svn/biji-repo1/src
** 
** Made by noel dovan
** Login   <dovan_n@epitech.net>
** 
** Started on  Fri Apr  5 19:26:40 2013 noel dovan
** Last update Tue May 14 18:08:38 2013 noel dovan
*/

#include "data.h"
#include "option_red.h"
#include "misc.h"
#include "sh.h"
#include "create.h"

int	option_red_right_write(char *buf, int fd, int r)
{
  if (write(fd, buf, r) == -1)
    return (1);
  if (close(fd) == -1)
    return (1);
  return (0);
}

int	option_red_right_filspere(char *buff, int *pipefd)
{
  int	fd;
  char	*buf;
  int	r;

  fd = open(buff, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if ((buf = malloc(sizeof(*buf) * SIZE)) == NULL || fd == -1)
    return (1);
  if ((r = read(pipefd[0], buf, SIZE)) == -1)
    {
      free(buf);
      return (1);
    }
  buf[r] = '\0';
  if (option_red_right_write(buf, fd, r) == 1)
    {
      free(buf);
      return (1);
    }
  free(buf);
  return (0);
}

int	option_red_right_filsfils(char *buf, t_all *all, int *pipefd,
				  char **check)
{
  char	*buff;

  if (close(pipefd[0]) == -1)
    return (1);
  if ((buff = get_side_value(buf, '>', 0)) == NULL)
    return (1);
  epur_begin_end(buff);
  if (dup2(pipefd[1], 1) == -1)
    {
      free(buff);
      return (1);
    }
  if ((all = check_buf(buff, all, check))->nul == 1)
    {
      free(buff);
      return (1);
    }
  free(buff);
  return (0);
}

int	option_red_right_fils(char *buf, t_all *all, int *pipefd, char **check)
{
  pid_t	pid;
  char	*buff;

  if ((pid = fork()) == -1)
    return (1);
  if (pid == 0)
    {
      option_red_right_filsfils(buf, all, pipefd, check);
      return (1);
    }
  else
    {
      if (close(pipefd[1]) == -1)
	return (1);
      if ((buff = get_side_value(buf, '>', 1)) == NULL)
	return (1);
      epur_begin_end(buff);
      if (check_correct_red(buff) == 1)
        return (1);
      if (option_red_right_filspere(buff, pipefd) == 1)
	return (1);
    }
  free(buff);
  return (0);
}

t_all	*option_red_right(char *buf, t_all *all, char **check)
{
  int	pipefd[2];
  pid_t	pid;

  if (pipe(pipefd) == -1)
    return (do_nul(all));
  if ((pid = fork()) == -1)
    return (do_nul(all));
  if (pid == 0)
    {
      check_red_begin(buf, ">");
      option_red_right_fils(buf, all, pipefd, check);
      my_printf(" \b");
      return (do_nul(all));
    }
  else
    {
      if (wait(0) == -1)
	return (do_nul(all));
      if (dup2(1, pipefd[1]) == -1)
	return (do_nul(all));
      if (dup2(0, pipefd[0]) == -1)
	return (do_nul(all));
    }
  return (all);
}
