/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 10:53:57 by aliens            #+#    #+#             */
/*   Updated: 2022/11/04 15:58:40 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <sys/select.h>

int	main(void)
{
	server	srv;
	try { srv.initServer(8080); }
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (-1);
	}
	while (1)
	{
		fd_set	set;
		timeval	timeout = {0};
		FD_ZERO(&set);
		FD_SET(srv._srv_sock._socket, &set);
		int srv_ready = select(srv._srv_sock._socket + 1, &set, NULL, NULL, &timeout);
		if (srv_ready == -1)
			return (-1);
		else if (srv_ready > 0)
		{
			sockaddr_in	from = {0};
			socklen_t	fromlen = sizeof(from);
			int	new_client = accept(srv._srv_sock._socket, (sockaddr *)&from, &fromlen);
			if (new_client)
			{
				client	cli;
				cli._cli = new_client;
				cli._from = from;
				srv._cli_socks.push_back(cli);
			}
			fd_set	set_reads;
			fd_set	set_write;
			fd_set	set_errors;
			int	fd = 0;
			timeval	timeout = {0};
			for (std::vector<client>::iterator it = srv._cli_socks.begin(); it != srv._cli_socks.end(); it++)
			{
				// std::cout << "coucou1" << std::endl;
				FD_SET(new_client, &set_reads);
				FD_SET(new_client, &set_write);
				FD_SET(new_client, &set_errors);
				if (new_client > fd)
					fd = new_client;
			}
			int	cliens_ready = select(fd + 1, &set_reads, &set_write, &set_errors, &timeout);
			if (cliens_ready == -1)
				return (-1);
			else if (cliens_ready > 0)
			{
				std::vector<client>::iterator it = srv._cli_socks.begin();
				while (it != srv._cli_socks.end())
				{
					// std::cout << "coucou" << std::endl;
					bool	disconnect = false;
					if (FD_ISSET(it->_cli, &set_errors))
						disconnect = true;
					else if (FD_ISSET(it->_cli, &set_reads))
					{
						char	buffer[200];
						if (recv(it->_cli, buffer, 199, 0) == -1)
							disconnect = true;
						else
							std::cout << buffer << std::endl;
					}
					if (disconnect)
						it = srv._cli_socks.erase(it);
					else
						it++;
				}
			}
		}
	}
	return (0);
}
