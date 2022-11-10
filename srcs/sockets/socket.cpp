/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:06:34 by aliens            #+#    #+#             */
/*   Updated: 2022/11/09 15:00:19 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

srvSocket::srvSocket(int port)
{
	this->_addr.sin_family = PF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	this->_addrlen = sizeof(this->_addr);
	if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)))
		return ;
	throw (srvSocket::initError());

	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == -1)
		throw (srvSocket::fcntlError());

	if (!bind(this->_socket, (const sockaddr *)&this->_addr, this->_addrlen))
		return ;
	throw (srvSocket::bindError());

	if (!listen(this->_socket, SOMAXCONN))
		return ;
	throw (srvSocket::listenError());
}

srvSocket::~srvSocket() { close(this->_socket); }

const char	*srvSocket::initError::what() const throw() { return ("socket: error: init"); }
const char	*srvSocket::fcntlError::what() const throw() { return ("socket: error: fcntl"); }
const char	*srvSocket::bindError::what() const throw() { return ("socket: error: bind"); }
const char	*srvSocket::listenError::what() const throw() { return ("socket: error: listen"); }
