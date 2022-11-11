/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:05:09 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/11 13:11:05 by ctirions         ###   ########.fr       */
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
				if (serv.key == "location")
				{
					srv._servers.back().add_directory(directory(), serv.name);
					while (!location.key.empty() || location.i)
					{
						location.i = false;
						location = get_next_value(serv.value);
						srv._servers.back().getDirectories().back().set(location.key, location.value);
						if (serv.value.size() > location.next_val)
							serv.value = serv.value.substr(location.next_val);
						else {
							srv._servers.back().getDirectories().back().checkValues();
							break ;
						}
					}
				}
				else
					srv._servers.back().set(serv.key, serv.value);
				if (main.value.size() > serv.next_val)
					main.value = main.value.substr(serv.next_val);
				else {
					srv._servers.back().checkValues();
					break ;
				}
			}
		}
		else
			throw (webserv::badConfFile(std::string("bad block name: ") + main.key));
		try {
			txt = txt.substr(main.next_val);
		}
		catch (std::exception &e) {
			check_double();
			stack_ports();
			break ;
		}
	}
}

void	webserv::add_server(server to_add) { this->_servers.push_back(to_add); }

void	webserv::stack_ports(void)
{
	for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end(); it++)
		_ports.push_back(it->getListen().second);
}

void	webserv::check_double(void)
{
	for (std::vector<server>::iterator it = _servers.begin(); it != _servers.end() - 1; it++)
	{
		std::vector<server>::iterator it2 = it + 1;
		for (; it2 != _servers.end(); it2++) {
			if (it->getListen().second == it2->getListen().second && it->getName() == it2->getName()) {
				it2--;
				_servers.erase(it2 + 1);
			}
		}
	}
}

/*___ exceptions ___*/

const char	*webserv::emptyConfFile::what() const throw() { return ("Empty configuration file."); }
const char	*webserv::badFileName::what() const throw() { return ("Put a correct file name!"); }
const char	*webserv::badInitialization::what() const throw() { return ("Erro occuring when initialize our webserv."); }
