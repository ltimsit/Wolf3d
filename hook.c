/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltimsit- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 17:02:32 by ltimsit-          #+#    #+#             */
/*   Updated: 2019/11/14 17:27:25 by ltimsit-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"


int load_texture(char *base_path, t_env *env)
{
	int i;
	char path[128];
	int x;
	int y;
	int size;

	i = -1;
	ft_bzero(path, 128);
	while (++i < NB_TEXTURE)
	{
		size = ft_strrchr(base_path, '/') - base_path;
		ft_strncat(path, base_path, size);
		ft_strcat(path, "/img/");
		ft_strcat(path, env->mlx->texture_names[i]);
		env->mlx->texture_images[i] = mlx_xpm_file_to_image(env->mlx->mlx_ptr, path, &x, &y);
		env->mlx->texture_dimensions[i][0] = x;
		env->mlx->texture_dimensions[i][1] = y;
		// printf("%s, %d %d\n", path, x, y);
		env->mlx->texture_datas[i] = get_data_ptr(env->mlx->texture_images[i]);
		path[0] = '\0';
	}
	return (1);
}

void move_verti(t_env *env, double speed)
{
	double new_x;
	double new_y;

	new_x = -speed*sin(env->player->angle_init);
	new_y = speed*cos(env->player->angle_init);
	if (new_x + env->player->x < 0 || new_y + env->player->y < 0 || (int)new_x + env->player->x > env->mI->len - 1
		|| new_y + env->player->x > env->mI->hgt - 1
		|| env->mI->map[(int)(new_y + env->player->y)][(int)(new_x + env->player->x)] == 'x')
		return;
	env->player->x += new_x;
	env->player->y += new_y;
	env->player->ray_a.x = env->player->x;
	env->player->ray_a.y = env->player->y;
	env->player->ray_b.x += new_x;
	env->player->ray_b.y += new_y;
	if (env->player->edit)
	{
		env->player->edit_coord.x = (double)((int)(env->player->x + -3.0 * sin(env->player->angle_init)));
		env->player->edit_coord.y = (double)((int)(env->player->y + 3.0*cos(env->player->angle_init)));
	}
}

int		key_press(int keycode, t_env *env)
{
	if (keycode == 53)
		quit_wolf(env);
	if (keycode == 123)
	{
		env->player->angle_init -= 5 * (M_PI/180);
		if (env->player->edit)
		{
			env->player->edit_coord.x = (double)((int)(env->player->x + -3.0 * sin(env->player->angle_init)));
			env->player->edit_coord.y = (double)((int)(env->player->y + 3.0 * cos(env->player->angle_init)));
		}
	}
	if (keycode == 124)
	{
		env->player->angle_init += 5 * (M_PI/180);
		if (env->player->edit)
		{
			env->player->edit_coord.x = (double)((int)(env->player->x + -3.0 * sin(env->player->angle_init)));
			env->player->edit_coord.y = (double)((int)(env->player->y + 3.0 * cos(env->player->angle_init)));
		}
	}
	if (keycode == 15 || keycode == 17)
		env->player->len_ray += keycode == 15 ? -0.5 : 0.5;
	if (keycode == 126)
	{
		move_verti(env, 0.1);
	}
	if (keycode == 125)
	{
		move_verti(env, -0.1);
	}
	if (keycode == 12 || keycode == 13)
	{
		env->player->x += keycode == 12 ? 0.1 : -0.1;
		env->player->ray_a.x = env->player->x;
		env->player->ray_b.x = -(env->player->len_ray)*sin(env->player->angle) + env->player->x;
		env->player->ray_b.y = env->player->len_ray*cos(env->player->angle) + env->player->y;
	}
	if (keycode == 49)
	{
		printf("player pos: %f, %f angle = %f\n", env->player->x, env->player->y, env->player->angle * 180/M_PI);
	}
	if (keycode == 35)
	{
		env->player->edit = !env->player->edit;
		env->player->edit_coord.x = (double)((int)(env->player->x + -3.0 * sin(env->player->angle_init)));
		env->player->edit_coord.y = (double)((int)(env->player->y + 3.0 * cos(env->player->angle_init)));
		// printf("%f %f\n", env->player->edit_coord.x, env->player->edit_coord.y);
	}
	if (keycode == 31)
		{
			if (env->player->edit_coord.x >= 0 && env->player->edit_coord.x >= 0)
				env->mI->map[(int)env->player->edit_coord.y][(int)env->player->edit_coord.x] = 'x';
		}
	return (0);
}

int		mouse_press(int button, int x, int y, t_env *env)
{
	if (button == 1)
		return (0);
	return (1);
}

void fill_top_bot(t_env *env)
{
	int *int_data = (int *)env->mlx->img_data;
	int i;
	int j;
	int color;

	j = -1;
	while (++j < W_HGT)
	{
		i = -1;
		while (++i < W_LEN)
		{
			if (j < W_HGT/2)
				color = 0x3333ff;
			else
				color = 0x995599;
			int_data[j * W_LEN + i] = color;
		}
	}
}

void fill_minimap(t_env *env)
{
	int i;
	int j;
	int *data;
	double x;
	double y;
	int xbis;
	int ybis;

	x = env->player->x;
	y = env->player->y;
	data = (int *)env->mlx->minimap_data;
	j = -1;
	while (++j < MINIMAP_HGT)
	{
		i = -1;
		while (++i < MINIMAP_LEN)
		{
			xbis = (int)(x - (sqrt((j / 12 - y) * (j / 12 - y) + (i / 12 - x) * (i / 12 - x))) * sin(env->player->angle_init));
			ybis = (int)(y + (sqrt((j / 12 - y) * (j / 12 - y) + (i / 12 - x) * (i / 12 - x))) * cos(env->player->angle_init));
			if (xbis >= 0 && ybis >= 0 && xbis < env->mI->len && ybis < env->mI->hgt)
			{
				if (env->mI->map[ybis][xbis] == 'x')
					data[j * MINIMAP_LEN + i] = 0x00ff00;
				else
					data[j * MINIMAP_LEN + i] = 0x002222;
			}
			else
				data[j * MINIMAP_LEN + i] = 0x555555;
		}
	}
}

int		loop_wolf(t_env *env)
{
	env->player->angle = env->player->angle_init;
	mlx_clear_window(env->mlx->mlx_ptr, env->mlx->win_ptr);
	fill_top_bot(env);
	// fill_minimap(env);
	spam_raycast(env);
	// mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx->img_ptr, 0, 0);
	// mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx->minimap_ptr, 20, 20);
	mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx->texture_images[1], 20, 20);

	return (0);
}

double double_abs(double a)
{
	return (a < 0 ? -a : a);
}

void set_player_ray(double angle, t_env *env)
{
	env->player->angle = angle;
	env->player->ray_b.x = -(env->player->len_ray)*sin(env->player->angle) + env->player->x;
	env->player->ray_b.y = env->player->len_ray*cos(env->player->angle) + env->player->y;
}

int get_color(double distance)
{
	int color;
	int shadow;
	char * color_tab = (char *)&color;

	color_tab[0] = 0xff;
	color_tab[1] = 0xff;
	color_tab[2] = 0xff;
	// if ((shadow = distance * 10) <= 0xff)
		// color_tab[2] = 0xff - shadow;
	// else color_tab[2] = 0;
	color_tab[3] = 0xff;

	return (color);
}

int get_color_fog(int color, double hgt)
{
	char *color_tab;
	int i;

	i = -1;
	color_tab = (char *)&color;
	while (++i < 3)
	{
		// color_tab[i] += 100 * hgt;
		color_tab[i] = (int)color_tab[i] + 100 * hgt < 0xff ? color_tab[i] + 100 * hgt : 0xff;
	}
	return (color);
}

int get_color_from_texture(t_collision inter, double hgt, t_env *env)
{
	double ratio_x;
	double ratio_y;
	double ratio;
	int *data_texture;
	int id;
	int x;
	int y;


	if (inter.type == 'x')
		id = 0;
	if (inter.type == 'p')
		id = 1;

	x = env->mlx->texture_dimensions[id][0];
	y = env->mlx->texture_dimensions[id][1];

	// printf("%f %f, \n", inter.col_coord.x, inter.col_coord.y);
	data_texture = (int *)env->mlx->texture_datas[id];
	ratio = inter.col_coord.x - (int)inter.col_coord.x;
	// printf("%f", ratio);
	if (ratio == 0.0)
		ratio = (inter.col_coord.y - (int)inter.col_coord.y) * x;
	else
		ratio *= x;
	// printf("ratio y: %f, ratio x: %f\n", hgt * 236, ratio);
	return (data_texture[(int)(hgt * y) * x + (int)ratio]);
}

void put_inter_in_image(t_collision inter, int horiz_pos, t_env *env)
{
	double distance;
	t_vector norme;
	int j;
	int j_max;
	int i;
	double hgt;
	double len;

	int *int_data = (int *)env->mlx->img_data;

	if (inter.col_coord.x == 0.0 && inter.col_coord.y == 0.0)
	{
		distance = env->player->len_ray * cos(env->player->angle_init - env->player->angle);
		inter.col_coord = env->player->ray_b;
	}
	else
		distance = inter.distance * cos(env->player->angle_init - env->player->angle);
	j = ((distance - 1) * W_HGT)/(2 * distance);
	j_max = ((distance + 1) * W_HGT)/(2 * distance);
	hgt = (double)j_max - (double)j;
	i = -1;
	while (j < j_max && ++i != -1)
	{
		// int_data[j * W_LEN + horiz_pos] = get_color(distance);
		// printf("%d, %f\n", i, (double)(i / hgt));
		if (j >= 0 && j < W_HGT)
		{
			// if (inter.type == ELEM_X)
				// int_data[j * W_LEN + horiz_pos] = get_color_from_texture(inter, (double)(i / hgt), env);
			if (inter.type == ELEM_FOG)
				int_data[j * W_LEN + horiz_pos] = get_color_fog(int_data[j * W_LEN + horiz_pos], (double)(i / hgt));
			else
				int_data[j * W_LEN + horiz_pos] = get_color_from_texture(inter, (double)(i / hgt), env);
		}
		j++;
	}
	i = -1;
	// len = W_MAX - j_max;
	double currentDist;
	double floorXWall = inter.col_coord.x;
	double floorYWall = inter.col_coord.y;
		int *data_texture;
		data_texture = (int *)env->mlx->texture_datas[2];
	while (j < W_HGT && ++i != -1)
	{
		currentDist = W_HGT / (2.0 * j - W_HGT); //you could make a small lookup table for this instead

		double weight = currentDist / distance;

		double currentFloorX = weight * floorXWall + (1.0 - weight) * env->player->x;
		double currentFloorY = weight * floorYWall + (1.0 - weight) * env->player->y;

		int floorTexX, floorTexY;
		floorTexX = (int)(currentFloorX * env->mlx->texture_dimensions[2][0]) % env->mlx->texture_dimensions[2][0];
		floorTexY = (int)(currentFloorY * env->mlx->texture_dimensions[2][1]) % env->mlx->texture_dimensions[2][1];

		//floor
		// buffer[j][x] = (texture[3][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
		int_data[j * W_LEN + horiz_pos] = data_texture[env->mlx->texture_dimensions[2][0] * floorTexY + floorTexX];
		int_data[(W_HGT - j) * W_LEN + horiz_pos] = data_texture[env->mlx->texture_dimensions[2][0] * floorTexY + floorTexX];
		//ceiling (symmetrical!)
		// buffer[W_HGT - j][x] = texture[6][texWidth * floorTexY + floorTexX];
		j++;
	}

}

void *thread_calcul(void *env_void)
{
	int i;
	t_collision inter;
	t_env *env;

	env = (t_env *)env_void;
	i = env->th_info.modulo;
	// printf("th %p\n", env);
	// printf("%p %p\n", env, env->player);
	while (env->th_info.angle_min < env->th_info.angle_max && i < W_LEN)
	{
		// if (i % NB_THREAD == env->th_info.modulo)
		// {
			set_player_ray(env->th_info.angle_min, env);
			inter = parse_map_for_intersec(env);
			put_inter_in_image(inter, i, env);
		// }
		i += 4;
		env->th_info.angle_min += env->th_info.offset;
	}
	// mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx->img_ptr, 0, 0);
	pthread_exit(NULL);
	return (NULL);
}

void spam_raycast(t_env *env)
{
	int i;
	double offset;
	double angle_min;
	double angle_max;
	t_vector inter;
	pthread_t tab_thread[NB_THREAD];
	t_env	env_copy[NB_THREAD];
	t_player player_th[NB_THREAD];

	angle_min = env->player->angle - (30 * M_PI / 180);
	angle_max = env->player->angle + (30 * M_PI / 180);
	offset = (60 * M_PI / 180) / W_LEN;
	i = -1;
	while (++i < NB_THREAD)
	{
		ft_memcpy(&(env_copy[i]), env, sizeof(*env));
		ft_memcpy(&(player_th[i]), env->player, sizeof(t_player));
		env_copy[i].player = &(player_th[i]);
		env_copy[i].th_info.modulo = i;
		env_copy[i].th_info.angle_min = angle_min + (i * offset);
		env_copy[i].th_info.angle_max = angle_max;
		env_copy[i].th_info.offset = offset * 4;
		pthread_create(tab_thread + i, NULL, thread_calcul, env_copy + i);
	}
	while (--i >= 0)
	{
		pthread_join(tab_thread[i], NULL);
		mlx_put_image_to_window(env->mlx->mlx_ptr, env->mlx->win_ptr, env->mlx->img_ptr, 0, 0);
	}
	// i = -1;
	// while (angle_min < angle_max && ++i != -1)
	// {
	// 	pthread_create(tab_thread + (i % 4), NULL, thread_calcul, env_copy + (i % 4))
	// 	set_player_ray(angle_min, env);
	// 	inter = parse_map_for_intersec(env);
	// 	// if (angle_min >= env->player->angle_init - 0.001 && angle_min <= env->player->angle_init + 0.001)
	// 	// {
	// 	// 	print_vector(inter, "central intersection", 1, env);
	// 	// 	// printf("%f\n", sqrt(inter.x * inter.x + inter.y * inter.y) * cos(angle_min));
	// 	// }
	// 	put_inter_in_image(inter, i, env);
	// 	angle_min += offset;
	// }
	// printf("i = %d\n", i);
}

void print_vector(t_vector vec, char *name, int option, t_env *env)
{
	t_vector norme;

	printf("{(%s) x: %.3f | y: %.3f}\n", name, vec.x, vec.y);
	if (option == 1)
	{
		norme.x = vec.x - env->player->x;
		norme.y = vec.y - env->player->y;
		printf("norme x = %f norme y = %f\n", norme.x, norme.y);
		printf("distance = %f\n", sqrt(norme.x * norme.x + norme.y * norme.y)); //* cos(env->player->angle));
	}
}

t_vector create_vector(double x, double y)
{
	t_vector result;
	result.x = x;
	result.y = y;
	return (result);
}

int check_if_valid_and_better(t_vector tocheck, t_collision *curr_best, t_vector player_pos, char elem)
{
	t_vector dist;
	double x;
	double y;
	double x2;
	double y2;
	double distance;
	if (tocheck.x == 0.0 && tocheck.y == 0.0)
		return (0);

	// if (elem == 'p')
	// {
	// 	tocheck.x = tocheck.x - (int)tocheck.x + 0.5;
	// 	tocheck.y = tocheck.y - (int)tocheck.y + 0.5;
	// }
	x = tocheck.x - player_pos.x;
	y = tocheck.y - player_pos.y;
	// x2 = curr_best->col_coord.x - player_pos.x;
	// y2 = curr_best->col_coord.y - player_pos.y;
	distance = x * x + y * y;
	if ((curr_best->col_coord.x == 0.0 && curr_best->col_coord.y == 0.0) || distance < curr_best->distance)
	{
		// *curr_best = tocheck;
		curr_best->col_coord = tocheck;
		curr_best->distance = distance;
		// curr_best->type = ELEM_X;
		return (1);
	}
	return (0);
	// x = tocheck.x - player_pos.x;
	// y = tocheck.y - player_pos.y;
	// x2 = curr_best->x - player_pos.x;
	// y2 = curr_best->y - player_pos.y;
	// if (distance < x2 * x2 + y2 * y2)
	// {
	// 	curr_best->col_coord = tocheck;
	// 	curr_best->distance = sqrt(distance);
	// 	// *curr_best = tocheck;
	// }
	// return;
}

int is_character(char to_test)
{
	int i;
	char *elem;

	elem = ELEMENT_CHARS;
	i = -1;
	while (++i < NB_ELEMENTS)
	{
		if (to_test == elem[i])
			return (1);
	}
	return (0);
}

void rotate_v_on_center(t_vector *v1, t_vector *v2, double angle)
{
	v2->y -= 0.5 * sin(-angle);
	v2->x -= 0.5 - (cos(-angle) * 0.5);
	v1->x -= 0.5 - (cos(angle) * 0.5);
	v1->y -= 0.5 * sin(angle);
}

t_collision parse_map_for_intersec(t_env *env)
{
	int i;
	int j;
	int k;
	t_vector tmp;
	t_vector c;
	t_vector d;
	t_vector result[4];
	t_vector up_l;
	t_vector up_r;
	t_vector dwn_l;
	t_vector dwn_r;
	// t_vector curr_best;
	t_collision curr_best_col;
	int j_min;
	int j_max;
	int i_min;
	int i_max;
	char tested_elem;
	int test;

	test = 0;

	i_min = env->player->x - env->player->len_ray - 1;
	j_min = env->player->y - env->player->len_ray - 1;
	i_max = env->player->x + env->player->len_ray;
	j_max = env->player->y + env->player->len_ray;
	if (i_min < -1)
		i_min = -1;
	if (j_min < -1)
		j_min = -1;
	if (j_max > env->mI->hgt)
		j_max = env->mI->hgt;
	if (i_max > env->mI->len)
		i_max = env->mI->len;
	// curr_best.x = 0.0;
	// curr_best.y = 0.0;
	curr_best_col.col_coord.x = 0.0;
	curr_best_col.col_coord.y = 0.0;
	curr_best_col.distance = 0.0;
	curr_best_col.type = ELEM_FOG;
	// j = -1;
	// printf("test %p\n", env->mI);
	// printf("%d %d, %d %d\n", i_min, i_max, j_min, j_max);
	while (++j_min < j_max)
	{
		// printf("test2\n");
		i = i_min;
		while (++i < i_max)
		{
			test = 0;
			if (env->player->edit && (j_min == (int)env->player->edit_coord.y && i == (int)env->player->edit_coord.x))
			{
				printf("j = %d i = %d\n", j_min, i);
				test = 1;
			}
			if (is_character((tested_elem = env->mI->map[j_min][i])) || test)
			{
				if (tested_elem == 'x' || test)
				{
					if (test)
					 tested_elem = 'x';
				up_l = create_vector((double)i, (double)j_min);
				up_r = create_vector((double)i + 1.0, (double)j_min);
				dwn_l = create_vector((double)i, (double)j_min + 1.0);
				dwn_r = create_vector((double)i + 1.0, (double)j_min + 1.0);

				result[0] = line_intersection(env->player->ray_a, env->player->ray_b, up_l, up_r);
				if (check_if_valid_and_better(result[0], &curr_best_col, env->player->ray_a, tested_elem))
					curr_best_col.type = tested_elem;
				result[1] = line_intersection(env->player->ray_a, env->player->ray_b, up_r, dwn_r);
				if (check_if_valid_and_better(result[1], &curr_best_col, env->player->ray_a, tested_elem))
					curr_best_col.type = tested_elem;
				result[2] = line_intersection(env->player->ray_a, env->player->ray_b, dwn_l, dwn_r);
				if (check_if_valid_and_better(result[2], &curr_best_col, env->player->ray_a, tested_elem))
					curr_best_col.type = tested_elem;
				result[3] = line_intersection(env->player->ray_a, env->player->ray_b, up_l, dwn_l);
				if (check_if_valid_and_better(result[3], &curr_best_col, env->player->ray_a, tested_elem))
					curr_best_col.type = tested_elem;
				}
				else if (tested_elem == 'p')
				{
					up_l = create_vector((double)i , (double)j_min + 0.5);
					up_r = create_vector((double)i + 1.0, (double)j_min + 0.5);
					// print_vector(up_l,"v1", 0, env);
					// print_vector(up_r,"v2", 0, env);
					rotate_v_on_center(&up_l, &up_r, env->player->angle_init);
					// if (!test)
					// {
					// 	print_vector(up_l, "v1", 0, env);
					// 	print_vector(up_r, "v2", 0, env);
					// 	test = 1;
					// }
					// print_vector(up_l, "v1", 0, env);
					// print_vector(up_r, "v2", 0, env);
					result[0] = line_intersection(env->player->ray_a, env->player->ray_b, up_l, up_r);
					if (check_if_valid_and_better(result[0], &curr_best_col, env->player->ray_a, tested_elem))
						curr_best_col.type = tested_elem;
				}
				// k = -1;
				// while (++k < 4)
				// {
				// 	if (result[k].x != 0 || result[k].y != 0)
				// 	{
				// 		printf("%d: ", k);
				// 		print_vector(env->player->ray_a, "a");
				// 		print_vector(env->player->ray_b, "b");
				// 		// print_vector(c, "c");
				// 		// print_vector(d, "d");
				// 		printf("%d %d ->> %f | %f\n", j_min, i, result[k].x, result[k].y);
				// 	}
				// }
			}
		}
	}
	// printf("coord = %f %f | dist = %f", curr_best_col.col_coord.x, curr_best_col.col_coord.y, curr_best_col.distance);
	if (!(curr_best_col.col_coord.x == 0.0 && curr_best_col.col_coord.y == 0.0))
		curr_best_col.distance = sqrt(curr_best_col.distance);
	return (curr_best_col);
}
