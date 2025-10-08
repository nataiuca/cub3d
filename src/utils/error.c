
#include "../../inc/cub3d.h"

int	error_msg(char *msg, char *arg, int exit_code)
{
	if (msg)
	{
		ft_putstr_fd("Error", 2);
		ft_putstr_fd(msg, 2);
		if (arg)
			ft_putstr_fd(arg, 2);
		ft_putstr_fd("\n", 2);
	}
	else
	{
		ft_putstr_fd("Error: ", 2);
		if (arg)
		{
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": ", 2);
		}
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	return(exit_code);
}

/*
** Función para mostrar error y salir del programa
** - Muestra "Error\n" seguido del mensaje personalizado
** - Sale del programa con código de error 1
*/
void	error_exit(char *msg)
{
	error_msg(msg, NULL, 0);
	exit(EXIT_FAILURE);
}

/*
** Función para mostrar error, limpiar memoria y salir
** - Igual que error_exit pero limpia la memoria antes de salir
** - Útil cuando ya hemos reservado memoria
*/
void	error_cleanup_exit(char *msg, t_game *game)
{
	error_msg(msg, NULL, 0);
	cleanup_game(game);
	exit(1);
}
