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

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int srv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (!srv_sock) {
		std::cout << "socket_error" << std::endl;
		return (-1);
	}

	struct sockaddr_in	addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);

	if (bind(srv_sock, (sockaddr *)&addr, sizeof(addr)) < 0) {
		std::cout << "bind_error" << std::endl;
		return (-1);
	}

	if (listen(srv_sock, SOMAXCONN) < 0) {
		std::cout << "listen_error" << std::endl;
		return (-1);
	}

	std::cout << "OK server" << std::endl;

	while (1)
	{	
		socklen_t	addrlen = sizeof(addr);
	
		int cli_sock;
		if ((cli_sock = accept(srv_sock, (sockaddr *)&addr, &addrlen)) < 0) {
			std::cout << "accept_error" << std::endl;
			break ;
		}
	
		char	buffer[500] = {0};
		if (!(recv(cli_sock, buffer, 499, 0)))
			break ;
		std::cout << buffer << std::endl;
	}
	
	close(srv_sock);
	
	return (0);
}
