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

static std::map<std::string, std::string>	split(std::string buff)
{
	std::string line;
	std::map<std::string, std::string>	ret;

	ret.insert(std::make_pair("method:", buff.substr(0, buff.find(" "))));
	buff.erase(0, buff.find(" ") + 1);
	ret.insert(std::make_pair("file:", buff.substr(0, buff.find(" "))));
	buff.erase(0, buff.find(" ") + 1);
	ret.insert(std::make_pair("version:", buff.substr(0, buff.find("\n") - 1)));
	buff.erase(0, buff.find("\n") + 1);


	std::stringstream	ss(buff);

	while (getline(ss, line))
	{
		size_t	pos = line.find(":");
		if (pos == std::string::npos)
			break ;
		ret.insert(std::make_pair(line.substr(0, pos + 1), line.substr(pos + 2)));
	}

	return (ret);
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

void	server::handle_client(config &srv)
{
	int select_socket = this->_servers.back()._socket;

	_timeout.tv_sec = 60 * 5;
	_timeout.tv_usec = 0;

	while (1)
	{
		this->_read_set = this->_tmp_set;

		FD_ZERO(&this->_write_set);
		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
			if (it->_ready)
				FD_SET(it->_cli, &this->_write_set);
	
		if (select(select_socket + 1, &this->_read_set, &this->_write_set, NULL, &_timeout) <= 0)
			throw (server::selectError());
 
		for (std::vector<srvSocket>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
		{
			if (FD_ISSET(it->_socket, &this->_read_set))
			{
				client	cli(it->_socket, &this->_tmp_set);
				this->_clients.push_back(cli);
				if (select_socket < cli._cli)
					select_socket = cli._cli;
				break;
			}
		}
		
		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			if (FD_ISSET(it->_cli, &this->_write_set))
			{
				if (!it->_respIsCreate)
				{
					it->_response = responseHttp(it->_body, it->_header, srv.getServers()).createResponse();
					it->_respIsCreate = true;
				}

				std::string	ret = *it->_response.begin();
				int len = ret.size();
				// std::cout << "BEGIN : " << ret << " END" << std::endl;
				it->_ret = send(it->_cli, ret.c_str(), len, 0);
				if (it->_ret < 0)
				{
					it->close_client(&this->_tmp_set);
					this->_clients.erase(it);
					break ;
				}
				
				it->_response.erase(it->_response.begin());
				if (it->_response.empty())
				{
					it->close_client(&this->_tmp_set);
					this->_clients.erase(it);
					std::cout << std::endl << "Is empty" << std::endl << "######################" << std::endl << std::endl;
					break ;
				}
				break ;
			}
		}

		for (std::vector<client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			if (FD_ISSET(it->_cli, &this->_read_set))
			{
				char	buffer[200] = {0};
				
				it->_ret = recv(it->_cli, buffer, 199, 0);
				if (it->_ret <= 0)
				{
					it->close_client(&this->_tmp_set);
					this->_clients.erase(it);
					std::cout << std::endl << "######################" << std::endl << std::endl;
					break ;
				}
				if (!it->_headerEnd)
				{
					it->_head += std::string(buffer, it->_ret);
					if (it->_head.find("\r\n\r\n") != std::string::npos)
					{
						it->_headerEnd = true;
						it->_body = it->_head.substr(it->_head.find("\r\n\r\n") + 4);
						it->_head = it->_head.substr(0, it->_head.find("\r\n\r\n"));
						it->_header = split(it->_head);
						if (it->_header.at("method:") == "POST")
							it->_bodyLength = stringToSize(it->_header.at("Content-Length:"));
						else
							it->_bodyLength = 0;
					}
					else
					 	send(it->_cli, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
				}
				else
					it->_body += std::string(buffer, it->_ret);
				if (it->_body.size() >= it->_bodyLength && it->_headerEnd)
					it->_ready = true;
				else
					send(it->_cli, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
				break;
			}
		}	
	}
}

const char	*server::selectError::what() const throw() { return ("server: error: select"); }
const char	*server::recvError::what() const throw() { return ("server: error: recv"); }
