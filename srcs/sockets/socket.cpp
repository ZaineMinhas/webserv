/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:06:34 by aliens            #+#    #+#             */
/*   Updated: 2022/11/04 12:06:25 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

void	srvSocket::initSocket(int port) {
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	this->_addrlen = sizeof(this->_addr);
	if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)))
		return ;
	throw (srvSocket::initError());
}

void	srvSocket::bindSocket()
{
	if (!bind(this->_socket, (const sockaddr *)&this->_addr, this->_addrlen))
		return ;
	throw (srvSocket::bindError());
}

void	srvSocket::listenSocket()
{
	if (!listen(this->_socket, SOMAXCONN))
		return ;
	throw (srvSocket::listenError());
}

void	srvSocket::closeSocket() { close(this->_socket); }

const char	*srvSocket::initError::what() const throw() { return ("socket: error: init"); }
const char	*srvSocket::bindError::what() const throw() { return ("socket: error: bind"); }
const char	*srvSocket::listenError::what() const throw() { return ("socket: error: listen"); }
