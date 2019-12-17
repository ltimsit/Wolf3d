/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltimsit- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 18:00:10 by ltimsit-          #+#    #+#             */
/*   Updated: 2019/11/14 18:56:47 by ltimsit-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H
#include "include/SDL2/SDL.h"
#include "mlx.h"
#include "libft/libft.h"
#include <math.h>
#include <pthread.h>

#define NB_TEXTURE 3

#define ELEM_X 'x'
#define ELEM_FOG '\0'
#define ELEM_P 'p'

#define ELEMENT_CHARS "xp"
#define NB_ELEMENTS 2

#define FILE_NAME "big_map.txt"
#define W_LEN 800
#define W_HGT 600
#define MINIMAP_LEN 100
#define MINIMAP_HGT 100
#define NB_THREAD 4
#define WALL_TEXTURE "wall.xpm"
#define CEIL_TEXTURE "ceil.xpm"
#define PENGUIN "penguin2.xpm"

typedef struct s_sdl_data
{
	SDL_Window		*w_ptr;
	SDL_Renderer	*renderer;
}				t_sdl_data;

typedef struct s_map_info
{
	size_t				len;
	size_t			hgt;
	char			**map;
}				t_map_info;

typedef	struct	s_mlx
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	char		*img_data;
	void 		*minimap_ptr;
	char		*minimap_data;
	void 		*texture_images[NB_TEXTURE];
	char 		*texture_datas[NB_TEXTURE];
	char 		*texture_names[NB_TEXTURE];
	int			texture_dimensions[NB_TEXTURE][2];

}				t_mlx;

// typedef struct s_point
// {
// 	double x;
// 	double y;
// }				t_point;



typedef struct s_vector
{
	double		x;
	double		y;
}							t_vector;

typedef struct s_collision
{
		char				type;
		t_vector 	col_coord;
		double 		distance;
}							t_collision;

typedef struct s_th_info
{
	int modulo;
	double angle_min;
	double angle_max;
	double offset;
}								t_th_info;

typedef struct s_player
{
	double	x;
	double	y;
	double angle;
	double angle_init;
	double distance_min;
	double len_ray;
	t_vector ray_a;
	t_vector ray_b;
	int				edit;
	t_vector edit_coord;
}							t_player;

typedef struct s_env
{
	t_gc		*gc;
	t_sdl_data	*sdl;
//	t_mlx		*dis;
	t_map_info	*mI;
	t_mlx		*mlx;
	t_player *player;
	t_th_info th_info;
}				t_env;




int			Create_map_tab(char *file_name, t_env *env);
int load_texture(char *base_path, t_env *env);

char		*get_data_ptr(void *img_ptr);
int		key_press(int keycode, t_env *env);
int		mouse_press(int button, int x, int y, t_env *env);
int		loop_wolf(t_env *env);
// int		exit_dis(t_env *env);
int quit_wolf(t_env *env);
t_vector	line_intersection(t_vector a, t_vector b, t_vector c, t_vector d);
t_vector subb_vectors(t_vector v1, t_vector v2);
t_collision parse_map_for_intersec(t_env *env);
void spam_raycast(t_env *env);
void print_vector(t_vector vec, char *name, int option, t_env *env);



#endif
