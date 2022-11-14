/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:06:34 by aliens            #+#    #+#             */
/*   Updated: 2022/11/14 11:06:14 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

srvSocket::srvSocket(size_t port)
{
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	this->_addrlen = sizeof(this->_addr);
	if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw (srvSocket::initError());

	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == -1)
		throw (srvSocket::fcntlError());

	if (bind(this->_socket, (const sockaddr *)&this->_addr, this->_addrlen) == -1)
		throw (srvSocket::bindError());

	if (listen(this->_socket, SOMAXCONN) == -1)
		throw (srvSocket::listenError());
}

srvSocket::~srvSocket() { close(this->_socket); }

srvSocket::srvSocket(const srvSocket &srv)
{
	this->_socket = srv._socket;
	this->_addr.sin_family = srv._addr.sin_family;
	this->_addr.sin_addr.s_addr = srv._addr.sin_addr.s_addr;
	this->_addr.sin_port = srv._addr.sin_port;
	this->_addrlen = srv._addrlen;
}

srvSocket	&srvSocket::operator=(const srvSocket &srv)
{
	this->_socket = srv._socket;
	this->_addr.sin_family = srv._addr.sin_family;
	this->_addr.sin_addr.s_addr = srv._addr.sin_addr.s_addr;
	this->_addr.sin_port = srv._addr.sin_port;
	this->_addrlen = srv._addrlen;
	return (*this);
}

const char	*srvSocket::initError::what() const throw() { return ("socket: error: init"); }
const char	*srvSocket::fcntlError::what() const throw() { return ("socket: error: fcntl"); }
const char	*srvSocket::bindError::what() const throw() { return ("socket: error: bind"); }
const char	*srvSocket::listenError::what() const throw() { return ("socket: error: listen"); }

bool	client::init_client(int srv)
{
	this->_fromlen = sizeof(this->_from);
	if ((this->_cli = accept(srv, (sockaddr *)&this->_from, &this->_fromlen)) == -1)
		return (false);
	
	if (fcntl(this->_cli, F_SETFL, O_NONBLOCK) == -1)
		return (false);

	return (true);
}
