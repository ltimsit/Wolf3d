/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltimsit- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 16:16:37 by ltimsit-          #+#    #+#             */
/*   Updated: 2019/11/14 16:22:15 by ltimsit-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <fcntl.h>
#include "wolf.h"

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

int main(int ac, char **v)
{
	// t_vector a;
	// t_vector b;
	// t_vector c;
	// t_vector d;
	t_vector result;

	t_vector a = {0.0, 0.0};
	t_vector b = {3.0, 3.0};
	t_vector c = {3.0, 0.0};
	t_vector d = {0.0, 3.0};
	result = line_intersection(a, b, c, d);
	printf("%f | %f\n", result.x, result.y);
}
