/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:23:06 by aliens            #+#    #+#             */
/*   Updated: 2022/11/02 13:47:33 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "socket.hpp"

#include <poll.h>

#include <vector>

class server {
private:
	tcpSocket			_srv_sock;
	std::vector<client>	_cli_socks;
	pollfd				poll_srv;
	
public:
	server();
	~server();
};

#endif
