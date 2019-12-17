/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltimsit- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 17:55:21 by ltimsit-          #+#    #+#             */
/*   Updated: 2019/11/14 17:25:00 by ltimsit-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <fcntl.h>
#include <sys/sysctl.h>
#include <unistd.h>

void print_map(t_map_info *map)
{
	size_t j;

	j = 0;
	while (j < map->hgt)
	{
		printf("%s\n", map->map[j]);
		j++;
	}
}

int main(int c, char **v)
{
//	t_sdl_data data;
	t_mlx dis;
	t_env env;
	t_map_info mI;
	t_gc gc;
	t_player player;

	(void)c;
	(void)v;
	printf("|| %ld ||", sysconf(_SC_NPROCESSORS_CONF));
	ft_bzero(&gc, sizeof(gc));
	ft_bzero(&dis, sizeof(dis));
	env.mI = &mI;
	env.gc = &gc;
	env.mlx = &dis;
	if (Create_map_tab(FILE_NAME, &env))
	{
		printf("%zu %zu test\n", env.mI->hgt, env.mI->len);
		print_map(env.mI);
	}
	player.edit = 0;
	player.x = 6.0;
	player.y = 3.0;
	player.angle_init = 0.0;
	player.angle = 1.5708;
	player.distance_min = -1;
	player.len_ray = 30.0;
	player.ray_a.x = player.x;
	player.ray_a.y = player.y;
	// player.ray_b.x = player.x + player.len_ray*cos(player.angle);
	// player.ray_b.y = player.y + player.len_ray*sin(player.angle);
	player.ray_b.x = -(player.len_ray)*sin(player.angle) + player.x;
	player.ray_b.y = player.len_ray*cos(player.angle) + player.y;
	// printf("%f ray_a x\n%f ray_a y\n", player.ray_a.x, player.ray_a.y);
	// printf("%f ray_b x\n%f ray_b y\n", player.ray_b.x, player.ray_b.y);
	env.player = &player;
	// parse_map_for_intersec(&env);
	printf("%s %p\n", v[0], dis.texture_names[0]);
	dis.texture_names[0] = ft_strdup(WALL_TEXTURE);
	dis.texture_names[1] = ft_strdup(PENGUIN);
	dis.texture_names[2] = ft_strdup(CEIL_TEXTURE);
	printf("%s %p\n", v[0], dis.texture_names[0]);
	dis.mlx_ptr = mlx_init();
	dis.win_ptr = mlx_new_window(dis.mlx_ptr, W_LEN, W_HGT, "Wolf3d");
	dis.img_ptr = mlx_new_image(dis.mlx_ptr, W_LEN, W_HGT);
	dis.minimap_ptr = mlx_new_image(dis.mlx_ptr, MINIMAP_LEN, MINIMAP_HGT);
	dis.img_data = get_data_ptr(dis.img_ptr);
	dis.minimap_data = get_data_ptr(dis.minimap_ptr);
	printf("%s %p\n", v[0], dis.texture_names[0]);
	load_texture(v[0], &env);
	mlx_hook(dis.win_ptr, 17, 0, quit_wolf, &env);
	mlx_hook(dis.win_ptr, 2, 0, key_press, &env);
	mlx_hook(dis.win_ptr, 4, 0, mouse_press, &env);
	mlx_loop_hook(dis.mlx_ptr, loop_wolf, &env);
	mlx_loop(dis.mlx_ptr);
/*
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (0);
	data.w_ptr = SDL_CreateWindow("Wolf3d", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, W_LEN, W_HGT, SDL_WINDOW_SHOWN);
	return (0);
	SDL_Delay(3000);
	data.renderer = SDL_CreateRenderer(data.w_ptr, -1, SDL_RENDERER_SOFTWARE);
	SDL_Quit();
	*/
	return (0);
}
