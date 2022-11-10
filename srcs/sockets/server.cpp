/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 13:41:45 by aliens            #+#    #+#             */
/*   Updated: 2022/10/28 13:41:45 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void	server::init_server(std::vector<size_t> ports)
{
	FD_ZERO(&this->_srv_set);
	for (std::vector<size_t>::iterator it = ports.begin(); it != ports.end(); it++)
	{
		try {

		}
		server.init_srvSocket(*it);
		FD_SET(server._socket, &this->_srv_set);
		this->_servers.push_back(server);
	}

	std::cout << " WEBSERV START " << std::endl;
}
#include <cerrno>
void	server::handle_client()
{
	this->_timeout.tv_sec = 3 * 60;
	this->_timeout.tv_usec = 0;
	int select_socket = this->_servers.back()._socket;

	while (1)
	{
		this->_cli_set = this->_srv_set;
		if (select(select_socket + 1, &this->_cli_set, NULL, NULL, &this->_timeout) == -1)
			throw (server::selectError());

		for (std::vector<srvSocket>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		{
			std::cout << it->_socket << std::endl;
			if (FD_ISSET(it->_socket, &this->_srv_set))
			{
				client	cli;
				if (!cli.init_client(it->_socket))
					throw (server::clientError());
				FD_SET(cli._cli, &this->_cli_set);
				this->_clients.push_back(cli);
				if (select_socket < cli._cli)
					select_socket = cli._cli;
			}
		}
		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			std::cout << it->_cli << std::endl;
			if (FD_ISSET(it->_cli, &this->_cli_set))
			{
				std::cout << "coucou" << std::endl;
				char	buffer[200];
				memset(buffer, 0, 199);
				while (recv(it->_cli, buffer, 199, 0) > 0)
					std::cout << buffer;
			}
		}
	}
}

const char	*server::selectError::what() const throw() { return ("server: error: select"); }
const char	*server::clientError::what() const throw() { return ("server: error: init client"); }
