/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:23:06 by aliens            #+#    #+#             */
/*   Updated: 2022/11/10 15:21:50 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "socket.hpp"

#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#include <vector>

struct server {
	std::vector<srvSocket>		_servers;
	std::vector<client>			_clients;
	fd_set						_cli_set;
	fd_set						_srv_set;
	timeval						_timeout;

	server() {}

	server(const server &srv) {
		this->_servers = srv._servers;
		this->_clients = srv._clients;
		this->_cli_set = srv._cli_set;
		this->_srv_set = srv._srv_set;
		this->_timeout = srv._timeout;
	}

	void	init_server(std::vector<size_t>	ports);
	
	void	handle_client();

	struct selectError : public std::exception { virtual const char *what() const throw(); };
	struct clientError : public std::exception { virtual const char *what() const throw(); };
};

#endif
