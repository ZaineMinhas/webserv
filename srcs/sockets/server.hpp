/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:23:06 by aliens            #+#    #+#             */
/*   Updated: 2022/11/09 21:30:19 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "socket.hpp"

#include <string.h>

#include <vector>

struct server {
	std::vector<srvSocket>		_servers;
	std::vector<client>			_clients;
	fd_set						_cli_set;
	fd_set						_srv_set;
	timeval						_timeout;

	void	init_server(std::vector<size_t>	ports);
	
	void	handle_client();

	struct selectError : public std::exception { virtual const char *what() const throw(); };
	struct acceptError : public std::exception { virtual const char *what() const throw(); };
};

#endif
