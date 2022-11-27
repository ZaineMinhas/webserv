/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:00:26 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/23 01:03:44 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include <stdlib.h>

# include <string>
# include <iostream>
# include <vector>
# include <map>
# include <utility>
# include <exception>
# include <fstream>
# include <sstream>

	/*___  ___*/

class	directory;

////////////////////
///    SERVER    ///
////////////////////

class serverBlock {
private:
	std::string						_name;
	std::string						_root;
	std::pair<std::string, size_t>	_listen;
	bool							_autoindex;
	size_t							_body_size;
	std::vector<directory>			_directories;
	std::vector<std::string>		_methods;
	std::map<size_t, std::string>	_error_pages;

public:

	/*___ canonical form ___*/

	serverBlock(void);
	serverBlock(const serverBlock &src);
	~serverBlock(void);
	serverBlock	&operator=(const serverBlock &src);

	/*___ setters ___*/

	void	setName(std::string &name);
	void	setRoot(std::string &root);
	void	setAutoindex(std::string &autoindex);
	void	setListen(std::string &listen);
	void	setBodySize(std::string &size);
	void	setDirectories(std::vector<directory> &directories);
	void	setMethods(std::string &methods);
	void	setErrorPages(std::string &error_pages);

	/*___ getters ___*/

	std::string						getName(void);
	std::string						getRoot(void);
	std::pair<std::string, size_t>	getListen(void);
	bool							getAutoindex(void);
	size_t							getBodySize(void);
	std::vector<directory>			&getDirectories(void);
	std::vector<std::string>		getMethods(void);
	std::map<size_t, std::string>	getErrorPages(void);

	/*___ utils ___*/

	void	set(std::string &key, std::string &value);
	void	add_directory(const directory &to_add, std::string &name);
	void	checkValues(void);

};

////////////////////
///  DIRECTORY   ///
////////////////////

class directory {
private:
	std::string								_name;
	std::string								_index;
	std::string								_root;
	std::vector<std::string>				_methods;
	std::pair<size_t, std::string>			_http_redirect;
	bool									_autoindex;

public:

	/*___ canonical form ___*/

	directory(void);
	directory(const directory &src);
	~directory(void);
	directory	&operator=(const directory &src);

	/*___ setters ___*/

	void	setName(std::string &name);
	void	setRoot(const std::string &root);
	void	setIndex(std::string &index);
	void	setMethods(std::string &methods);
	void	setHttpRedirect(std::string &redirects);
	void	setAutoindex(std::string &autoindex);

	/*___ getters ___*/

	std::string								getName(void);
	std::string								getRoot(void);
	std::string								getIndex(void);
	std::vector<std::string>				getMethods(void);
	std::pair<size_t, std::string>			getHttpRedirect(void);
	bool									getAutoindex(void);

	/*___ utils ___*/

	void	set(std::string &key, std::string &value);
	void	checkValues(void);
};

#endif
