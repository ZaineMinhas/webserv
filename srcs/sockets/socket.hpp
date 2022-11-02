/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:02:19 by aliens            #+#    #+#             */
/*   Updated: 2022/11/02 13:53:53 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

struct client;

struct tcpSocket {
	int			_socket;
	sockaddr_in	_addr;
	socklen_t	_addrlen;
	
	tcpSocket(bool client = false, int domain = AF_INET, int port = 8080);
	~tcpSocket();

	void	initSocket(int domain, int type, int protocol);
	void	closeSocket();

	void	bindSocket(int domain, int port);
	void	listenSocket();
	
	void	connectSocket(int domain, const std::string &addr, int port);
	void	acceptSocket(client *client);

	int		recvSocket(client *client, char *buffer, unsigned int len);
	int		sendSocket(const char *data, unsigned int len);

	struct initError : public std::exception { virtual const char	*what() const throw(); };
	struct bindError : public std::exception { virtual const char	*what() const throw(); };
	struct recvError : public std::exception { virtual const char	*what() const throw(); };
	struct sendError : public std::exception { virtual const char	*what() const throw(); };
	struct listenError : public std::exception { virtual const char	*what() const throw(); };
	struct acceptError : public std::exception { virtual const char	*what() const throw(); };
	struct connectError : public std::exception { virtual const char	*what() const throw(); };

};

struct client {
	tcpSocket	_cli;

	const char	*getAddr();
};

#endif
