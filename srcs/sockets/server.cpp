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
	FD_ZERO(&this->_tmp_set);
	for (std::vector<size_t>::iterator it = ports.begin(); it != ports.end(); it++)
	{
		srvSocket	srv(*it);
		FD_SET(srv._socket, &this->_tmp_set);
		this->_servers.push_back(srv);
	}

	std::cout << " WEBSERV START " << std::endl;
}

server::~server()
{
	for (std::vector<srvSocket>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		it->close_srvSocket(&this->_tmp_set);
}

server::server(const server& srv)
{
	this->_servers = srv._servers;
	this->_clients = srv._clients;
	this->_read_set = srv._read_set;
	this->_write_set = srv._write_set;
	this->_tmp_set = srv._tmp_set;
	this->_timeout = srv._timeout;
}

server	&server::operator=(const server &srv)
{
	this->_servers = srv._servers;
	this->_clients = srv._clients;
	this->_read_set = srv._read_set;
	this->_write_set = srv._write_set;
	this->_tmp_set = srv._tmp_set;
	this->_timeout = srv._timeout;
	return (*this);
}

#include <cerrno>

void	server::handle_client(config &srv)
{
	std::string	buff;
	int select_socket = this->_servers.back()._socket;

	while (1)
	{
		this->_read_set = this->_tmp_set;

		FD_ZERO(&this->_write_set);
		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
			if (!it->_response.empty())
				FD_SET(it->_cli, &this->_write_set);

		if (select(select_socket + 1, &this->_read_set, &this->_write_set, NULL, NULL) <= 0)
			throw (server::selectError());
 
		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			std::cout << "maybe write client : " << it->_cli << std::endl;
			if (FD_ISSET(it->_cli, &this->_write_set))
			{
				std::cout << "write client : " << it->_cli << std::endl; 
				std::cout << "Line : " << it->_response[0].c_str() << " END" << std::endl;
				it->_ret = send(it->_cli, it->_response[0].c_str(), it->_response[0].size(), 0);
				std::cout << "COUCOU" << std::endl;
				it->_response.erase(it->_response.begin());
				if (it->_ret < 0)
				{
					std::cout << "ICI : " << strerror(errno) << std::endl;
					it->close_client(&this->_tmp_set);
					this->_clients.erase(it);
					break ;
				}
				if (it->_response.empty())
				{
					it->close_client(&this->_tmp_set);
					this->_clients.erase(it);
					std::cout << std::endl << "######################" << std::endl;
				}
				break ;
			}
		}

		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			std::cout << "maybe read client : " << it->_cli << std::endl; 
			if (FD_ISSET(it->_cli, &this->_read_set))
			{
				std::cout << "read client : " << it->_cli << std::endl; 
				char	buffer[200];
				
				ssize_t	ret = recv(it->_cli, buffer, 199, 0);
				if (ret < 0)
				{
					it->close_client(&this->_tmp_set);
					this->_clients.erase(it);
					break ;
				}

				buff += std::string(buffer, ret);

				if (buff.find("\r\n\r\n") != std::string::npos)
				{
					std::cout << buff << std::endl;
					if (it->_response.empty())
					{
						std::vector<std::string>	request = split(buff);
						responseHttp	response(request, srv.getServers());
						it->_response = response.createResponse();
						// for (std::vector<std::string>::iterator iter = it->_response.begin(); iter != it->_response.end(); iter++)
						// 	std::cout << "LINE : " << iter->c_str() << " END" << std::endl;
					}
					buff.clear();
				}
				else
					send(it->_cli, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
				break;
			}
		}
	
		for (std::vector<srvSocket>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		{
			if (FD_ISSET(it->_socket, &this->_read_set))
			{
				client	cli(it->_socket, &this->_tmp_set);
				std::cout << "connexion de : " << cli._cli << std::endl; 
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
