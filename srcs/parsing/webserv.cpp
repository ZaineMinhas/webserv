/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:05:09 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/03 16:20:25 by ctirions         ###   ########.fr       */
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

void	webserv::check_conf_file(std::string file, webserv &srv)
{
	std::string		txt = file_to_string(file);
	if (txt.empty() || trim(txt, "\n ").empty())
		throw (webserv::emptyConfFile());

	t_parsing	main;
	t_parsing	server;
	t_parsing	location;

	while (!main.key.empty() || main.i)
	{
		main.i = false;
		main = get_next_value(txt);
		if (main.key == "server")
		{
			while (!server.key.empty() || server.i)
			{
				server.i = false;
				server = get_next_value(main.value);
				std::cout << "KEY : " << server.key << std::endl;
				std::cout << "VAL : " << server.value << std::endl;
				if (server.key == "location")
				{
					while (!location.key.empty() || location.i)
					{
						location.i = false;
						location = get_next_value(server.value);
						srv._servers.back().getDirectories().back().set(location.key, location.value);
					}
				}
				else
					srv._servers.back().set(server.key, server.value);
			}
		}
	}

	std::cout << txt << std::endl;
}

/*___ exceptions ___*/

const char	*webserv::badConfFile::what() const throw() { return ("Bad configuration file."); }

const char	*webserv::emptyConfFile::what() const throw() { return ("Empty configuration file."); }

const char	*webserv::badFileName::what() const throw() { return ("Put a correct file name!"); }

const char	*webserv::badInitialization::what() const throw() { return ("Erro occuring when initialize our webserv."); }
