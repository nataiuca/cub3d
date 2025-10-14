#include "cub3d.h"

static void	free_sprite_frames(t_game *game, t_sprite *sprite)
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

static void	free_single_sprite(t_game *game, t_sprite *sprite)
{
	if (!sprite)
		return ;
	free_sprite_frames(game, sprite);
	free(sprite);
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
			free_single_sprite(game, game->sprite[i]);
			game->sprite[i] = NULL;
		}
		i++;
	}
	free(game->sprite);
	game->sprite = NULL;
}
