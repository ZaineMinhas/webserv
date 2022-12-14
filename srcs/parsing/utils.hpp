/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:20:04 by ctirions          #+#    #+#             */
/*   Updated: 2023/01/10 17:05:13 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include "config.hpp"

typedef struct	parsing t_parsing;

std::string			file_to_string(std::string file);
std::string	trim(const std::string &txt, std::string to_trim);
t_parsing			get_next_value(std::string &value);

struct parsing
{
	parsing(void);

	const t_parsing	&operator=(const t_parsing &rhs);

	std::string	key;
	std::string	value;
	std::string	name;
	bool		i;
	size_t		next_val;
};

#endif
