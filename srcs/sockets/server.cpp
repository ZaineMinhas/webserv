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

static std::string	createResponse(std::vector<std::string> request, config &srv)
{
	std::pair<std::string, size_t>	host;
	size_t	i = 0;
	for (std::vector<std::string>::iterator it = request.begin(); it != request.end(); it++)
	{
		if (*it == "Host: ")
		{
			it++;
			host.first = it->substr(0, it->find(":"));
			host.second = atol(it->substr(it->find(":"), it->size() - it->find(":")).c_str());
			break ;
		}
	}
	for (std::vector<serverBlock>::iterator it = srv.getServers().begin(); it != srv.getServers().end(); it++, i++)
		if (host == it->getListen())
			break ;
	
	return("coucou");
}

static std::vector<std::string>	split(std::string buff)
{
	std::stringstream ss(buff);
	std::istream_iterator<std::string>	begin(ss);
	std::istream_iterator<std::string>	end;
	return (std::vector<std::string>(begin, end));
}

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

void	server::handle_client(config &srv)
{
	std::string	buff;
	int select_socket = this->_servers.back()._socket;

	while (1)
	{
		// this->_timeout.tv_sec = 3 * 60;
		// this->_timeout.tv_usec = 0;

		this->_cli_set = this->_srv_set;
		if (select(select_socket + 1, &this->_cli_set, NULL, NULL, NULL) <= 0)
			throw (server::selectError());

		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			if (FD_ISSET(it->_cli, &this->_cli_set))
			{
				char	buffer[200];
				
				ssize_t	ret = recv(it->_cli, buffer, 199, 0);
				if (ret < 0)
					throw (server::recvError());
				
				buff += std::string(buffer, ret);
				if (buff.find("\r\n\r\n") != std::string::npos)
				{
					std::cout << buff;
					std::vector<std::string>	request = split(buff);
					std::string					respond = createResponse(request, srv);
					buff.clear();
					send(it->_cli, "HTTP/1.1 200 Ok\r\nContent-Lenght: 6\r\n\r\n<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=utf-8/>\n\t\t<title>salut</title>\n\t</head>\n\t<body>\n\t\t<h1>coucou</h1>\n\t</body>\n</html>", 167, 0);
					it->close_client(&this->_srv_set);
					this->_clients.erase(it);
				}
				break;
			}
		}
		for (std::vector<srvSocket>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		{
			if (FD_ISSET(it->_socket, &this->_cli_set))
			{
				client	cli(it->_socket, &this->_srv_set);
				this->_clients.push_back(cli);
				if (select_socket < cli._cli)
					select_socket = cli._cli;
				break;
			}
		}
	}
}

const char	*server::selectError::what() const throw() { return ("server: error: select"); }
const char	*server::recvError::what() const throw() { return ("server: error: recv"); }
