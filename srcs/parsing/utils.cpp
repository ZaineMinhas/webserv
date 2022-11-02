/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:19:12 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/02 13:51:00 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "webserv.hpp"

std::string	file_to_string(std::string file)
{
	std::string		txt;
	std::ifstream	ftxt(file);
	if (ftxt) {
		std::ostringstream	ss;
		ss << ftxt.rdbuf();
		txt = ss.str();
	}
	else
		throw (webserv::badFileName());
	return (txt);
}

std::string	trim(std::string &txt, std::string to_trim)
{
	size_t	begin = txt.find_first_not_of(to_trim);
	if (begin == std::string::npos)
		throw (webserv::emptyConfFile());
	size_t	end = txt.find_last_not_of(to_trim) + 1;
	txt = txt.substr(begin, end - begin);
	return (txt);
}
