/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-mada <bde-mada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:56:04 by bde-mada          #+#    #+#             */
/*   Updated: 2025/06/18 18:56:09 by bde-mada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	is_valid_float(char *str)
{
    int	i;
    int	has_dot;

    if (!str || !*str)
        return (0);
    i = 0;
    has_dot = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (str[i] == '.' && !has_dot)
            has_dot = 1;
        else if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (i > (str[0] == '-' || str[0] == '+' ? 1 : 0));
}