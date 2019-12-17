/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltimsit- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 16:56:37 by ltimsit-          #+#    #+#             */
/*   Updated: 2019/11/14 17:01:56 by ltimsit-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <stdlib.h>

int quit_wolf(t_env *env)
{
		ft_free_gc(env->gc);
		exit(0);
		return (0);
}

char	*get_data_ptr(void *img_ptr)
{
	char	*data_ptr;
	int		bit_pp;
	int		size_line;
	int		endian;

	data_ptr = mlx_get_data_addr(img_ptr, &bit_pp, &size_line, &endian);
	return (data_ptr);
}

// void get_player_vector(t_env *env)
// {
// 	x = env->player->x;
// 	y = env->player->y;
// 	angle = env->player->angle;
// }

t_vector subb_vectors(t_vector v1, t_vector v2)
{
	t_vector result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	return result;
}


t_vector	line_intersection(t_vector a, t_vector b, t_vector c, t_vector d)
{
	t_vector r;
	t_vector s;
	double p;
	double u;
	double t;
	t_vector result = {0.0, 0.0};

	// result = {0, 0};
	r = subb_vectors(b, a);
	s = subb_vectors(d, c);
	p = r.x *	s.y - r.y * s.x;
	u = ((c.x - a.x) *r.y - (c.y - a.y) * r.x) / p;
	t = ((c.x - a.x) *s.y - (c.y - a.y) * s.x) / p;
	if (0 <= u && u <= 1 && 0 <= t && t <= 1)
	{
		result.x = a.x + t * r.x;
		result.y = a.y + t * r.y;
	}
	return (result);
}
