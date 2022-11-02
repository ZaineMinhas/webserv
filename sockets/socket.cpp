/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:06:34 by aliens            #+#    #+#             */
/*   Updated: 2022/11/02 13:55:43 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

tcpSocket::tcpSocket(bool client, int domain, int port)
{
	if (!client)
		this->_addr.sin_family = domain;
		this->_addr.sin_addr.s_addr = INADDR_ANY;
		this->_addr.sin_port = htons(port);
		this->_addrlen = sizeof(this->_addr);
}

tcpSocket::~tcpSocket() {}



void	tcpSocket::initSocket(int domain, int type, int protocol)
{
	if ((this->_socket = socket(domain, type, protocol)))
		return ;
	throw (tcpSocket::initError());
}

void	tcpSocket::closeSocket() { close(this->_socket); }



void	tcpSocket::bindSocket(int domain, int port)
{
	if (!bind(this->_socket, (const sockaddr *)&this->_addr, this->_addrlen))
		return ;
	throw (tcpSocket::bindError());
}

void	tcpSocket::listenSocket()
{
	if (!listen(this->_socket, SOMAXCONN))
		return ;
	throw (tcpSocket::listenError());
}



void	tcpSocket::connectSocket(int domain, const std::string &addr, int port)
{
	sockaddr_in	srv;
	inet_pton(domain, addr.c_str(), &srv.sin_addr.s_addr);
	srv.sin_family = domain;
	srv.sin_port = htons(port);
	
	if (!(connect(this->_socket, (const sockaddr *)&srv, sizeof(srv))))
		return ;
	throw (tcpSocket::connectError());
}

void	tcpSocket::acceptSocket(client *client)
{
	if ((client->_cli._socket = accept(this->_socket, (sockaddr *)&client->_cli._addr, &client->_cli._addrlen)))
		return ;
	throw (tcpSocket::acceptError());
}



int		tcpSocket::recvSocket(client *client, char *buffer, unsigned int len)
{
	int	ret = recv(client->_cli._socket, buffer, len, 0);
	if (ret == -1)
		throw (tcpSocket::recvError());
	return (ret);
}

int		tcpSocket::sendSocket(const char *data, unsigned int len)
{
	int ret = send(this->_socket, data, len, 0);
	if (ret == -1)
		throw(tcpSocket::sendError());
	return (ret);
}



const char	*tcpSocket::initError::what() const throw() { return ("socket: error: init"); }
const char	*tcpSocket::bindError::what() const throw() { return ("socket: error: bind"); }
const char	*tcpSocket::recvError::what() const throw() { return ("socket: error: recv"); }
const char	*tcpSocket::sendError::what() const throw() { return ("socket: error: send"); }
const char	*tcpSocket::listenError::what() const throw() { return ("socket: error: listen"); }
const char	*tcpSocket::acceptError::what() const throw() { return ("socket: error: accept"); }
const char	*tcpSocket::connectError::what() const throw() { return ("socket: error: connect"); }





const char	*client::getAddr()
{
	char buffer[INET_ADDRSTRLEN] = {0};
	return (inet_ntop(this->_cli._addr.sin_family, (const void *)&(this->_cli._addr.sin_addr), buffer, INET_ADDRSTRLEN));
}
