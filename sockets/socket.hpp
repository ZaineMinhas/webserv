/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:02:19 by aliens            #+#    #+#             */
/*   Updated: 2022/11/02 10:13:07 by aliens           ###   ########.fr       */
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

struct client
{
	tcpSocket	_cli;
	sockaddr_in	_srv;
};

class tcpSocket {
private:
	int			_socket = 0;
	sockaddr_in	_addr = {0};
	socklen_t	_addrlen = 0;
	
public:
	tcpSocket(int domain = AF_INET, int port = 8080);
	~tcpSocket();

	bool	initSocket(int domain, int type, int protocol);
	void	closeSocket();

	bool	bindSocket(int domain, int port);
	bool	listenSocket();
	
	bool	connectSocket(int domain, const std::string &addr, int port);
	bool	acceptSocket(client *client);

	int		recvSocket(char *buffer, unsigned int len);
	int		sendSocket(const char *data, unsigned int len);

	class errorSocket : public std::exception {
	public:
		virtual const char	*bindError() const throw();
		virtual const char	*recvError() const throw();
		virtual const char	*sendError() const throw();
		virtual const char	*listenError() const throw();
		virtual const char	*acceptError() const throw();
		
	};
	
};

#endif
