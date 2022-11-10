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
	for (std::vector<size_t>::iterator it = ports.begin(); it != ports.end(); it++)
	{
		srvSocket	server(*it);
		this->_servers.push_back(server);
	}

	std::cout << " WEBSERV START " << std::endl;
}

void	server::handle_client()
{
	FD_ZERO(&this->_srv_set);
	for (std::vector<srvSocket>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		FD_SET(it->_socket, &this->_srv_set);

	this->_timeout = {0};
	int select_socket = this->_servers.at(this->_servers.size() - 1)._socket;

	while (1)
	{
		std::cout << "coucou" << std::endl;
		this->_cli_set = this->_srv_set;
		if (select(select_socket + 1, &this->_cli_set, NULL, NULL, &this->_timeout) < 0)
			throw (server::selectError());


		for (std::vector<srvSocket>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		{
			if (FD_ISSET(it->_socket, &this->_srv_set))
			{
				client	cli;
				cli._fromlen = sizeof(cli._from);
				if ((cli._cli = accept(it->_socket, (sockaddr *)&(cli._from), &(cli._fromlen))) < 0)
					throw (server::acceptError());
				FD_SET(cli._cli, &this->_cli_set);
				this->_clients.push_back(cli);
				if (select_socket < cli._cli)
					select_socket = cli._cli;
			}
		}
		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			if (FD_ISSET(it->_cli, &this->_cli_set))
			{
				char	buffer[200] = {0};
				while (recv(it->_cli, buffer, 199, 0) > 0)
					std::cout << buffer;
				std::cout << std::endl;
			}
		}
	}
}

const char	*server::selectError::what() const throw() { return ("socket: error: select"); }
const char	*server::acceptError::what() const throw() { return ("socket: error: accept"); }
