/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:08:13 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/11 13:10:53 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int argc, char **argv) {
	webserv	srv;
	try {
		if (argc == 2)
			srv.check_conf_file(argv[1], srv);
		else
			srv.check_conf_file("./conf_files/file1.conf", srv);
	}
	catch (std::exception &e) { std::cout << e.what() << std::endl; }

	return (0);
}
