/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:06:34 by aliens            #+#    #+#             */
/*   Updated: 2022/12/12 15:11:05 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

srvSocket::srvSocket(size_t port)
{
	const int opt = 1;
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	this->_addrlen = sizeof(this->_addr);
	if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw (srvSocket::initError());

	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == -1)
		throw (srvSocket::fcntlError());

	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw (srvSocket::optError());

	if (bind(this->_socket, (const sockaddr *)&this->_addr, this->_addrlen) == -1)
		throw (srvSocket::bindError());

	if (listen(this->_socket, SOMAXCONN) == -1)
		throw (srvSocket::listenError());
}

void	srvSocket::close_srvSocket(fd_set *set)
{
	close(this->_socket);
	FD_CLR(this->_socket, set);
}

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

const char	*srvSocket::initError::what() const throw() { return ("server socket: error: init"); }
const char	*srvSocket::fcntlError::what() const throw() { return ("server socket: error: fcntl"); }
const char	*srvSocket::optError::what() const throw() { return ("server socket: error: setsockopt"); }
const char	*srvSocket::bindError::what() const throw() { return ("server socket: error: bind"); }
const char	*srvSocket::listenError::what() const throw() { return ("server socket: error: listen"); }

client::client(int srv, fd_set *set)
{
	this->_ret = 0;
	
	this->_fromlen = sizeof(this->_from);
	if ((this->_cli = accept(srv, (sockaddr *)&this->_from, &this->_fromlen)) == -1)
		throw (client::initError());
	
	if (fcntl(this->_cli, F_SETFL, O_NONBLOCK) == -1)
		throw (client::fcntlError());

	FD_SET(this->_cli, set);
}

client::client(const client &cli)
{
	this->_cli = cli._cli;
	this->_from.sin_family = cli._from.sin_family;
	this->_from.sin_addr.s_addr = cli._from.sin_addr.s_addr;
	this->_from.sin_port = cli._from.sin_port;
	this->_fromlen = cli._fromlen;
}

client	&client::operator=(const client &cli)
{
	this->_cli = cli._cli;
	this->_from.sin_family = cli._from.sin_family;
	this->_from.sin_addr.s_addr = cli._from.sin_addr.s_addr;
	this->_from.sin_port = cli._from.sin_port;
	this->_fromlen = cli._fromlen;
	return (*this);
}

void	client::close_client(fd_set *set)
{
	close(this->_cli);
	if (set)
		FD_CLR(this->_cli, set);
}

const char	*client::initError::what() const throw() { return ("client socket: error: init"); }
const char	*client::fcntlError::what() const throw() { return ("client socket: error: fcntl"); }
const char	*client::optError::what() const throw() { return ("client socket: error: setsockopt"); }
