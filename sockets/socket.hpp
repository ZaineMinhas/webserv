/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:02:19 by aliens            #+#    #+#             */
/*   Updated: 2022/11/01 16:06:18 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class tcpSocket {
private:
	int	_socket;

public:
	tcpSocket();
	~tcpSocket();
	
};

struct client
{
	tcpSocket	cli_sock;
	sockaddr_in	addr;
};

