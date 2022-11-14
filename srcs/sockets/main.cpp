/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 10:53:57 by aliens            #+#    #+#             */
/*   Updated: 2022/11/14 13:28:54 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <sys/select.h>

int	main(void)
{
	std::vector<size_t>	ports;
	ports.push_back(8080);
	
	try {
		server webserv(ports);
		webserv.handle_client();
	}
	catch (std::exception &e) { 
		std::cout << e.what() << std::endl; 
		return (-1);
	}

	return (0);
}
