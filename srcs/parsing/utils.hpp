/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:20:04 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/03 15:11:48 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include "server.hpp"

typedef struct	parsing t_parsing;

std::string			file_to_string(std::string file);
std::string	trim(const std::string &txt, std::string to_trim);
t_parsing			get_next_value(std::string &value);

struct parsing
{
	parsing(void);

	std::string	key;
	std::string	value;
	std::string	name;
	bool		i;
};


#endif
