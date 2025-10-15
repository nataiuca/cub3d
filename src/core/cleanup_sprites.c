#include "cub3d.h"

void	free_sprite_frames(t_game *game, t_sprite *sprite)
{
	int	j;

	if (!sprite || !sprite->frames)
		return ;
	j = 0;
	while (j < sprite->frame_count)
	{
		if (sprite->frames[j])
		{
			mlx_delete_image(game->mlx, sprite->frames[j]);
			sprite->frames[j] = NULL;
		}
		j++;
	}
	free(sprite->frames);
	sprite->frames = NULL;
}

void	free_sprite(t_game *game)
{
	int	i;

	if (!game || !game->sprite)
		return ;
	i = 0;
	while (i < game->sprite_count)
	{
		if (game->sprite[i])
		{
			free_sprite_frames(game, game->sprite[i]);
			free(game->sprite[i]);
			game->sprite[i] = NULL;
		}
		i++;
	}
	free(game->sprite);
	game->sprite = NULL;
}
