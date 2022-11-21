/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:05:09 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/21 18:25:44 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

////////////////////
///   WEBSERV    ///
////////////////////

/*___ canonical form ___*/

config::config(void) {}

config::config(const config &src) { *this = src; }

config::~config(void) {}

config	&config::operator=(const config &src) {
	_servers = src._servers;
	_ports = src._ports;
	return (*this);
}

/*___ getters ___*/

std::vector<serverBlock>	&config::getServers(void) { return (_servers); }
std::vector<size_t>			config::getPorts(void) const { return (_ports); }


/*___ utils ___*/

void	config::check_conf_file(std::string file, config &srv)
{
	std::string		txt = file_to_string(file);
	if (txt.empty() || trim(txt, "\n ").empty())
		throw (config::emptyConfFile());

	t_parsing	main;
	t_parsing	serv;
	t_parsing	location;

	while (!main.key.empty() || main.i)
	{
		main.i = false;
		main = get_next_value(txt);
		if (main.key == "server")
		{
			srv.add_server(serverBlock());
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
			throw (config::badConfFile(std::string("bad block name: ") + main.key));
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

void	config::add_server(serverBlock to_add) { this->_servers.push_back(to_add); }

void	config::stack_ports(void)
{
	for (std::vector<serverBlock>::iterator it = _servers.begin(); it != _servers.end(); it++)
		_ports.push_back(it->getListen().second);
}

void	config::check_double(void)
{
	for (std::vector<serverBlock>::iterator it = _servers.begin(); it != _servers.end() - 1; it++)
	{
		std::vector<serverBlock>::iterator it2 = it + 1;
		for (; it2 != _servers.end(); it2++) {
			if (it->getListen().second == it2->getListen().second && it->getName() == it2->getName()) {
				it2--;
				_servers.erase(it2 + 1);
			}
		}
	}
}

/*___ exceptions ___*/

const char	*config::emptyConfFile::what() const throw() { return ("Empty configuration file."); }
const char	*config::badFileName::what() const throw() { return ("Put a correct file name!"); }
const char	*config::badInitialization::what() const throw() { return ("Erro occuring when initialize our webserv."); }
