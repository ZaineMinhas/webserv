/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:02:19 by aliens            #+#    #+#             */
/*   Updated: 2022/11/14 11:06:23 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <iostream>

struct srvSocket {
	int	_socket;
	sockaddr_in	_addr;
	socklen_t	_addrlen;

	srvSocket(size_t port);
	~srvSocket();

	srvSocket(const srvSocket &srv);

	srvSocket	&operator=(const srvSocket &srv);
	
	struct initError : public std::exception { virtual const char *what() const throw(); };
	struct fcntlError : public std::exception { virtual const char *what() const throw(); };
	struct bindError : public std::exception { virtual const char *what() const throw(); };
	struct listenError : public std::exception { virtual const char *what() const throw(); };

};

struct client {
	int			_cli;
	sockaddr_in	_from;
	socklen_t	_fromlen;

	bool	init_client(int srv);
};


#endif
