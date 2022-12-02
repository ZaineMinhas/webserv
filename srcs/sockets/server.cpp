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
#include "responseHttp.hpp"

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
		it->close_srvSocket(&this->_srv_set);
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
		std::cout << "select_socket : " << select_socket << std::endl;
		if (select(select_socket + 1, &this->_cli_set, NULL, NULL, NULL) <= 0)
			throw (server::selectError());

		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
			std::cout << it->_cli << std::endl;

		std::cout <<  "######################" << std::endl;

		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			if (FD_ISSET(it->_cli, &this->_cli_set))
			{
				char	buffer[200];
				
				ssize_t	ret = recv(it->_cli, buffer, 199, 0);
				if (ret < 0)
					continue ;
				
				buff += std::string(buffer, ret);

				std::cout << "HERE!!!!!!!!!!!!!!!  : " << it->_cli << std::endl;	
				if (buff.find("\r\n\r\n") != std::string::npos)
				{
					std::cout << buff << std::endl << " client : " << it->_cli << std::endl;
					if (it->_response.empty())
					{
						std::vector<std::string>	request = split(buff);
						responseHttp	response(request, srv.getServers());
						it->_response = response.createResponse();
					}
					// for (std::vector<std::string>::iterator iter = it->_response.begin(); iter != it->_response.end(); iter++)
					// 	std::cout << iter->c_str() << "\n----------------------" <<std::endl;
					buff.clear();

					int ret = send(it->_cli, it->_response[0].c_str(), it->_response[0].size(), 0);
					std::cout << ret << std::endl;
					it->_response.erase(it->_response.begin());
					if (it->_response.empty())
					{
						std::cout << "SALUT\n ";
						it->close_client(&this->_srv_set);
						this->_clients.erase(it);
					}
					std::cout << "------------------\n\n";
				}
				else
					send(it->_cli, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
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
