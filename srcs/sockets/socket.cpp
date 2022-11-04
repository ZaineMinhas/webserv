/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:06:34 by aliens            #+#    #+#             */
/*   Updated: 2022/11/03 18:52:19 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

tcpSocket::tcpSocket(bool client, int domain, int port)
{
	if (!client) {
		this->_addr.sin_family = domain;
		this->_addr.sin_addr.s_addr = INADDR_ANY;
		this->_addr.sin_port = htons(port);
		this->_addrlen = sizeof(this->_addr);
	}
	else
		this->_addrlen = sizeof(this->_addr);
	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == -1)
		throw (tcpSocket::fcntlError());
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

void	tcpSocket::acceptSocket(tcpSocket *client)
{
	if ((client->_socket = accept(this->_socket, (sockaddr *)&client->_addr, &client->_addrlen)))
		return ;
	throw (tcpSocket::acceptError());
}



int		tcpSocket::recvSocket(tcpSocket *client, char *buffer, unsigned int len)
{
	int	ret = recv(client->_socket, buffer, len, 0);
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



const char	*tcpSocket::getAddr()
{
	char buffer[INET_ADDRSTRLEN] = {0};
	return (inet_ntop(this->_addr.sin_family, (const void *)&(this->_addr.sin_addr), buffer, INET_ADDRSTRLEN));
}



const char	*tcpSocket::initError::what() const throw() { return ("socket: error: init"); }
const char	*tcpSocket::bindError::what() const throw() { return ("socket: error: bind"); }
const char	*tcpSocket::recvError::what() const throw() { return ("socket: error: recv"); }
const char	*tcpSocket::sendError::what() const throw() { return ("socket: error: send"); }
const char	*tcpSocket::fcntlError::what() const throw() { return ("socket: error: fcntl"); }
const char	*tcpSocket::listenError::what() const throw() { return ("socket: error: listen"); }
const char	*tcpSocket::acceptError::what() const throw() { return ("socket: error: accept"); }
const char	*tcpSocket::connectError::what() const throw() { return ("socket: error: connect"); }





