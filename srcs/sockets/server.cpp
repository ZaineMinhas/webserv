/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 13:41:45 by aliens            #+#    #+#             */
/*   Updated: 2022/10/28 13:41:45 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

int main()
{
	tcpSocket	srv_sock;
	char		buffer[1024] = {0};
	int			bufferlen = 1023;

	try {
		srv_sock.initSocket(AF_INET, SOCK_STREAM, 0);
		srv_sock.bindSocket(AF_INET, 8080);
		srv_sock.listenSocket();
	}
	catch (std::exception &e) { std::cout << e.what() << std::endl; }

	std::cout << "OK server" << std::endl;

	while (1) {	
		client cli_sock;
		try {
			srv_sock.acceptSocket(&cli_sock);
			srv_sock.recvSocket(&cli_sock, buffer, bufferlen);
		}
		catch (std::exception &e) { std::cout << e.what() << std::endl; }

		std::cout << buffer << std::endl;
	}
	
	srv_sock.closeSocket();
	
	return (0);
}
