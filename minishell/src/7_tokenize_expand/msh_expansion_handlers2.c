/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_expansion_handlers2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdi-leo <tcorax42@gmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 11:22:59 by tdi-leo           #+#    #+#             */
/*   Updated: 2022/12/29 16:42:32 by tdi-leo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	handle_exit_status(t_splitter *v)
{
	if (join_exit_status_to_buffer(v))
		return (error_handler(1, ft_strdup("ESPANSIONE EXIT STATUS FALLITA\n"),
				DO_NOT_RESET));
	return (EXIT_SUCCESS);
}

/**
 * @brief This loop handles buffer addition for literal case_handlers. If mode 1
 * is passed to parameter, the loop expands $variables on it's way.
 * 
 * @param mode 
 * @param v 
 * @param utility_c 
 * @return int 
 */
static int	_literal_buffer_loop(int mode, t_splitter *v, size_t utility_c)
{
	while (v->cursor < utility_c)
	{
		if (mode)
		{
			if (is_var(v->input[v->cursor])
				&& !is_var_terminator(v->input[v->cursor + 1]))
			{
				if (handle_var(v))
					return (error_handler(g_exit_status,
							ft_strdup("!!! _literal_buffer_loop"),
							DO_NOT_RESET));
			}
			if (v->cursor >= utility_c)
				break ;
		}
		ft_printd(0, "__JOINING CHAR {%c} TO BUFFER\n", v->input[v->cursor]);
		jointo_buffer(v, 0);
		v->cursor += 1;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Handler for double_quote_literal expansion. It double checks if the 
 * quote delimiter is correctly terminated, and then join uncoditionally every 
 * char to bufferline expanding $variables on it's way.
 * 
 * @param v 
 * @return int 
 */
int	dquote_section_handler(t_splitter *v)
{
	size_t	utility_c;

	ft_printd(0, "__ENT DQUOTE SECTION. cursor: {%d}:{%c} bufferline: {%s}\n",
		v->cursor, v->input[v->cursor], v->bufferline);
	while (v->input[v->cursor] == '\"')
	{
		v->handling_dquote = 1;
		utility_c = v->cursor;
		if (move_cursor_next_occurrence(&utility_c, v->input, '\"'))
			return (error_handler(g_exit_status,
					ft_strdup("!!! dquote_section 1"), DO_NOT_RESET));
		v->cursor += 1;
		if (v->cursor == utility_c)
		{
			jointo_buffer(v, 1);
			v->cursor += 1;
			return (EXIT_SUCCESS);
		}
		if (_literal_buffer_loop(1, v, utility_c))
			return (error_handler(g_exit_status,
					ft_strdup("!!! dquote_section 2"), DO_NOT_RESET));
		v->cursor += 1;
		v->handling_dquote = 0;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Handler for single_quote_literal expansion. It double checks if the
 * quote delimiter is correctly terminated, and then join uncoditionally every
 * char to bufferline.
 * 
 * @param v 
 * @return int 
 */
int	squote_section_handler(t_splitter *v)
{
	size_t	utility_c;

	while (v->input[v->cursor] == '\'')
	{
		utility_c = v->cursor;
		if (move_cursor_next_occurrence(&utility_c, v->input, '\''))
			return (error_handler(g_exit_status, ft_strdup("squote_section 1"),
					DO_NOT_RESET));
		v->cursor += 1;
		if (v->cursor == utility_c)
		{
			jointo_buffer(v, 1);
			v->cursor += 1;
			return (EXIT_SUCCESS);
		}
		if (_literal_buffer_loop(0, v, utility_c))
			return (error_handler(g_exit_status,
					ft_strdup("squote_section 2"), DO_NOT_RESET));
		v->cursor += 1;
	}
	ft_printd(0, "__EXIT SQUOTE SECTION. cursor: {%d}:{%c} bufferline: {%s}\n",
		v->cursor, v->input[v->cursor], v->bufferline);
	return (EXIT_SUCCESS);
}
