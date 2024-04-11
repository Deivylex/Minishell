/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esalmela <esalmela@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:56:05 by esalmela          #+#    #+#             */
/*   Updated: 2024/04/11 13:56:51 by esalmela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_split
{
	char	**str;
	size_t	start;
	size_t	end;
	size_t	len;
	size_t	i;
}	t_split;

#endif
