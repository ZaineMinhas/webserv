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

class responseHttp;

// static std::string	createResponse(std::vector<std::string> request, config &srv)
// {

// 	std::string						response;
// 	std::vector<serverBlock>		serversTmp = srv.getServers();
// 	std::pair<std::string, size_t>	host;
// 	std::string						htmlFileName;
// 	size_t							i_s = 0;
// 	size_t							i_d = 0;

// 	/* Get index of the server */

// 	for (std::vector<std::string>::iterator it = request.begin(); it != request.end(); it++)
// 	{
// 		if (*it == "Host:")
// 		{
// 			it++;
// 			host.first = it->substr(0, it->find(":"));
// 			std::stringstream	ss(it->substr(it->find(":") + 1, it->size() - it->find(":") + 1));
// 			ss >> host.second;
// 			break ;
// 		}
// 	}
// 	for (std::vector<serverBlock>::iterator it = serversTmp.begin(); it != serversTmp.end(); it++, i_s++)
// 		if (host == it->getListen())
// 			break ;

// 	/* Find location index */

// 	std::vector<directory>	directoriesTmp = serversTmp[i_s].getDirectories();

// 	for (std::vector<directory>::iterator it = directoriesTmp.begin(); it != directoriesTmp.end(); it++, i_d++)
// 		if (it->getName() == request[1])
// 			break ;

// 	/* Add response header */

// 	if (i_d == directoriesTmp.size())
// 	{
// 		htmlFileName = serversTmp[i_s].getRoot() + request[1];
// 		std::cout << htmlFileName << std::endl;
// 	}
// 	else
// 	{
// 		if (!directoriesTmp[i_d].getRoot().empty())
// 			htmlFileName += directoriesTmp[i_d].getRoot();
// 		else
// 			htmlFileName += serversTmp[i_s].getRoot();
// 		if (!directoriesTmp[i_d].getIndex().empty())
// 			htmlFileName += "/" + directoriesTmp[i_d].getIndex();
// 	}

// 	std::cout << "File name : " << htmlFileName << std::endl;

// 	response += request[2];
// 	response += " 200 Ok";
// 	response += ""; // size_t to string
// 	response += "\r\n\r\n";

// 	/* Add html content */
	
// 	std::string		htmlTxt;
// 	std::ifstream	ftxt(htmlFileName.c_str());
// 	if (ftxt) {
// 		std::ostringstream	ss;
// 		ss << ftxt.rdbuf();
// 		htmlTxt = ss.str();
// 	}
// 	else
// 		; // No page to return --> ERROR
// 	response += htmlTxt;

// 	return(response);
// }

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
					// std::string					responses = createResponse(request, srv);
					responseHttp	response(request, srv.getServers());
					response.createResponse();
					buff.clear();
					send(it->_cli, response.toSend(), response.size(), 0);
					it->close_client(&this->_srv_set);
					this->_clients.erase(it);
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
