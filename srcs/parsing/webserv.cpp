/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:05:09 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/04 14:31:42 by ctirions         ###   ########.fr       */
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
	t_parsing	serv;
	t_parsing	location;

	while (!main.key.empty() || main.i)
	{
		main.i = false;
		main = get_next_value(txt);
		if (main.key == "server")
		{
			srv.add_server(server());
			while (!serv.key.empty() || serv.i)
			{
				serv.i = false;
				serv = get_next_value(main.value);
				std::cout << "KEY : " << serv.key << std::endl;
				std::cout << "VAL : " << serv.value << std::endl;
				if (serv.key == "location")
				{
					srv._servers.back().add_directory(directory());
					while (!location.key.empty() || location.i)
					{
						location.i = false;
						location = get_next_value(serv.value);
						std::cout << "KEY : " << location.key << std::endl;
						std::cout << "VAL : " << location.value << std::endl;
						srv._servers.back().getDirectories().back().set(location.key, location.value);
						if (serv.value.size() > location.next_val)
							serv.value = serv.value.substr(location.next_val);
						else
							break;
					}
				}
				else
					srv._servers.back().set(serv.key, serv.value);
				if (main.value.size() > serv.next_val)
					main.value = main.value.substr(serv.next_val);
				else
					break ;
			}
		}
		if (txt.size() > main.value.size())
			txt = txt.substr(main.next_val);
		else
			break;
	}

	std::cout << txt << std::endl;
}

void	webserv::add_server(server to_add) { _servers.push_back(to_add); }

/*___ exceptions ___*/

const char	*webserv::badConfFile::what() const throw() { return ("Bad configuration file."); }

const char	*webserv::emptyConfFile::what() const throw() { return ("Empty configuration file."); }

const char	*webserv::badFileName::what() const throw() { return ("Put a correct file name!"); }

const char	*webserv::badInitialization::what() const throw() { return ("Erro occuring when initialize our webserv."); }
