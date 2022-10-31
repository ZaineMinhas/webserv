/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:00:26 by ctirions          #+#    #+#             */
/*   Updated: 2022/10/31 17:32:59 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <vector>

class	directory;

class server {
private:
	std::string					name;
	int							port;
	unsigned int				body_size;
	bool						autoindex;
	std::vector<std::string>	methods;
	std::vector<directory>		directories;

public:
	server();
	~server();
};

class directory {
private:
	std::string					name;
	std::string					root;
	std::vector<std::string>	methods;


public:
	directory();
	~directory();

};

#endif
