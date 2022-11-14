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

server::server(std::vector<size_t> ports)
{
	FD_ZERO(&this->_srv_set);
	for (std::vector<size_t>::iterator it = ports.begin(); it != ports.end(); it++)
	{
		srvSocket	srv(*it);
		FD_SET(srv._socket, &this->_srv_set);
		this->_servers.push_back(srv);
	}

	std::cout << " WEBSERV START " << std::endl;
}

server::~server()
{
	for (std::vector<srvSocket>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		FD_CLR(it->_socket, &this->_srv_set);
}

server::server(const server& srv)
{
	this->_servers = srv._servers;
	this->_clients = srv._clients;
	this->_cli_set = srv._cli_set;
	this->_srv_set = srv._srv_set;
	this->_timeout = srv._timeout;
}

server	&server::operator=(const server &srv)
{
	this->_servers = srv._servers;
	this->_clients = srv._clients;
	this->_cli_set = srv._cli_set;
	this->_srv_set = srv._srv_set;
	this->_timeout = srv._timeout;
	return (*this);
}

void	server::handle_client()
{
	int select_socket = this->_servers.back()._socket;

	while (1)
	{
		this->_timeout.tv_sec = 3 * 60;
		this->_timeout.tv_usec = 0;

		this->_cli_set = this->_srv_set;
		if (select(select_socket + 1, &this->_cli_set, NULL, NULL, &this->_timeout) <= 0)
			throw (server::selectError());

		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			if (FD_ISSET(it->_cli, &this->_cli_set))
			{
				char	buffer[200];
				while (recv(it->_cli, buffer, 199, 0) > 0)
					std::cout << buffer;
				std::cout << std::endl;
				it->close_client(&this->_srv_set);
				this->_clients.erase(it);
				break ;
			}
		}
		for (std::vector<srvSocket>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		{
			if (FD_ISSET(it->_socket, &this->_srv_set))
			{
				std::cout << it->_socket << std::endl;
				client	cli(it->_socket);
				FD_SET(cli._cli, &this->_srv_set);
				this->_clients.push_back(cli);
				if (select_socket < cli._cli)
					select_socket = cli._cli;
			}
		}
	}
}

const char	*server::selectError::what() const throw() { return ("server: error: select"); }
