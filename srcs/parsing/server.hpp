/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:00:26 by ctirions          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/11/01 12:10:49 by ctirions         ###   ########.fr       */
=======
/*   Updated: 2022/11/01 11:52:16 by ctirions         ###   ########.fr       */
>>>>>>> cbb753b3b56a06263f4593950bed03afaa27d900
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <vector>
# include <utility>

class	directory;

class server {
private:
	std::string								name;
	int										port;
	bool									autoindex;
	unsigned int							body_size;
	std::vector<directory>					directories;
	std::vector<std::string>				methods;
	std::pair<unsigned int, std::string>	error_page;


public:
	server();
	~server();
};

class directory {
private:
	std::string								name;
	std::string								root;
	std::string								index;
	std::vector<std::string>				methods;
	std::pair<unsigned int, std::string>	http_redirect;
	bool									autoindex;

public:
	directory();
	~directory();

};

#endif
