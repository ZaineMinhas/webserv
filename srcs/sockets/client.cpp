/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 13:17:57 by aliens            #+#    #+#             */
/*   Updated: 2022/10/28 13:17:57 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{	
	int	client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sock == -1) {
		std::cout << "socket_error" << std::endl;
		return (-1);
	}

	struct sockaddr_in	server;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(8080);

	if (connect(client_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
		std::cout << "socket_connection_error" << std::endl;
		return (-1);
	}
	std::cout << "OK" << std::endl;
	close(client_sock);
	return (0);
}
