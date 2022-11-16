/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:53:28 by aliens            #+#    #+#             */
/*   Updated: 2022/11/16 17:24:23 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "srcs/parsing/config.hpp"
#include "srcs/sockets/server.hpp"

int	main(int argc, char **argv) {
	config	srv;
	try {
		if (argc == 2)
			srv.check_conf_file(argv[1], srv);
		else
			srv.check_conf_file("./conf_files/file1.conf", srv);
	}
	catch (std::exception &e) { std::cout << e.what() << std::endl; }

    try {
		server webserv(srv.getPorts());
		webserv.handle_client();
	}
	catch (std::exception &e) { 
		std::cout << e.what() << std::endl; 
		return (-1);
	}

	return (0);
}