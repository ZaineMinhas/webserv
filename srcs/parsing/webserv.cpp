/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:05:09 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/02 14:42:32 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

////////////////////
///   WEBSERV    ///
////////////////////

/*___ canonical form ___*/

webserv::webserv(void) {}

webserv::webserv(const webserv &src) {}

webserv::~webserv(void) {}

webserv	&webserv::operator=(const webserv &src) {
	return (*this);
}

/*___ utils ___*/

void	webserv::check_conf_file(std::string file)
{
	std::string		txt = file_to_string(file);
	if (txt.empty() || trim(txt, "\n ").empty())
		throw (webserv::emptyConfFile());

	std::cout << txt << std::endl;
}

/*___ exceptions ___*/

const char	*webserv::badConfFile::what() const throw() {
	return ("Bad configuration file.");
}

const char	*webserv::emptyConfFile::what() const throw() {
	return ("Empty configuration file.");
}

const char	*webserv::badFileName::what() const throw() {
	return ("Put a correct file name!");
}

const char	*webserv::badInitialization::what() const throw() {
	return ("Erro occuring when initialize our webserv.");
}
