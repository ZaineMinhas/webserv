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

#include <vector>
#include <map>
#include <poll.h>

int main()
{
	tcpSocket	srv_sock;
	char		buffer[200] = {0};
	int			bufferlen = 199;
	
	try {
		srv_sock.initSocket(AF_INET, SOCK_STREAM, 0);
		srv_sock.bindSocket(AF_INET, 8080);
		srv_sock.listenSocket();
	}
	catch (std::exception &e) { std::cout << e.what() << std::endl; }

	std::cout << "OK server" << std::endl;

	std::vector<pollfd>			poll_clients;
	std::map<int, tcpSocket>	clients;


	while (1)
	{
		{
			pollfd		poll_srv;
			poll_srv.fd = srv_sock._socket;
			poll_srv.events = POLLIN;

			if (poll(&poll_srv, 1, 0) == -1) {
				std::cout << "poll error" << std::endl;
				break ;
			}

			tcpSocket	cli_sock(true);
			try { srv_sock.acceptSocket(&cli_sock); }
			catch (std::exception &e) { std::cout << e.what() << std::endl; }


			pollfd	poll_cli;
			poll_cli.fd = cli_sock._socket;
			poll_cli.events = POLLIN | POLLOUT;

			poll_clients.push_back(poll_cli);
			clients[cli_sock._socket] = cli_sock;
		}

		if (!poll_clients.empty())
		{
			if (poll(poll_clients.data(), static_cast<nfds_t>(poll_clients.size()), 0) == -1) {
				std::cout << "poll error" << std::endl;
				return (1);
			}
			for (std::vector<pollfd>::iterator it_poll = poll_clients.begin(); it_poll != poll_clients.end(); it_poll++)
			{
				std::map<int, tcpSocket>::iterator	it_cli = clients.find(it_poll->fd);
				if (it_cli == clients.end()) {
					it_poll = poll_clients.erase(it_poll);
					continue;
				}
				tcpSocket	client = it_cli->second;
				bool		disconnect = false;
				if (it_poll->revents & POLLERR)
				{
					int			err;
					socklen_t	errlen = sizeof(err);
					if (getsockopt(client._socket, SOL_SOCKET, SO_ERROR, reinterpret_cast<char *>(&err), &errlen) != 0) {
						std::cout << "unknow error" << std::endl;
						disconnect = true;
					}
					if (err != 0) {
						std::cout << "socket: error: " << err << std::endl;
						disconnect = true;
					}
				}
				else if (it_poll->revents & (POLLHUP | POLLNVAL)) {
					disconnect = true;
				}
				else if (it_poll->revents & POLLIN) {
					disconnect = true;
				}
				else if (it_poll->revents & POLLOUT) {
					try { srv_sock.recvSocket(&client, buffer, bufferlen); }
					catch (std::exception &e) { std::cout << e.what() << std::endl; }
					std::cout << client.getAddr() << " :" << std::endl << buffer << std::endl;
					// try { client.sendSocket("coucou\n", 8); }
					// catch (std::exception &e) { std::cout << e.what() << std::endl; }
					disconnect = true;
				}
				if (disconnect) {
					it_poll = poll_clients.erase(it_poll);
					clients.erase(it_cli);
				}
				else
					it_poll++;
			}
		}
	}
	
	srv_sock.closeSocket();
	
	return (0);
}
