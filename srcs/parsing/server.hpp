/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:00:26 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/01 14:54:23 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <vector>
# include <utility>

	/*___  ___*/

class	directory;

////////////////////
///    SERVER    ///
////////////////////

class server {
private:
	std::string								_name;
	std::string								_root;
	bool									_autoindex;
	unsigned int							_port;
	unsigned int							_body_size;
	std::vector<directory>					_directories;
	std::vector<std::string>				_methods;
	std::pair<unsigned int, std::string>	_error_pages;

public:

	/*___ constructors / destructor ___*/

	server(void);
	~server(void);

	/*___ setters ___*/

	void	setName(std::string &name);
	void	setRoot(std::string &root);
	void	setAutoindex(bool &autoindex);
	void	setPort(unsigned int &port);
	void	setBodySize(unsigned int &size);
	void	setDirectories(std::vector<directory> &directories);
	void	setMethods(std::vector<std::string> &methods);
	void	setErrorPages(std::pair<unsigned int, std::string> &error_pages);

	/*___ getters ___*/

	std::string								getName(void) const;
	std::string								getRoot(void) const;
	bool									getAutoindex(void) const;
	unsigned int							getPort(void) const;
	unsigned int							getBodySize(void) const;
	std::vector<directory>					getDirectories(void) const;
	std::vector<std::string>				getMethods(void) const;
	std::pair<unsigned int, std::string>	getErrorPages(void) const;

};

////////////////////
///  DIRECTORY   ///
////////////////////

class directory {
private:
	std::string								_name;
	std::string								_root;
	std::string								_index;
	std::vector<std::string>				_methods;
	std::pair<unsigned int, std::string>	_http_redirect;
	bool									_autoindex;

public:

	/*___ constructors / destructor ___*/

	directory();
	~directory();

	/*___ setters ___*/

	void	setName(std::string &name);
	void	setRoot(std::string &root);
	void	setIndex(std::string &index);
	void	setMethods(std::vector<std::string> &methods);
	void	setHttpRedirect(std::pair<unsigned int, std::string> &redirects);
	void	setAutoindex(bool &autoindex);

	/*___ getters ___*/

	std::string								getName(void) const;
	std::string								getRoot(void) const;
	std::string								getIndex(void) const;
	std::vector<std::string>				getMethods(void) const;
	std::pair<unsigned int, std::string>	getHttpRedirect(void) const;
	bool									getAutoindex(void) const;
};

#endif
