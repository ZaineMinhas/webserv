/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:19:12 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/10 13:59:03 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "webserv.hpp"

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
		throw (webserv::badFileName());
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
		// std::cout << "key : " << ret.key << std::endl;
		if (line.find_first_of(' ') != std::string::npos)
			ret.value = trim(line.substr(line.find_first_of(' ')), WHITESPACE);
		// std::cout << "value : " << ret.value << std::endl;
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