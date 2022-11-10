/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 10:53:57 by aliens            #+#    #+#             */
/*   Updated: 2022/11/09 21:08:58 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <sys/select.h>

int	main(void)
{
	std::vector<size_t>	ports;
	ports.push_back(8080);
	
	server webserv;

	try { webserv.init_server(ports); }
	catch (std::exception &e) { std::cout << e.what() << std::endl; }

	try { webserv.handle_client(); }
	catch (std::exception &e) { std::cout << e.what() << std::endl; }

	return (0);
}
