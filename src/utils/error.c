/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 16:16:36 by root              #+#    #+#             */
/*   Updated: 2025/10/15 16:16:37 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Prints an error message to stderr.
 * 			If `msg` is provided, displays it in red with optional argument.
 * 			If not, prints system errno description.
 *
 * @param msg		Error message (optional).
 * @param arg		Optional argument (e.g., file path).
 * @param exit_code	Value to return for error handling.
 *
 * @return	The same exit code passed as parameter.
 */

int	error_msg(char *msg, char *arg, int exit_code)
{
	if (msg)
	{
		ft_putstr_fd(RED, 2);
		ft_putstr_fd("Error", 2);
		ft_putstr_fd(msg, 2);
		if (arg)
			ft_putstr_fd(arg, 2);
		ft_putstr_fd("\n", 2);
		ft_putstr_fd(RESET, 2);
	}
	else
	{
		ft_putstr_fd(RED, 2);
		ft_putstr_fd("Error: ", 2);
		if (arg)
		{
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": ", 2);
		}
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		ft_putstr_fd(RESET, 2);
	}
	return (exit_code);
}

/**
 * @brief	Prints an error message, cleans up all resources, and exits.
 * 			Useful when memory or MLX instances have already been allocated.
 *
 * @param msg	Error message to display.
 * @param game	Pointer to the main game structure for cleanup.
 */

void	error_cleanup_exit(char *msg, t_game *game)
{
	error_msg(msg, NULL, 0);
	cleanup_game(game);
	exit(1);
}
