/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:23:06 by aliens            #+#    #+#             */
/*   Updated: 2022/12/05 13:45:31 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "socket.hpp"
#include "../parsing/config.hpp"

#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#include <sstream>
#include <iterator>
#include <vector>

struct server {
	std::vector<srvSocket>		_servers;
	std::vector<client>			_clients;
	fd_set						_read_set;
	fd_set						_write_set;
	fd_set						_tmp_set;
	timeval						_timeout;

	server(std::vector<size_t> ports);
	server(const server &srv);
	~server();

	server	&operator=(const server &srv);
	
	void	handle_client(config &srv);

	struct selectError : public std::exception { virtual const char *what() const throw(); };
	struct recvError : public std::exception { virtual const char *what() const throw(); };
};

#endif
