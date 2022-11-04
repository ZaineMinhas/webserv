/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:23:06 by aliens            #+#    #+#             */
/*   Updated: 2022/11/04 13:46:03 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "socket.hpp"

#include <vector>

struct server {
	srvSocket				_srv_sock;
	std::vector<client>		_cli_socks;

	void	initServer(int port);
	
};

#endif
