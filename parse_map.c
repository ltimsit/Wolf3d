/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltimsit- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 12:18:27 by ltimsit-          #+#    #+#             */
/*   Updated: 2019/11/14 17:26:35 by ltimsit-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "fcntl.h"
#include "unistd.h"
#include "wolf.h"

int				get_line_gc(int fd, char **line, t_gc *gc)
{
	int ret;

	if ((ret = ft_get_next_line(fd, line, 0)))
	{
		if (ret == -1)
			return (-1);
		if (!(ft_add_to_gc(*line, gc)))
		{
			ft_putendl_fd("Error in get_line_gc", STDERR_FILENO);
			return (-1);
		}
	}
	return (ret);
}

int				get_header(int fd, t_env *env)
{
	int ret;
	char *line;
	char *cmd;

	if ((ret = get_line_gc(fd, &line, env->gc)) == -1)
	{
		ft_putendl_fd("Error in get_header", STDERR_FILENO);
		return (0);
	}
	if ((cmd = ft_strstr(line, "height")))
	{
		if (!(env->mI->hgt = (size_t)ft_atoi(cmd + 6)))
		{
			ft_putendl_fd("Error in get_header: height = 0", STDERR_FILENO);
			free(line);
			return (0);
		}
	}
	if ((cmd = ft_strstr(line, "length")))
	{
		if (!(env->mI->len = (size_t)ft_atoi(cmd + 6)))
		{
			ft_putendl_fd("Error in get_header: length = 0", STDERR_FILENO);
			free(line);
			return (0);
		}
	}
	// printf("%zu %zu\n", env->mI->len, env->mI->hgt);
	// free(line);
	return (1);
}

int			Create_map_tab(char *file_name, t_env *env)
{
	int ret;
	int fd;
	char *line;
	size_t i;

	if ((fd = open(file_name, O_RDONLY)) == -1)
		ft_putendl_fd("Open error in Create_map_tab", STDERR_FILENO);
	if (!get_header(fd, env))
		return (0);
	if (!(env->mI->map = (char **)malloc(sizeof(char *) * env->mI->hgt)))
	{
		ft_putendl_fd("Malloc error in Create_map_tab", STDERR_FILENO);
		return (0);
	}
	i = 0;
	while ((ret = get_line_gc(fd, &line, env->gc)) && i < env->mI->hgt)
	{
		if (ret == -1)
			return (0);
		if (ft_strlen(line) != env->mI->len)
		{
			ft_putendl_fd("Error line size different than header", STDERR_FILENO);
			free(line);
			return (0);
		}
		env->mI->map[i++] = line;
	}
	return (1);
}
