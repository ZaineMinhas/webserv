/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:06:34 by aliens            #+#    #+#             */
/*   Updated: 2022/11/02 10:13:46 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

tcpSocket::tcpSocket(int domain, int port) {
	this->_addr.sin_family = domain;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	this->_addrlen = sizeof(this->_addr);
}

tcpSocket::~tcpSocket() {}



bool	tcpSocket::initSocket(int domain, int type, int protocol)
{
	if ((this->_socket = socket(domain, type, protocol)))
		return (true);
	return (false);
}

void	tcpSocket::closeSocket() { close(this->_socket); }



bool	tcpSocket::bindSocket(int domain, int port)
{
	if (!bind(this->_socket, (const sockaddr *)&this->_addr, this->_addrlen) < 0)
		return (true);
	throw (tcpSocket::errorSocket().sendError());
}

bool	tcpSocket::listenSocket()
{
	if (!listen(this->_socket, SOMAXCONN))
		return (true);
	throw (tcpSocket::errorSocket().listenError());
}



bool	tcpSocket::connectSocket(int domain, const std::string &addr, int port)
{
	sockaddr_in	srv;
	inet_pton(domain, addr.c_str(), &srv.sin_addr.s_addr);
	srv.sin_family = domain;
	srv.sin_port = htons(port);
	
	return (!(connect(this->_socket, (const sockaddr *)&srv, sizeof(srv))));
}

bool	tcpSocket::acceptSocket(client *client)
{
	if (!(client->_cli._socket = accept(this->_socket, (sockaddr *)&this->_addr, &this->_addrlen)))
		return (true);
	throw (tcpSocket::errorSocket().acceptError());
}



int		tcpSocket::recvSocket(char *buffer, unsigned int len)
{
	int	ret = recv(this->_socket, buffer, len, 0);
	if (ret == -1)
		throw (tcpSocket::errorSocket().recvError());
	return (ret);
}

int		tcpSocket::sendSocket(const char *data, unsigned int len)
{
	int ret = send(this->_socket, data, len, 0);
	if (ret == -1)
		throw(tcpSocket::errorSocket().sendError());
	return (ret);
}



const char	*tcpSocket::errorSocket::bindError() const throw() { return ("socket: error: bind"); }
const char	*tcpSocket::errorSocket::recvError() const throw() { return ("socket: error: recv"); }
const char	*tcpSocket::errorSocket::sendError() const throw() { return ("socket: error: send"); }
const char	*tcpSocket::errorSocket::listenError() const throw() { return ("socket: error: listen"); }
const char	*tcpSocket::errorSocket::acceptError() const throw() { return ("socket: error: accept"); }
