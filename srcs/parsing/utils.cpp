/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:19:12 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/22 15:19:24 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

const std::string WHITESPACE = " \n\r\t\f\v";

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
		throw (config::badFileName());
	return (txt);
}

std::string	trim(const std::string &txt, std::string to_trim)
{
	size_t				begin = txt.find_first_not_of(to_trim);
	std::string	ret;
	if (begin == std::string::npos)
		return ("");
	size_t	end = txt.find_last_not_of(to_trim) + 1;
	ret = txt.substr(begin, end - begin);
	return (ret);
}

t_parsing	get_next_value(std::string &value)
{
	t_parsing			ret;
	std::istringstream	f(value);
	std::string			line;

	while (trim(line, WHITESPACE).empty() && ++ret.next_val)
		std::getline(f, line);

	ret.next_val += line.size() + 1;
	if (line.find_first_of('{') == std::string::npos)
	{
		ret.key = trim(line.substr(0, line.find_first_of(' ')), WHITESPACE);
		if (line.find_first_of(' ') != std::string::npos)
			ret.value = trim(line.substr(line.find_first_of(' ')), WHITESPACE);
	}
	else
	{
		int	bracket = 1;
		ret.key = trim(line.substr(0, line.find_first_of(' ')), WHITESPACE);
		ret.name = trim(line.substr(line.find_first_of(' '), line.find_first_of('{') - line.find_first_of(' ')), WHITESPACE);
		while (std::getline(f, line))
		{
			ret.next_val += line.size() + 1;
			if (line.find_first_of('{') != std::string::npos)
				bracket++;
			if (line.find_first_of('}') != std::string::npos)
				bracket--;
			if (!bracket)
				break ;
			ret.value += line + "\n";
		}
	}
	return (ret);
}

parsing::parsing(void) { i = true; next_val = 0; }

const t_parsing	&t_parsing::operator=(const t_parsing &rhs)
{
	key = rhs.key;
	value = rhs.value;
	name = rhs.name;
	i = rhs.i;
	next_val = rhs.next_val;
	return (*this);
}
