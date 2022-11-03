/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 14:04:18 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/03 16:28:55 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "webserv.hpp"

/*___  ___*/
////////////////////
///              ///
////////////////////


////////////////////
///    SERVER    ///
////////////////////

/*___ canonical form ___*/

server::server(void) {}

server::server(const server &src) {}

server::~server(void) {}

server	&server::operator=(const server &src) {
	return (*this);
}

/*___ setters ___*/

void	server::setName(std::string &name) { _name = name; }
void	server::setRoot(std::string &root) { _root = root; }
void	server::setAutoindex(bool &autoindex) { _autoindex = autoindex; }
void	server::setPort(unsigned int &port) { _port = port; }
void	server::setBodySize(unsigned int &size) { _body_size = size; }
void	server::setDirectories(std::vector<directory> &directories) { _directories = directories; }
void	server::setMethods(std::vector<std::string> &methods) { _methods = methods; }
void	server::setErrorPages(std::map<unsigned int, std::string> &error_pages) { _error_pages = error_pages; }

/*___ getters ___*/

std::string							server::getName(void) const { return (_name); }
std::string							server::getRoot(void) const { return (_root); }
bool								server::getAutoindex(void) const { return (_autoindex); }
unsigned int						server::getPort(void) const {return (_port); }
unsigned int						server::getBodySize(void) const { return (_body_size); }
std::vector<directory>				server::getDirectories(void) const { return (_directories); }
std::vector<std::string>			server::getMethods(void) const { return (_methods); }
std::map<unsigned int, std::string>	server::getErrorPages(void) const { return (_error_pages); }

/*___ utils ___*/

void	server::set(std::string &key, std::string &value)
{
	if (key == "server_name")
		this->setName(value);
	else if (key == "root")
		this->setRoot(value);
	else if (key == "autoindex")
		this->setAutoindex(value);
	else if (key == "listen")
		this->setPort(value);
	else if (key == "body_size")
		this->setBodySize(value);
	else if (key == "method")
		this->setMethods(value);
	else if (key == error_page)
		this->setErrorPages(value);
}

////////////////////
///  DIRECTORY   ///
////////////////////

/*___ canonical form ___*/

directory::directory(void) {}

directory::directory(const directory &src) {}

directory::~directory(void) {}

directory	&directory::operator=(const directory &src) {
	return (*this);
}

/*___ setters ___*/

void	directory::setName(std::string &name) { _name = name; }
void	directory::setRoot(std::string &root) { _root = root; }
void	directory::setIndex(std::string &index) { _index = index; }

void	directory::setMethods(std::string &methods)
{
	std::istringstream	f(methods);
	std::string			s;
	while (getline(f, s, ' '))
	{
		if (s != "GET" && s != "POST" && s != "DELETE")
			throw (webserv::badConfFile());
		this->_methods.push_back(s);
	}
}

void	directory::setHttpRedirect(std::string &redirects)
{
	std::istringstream	f(redirects);
	std::string			s;
	std::string			url;
	unsigned int		val = 0;
	if (getline(f, s, ' '))
	{
		std::istringstream	fval(s);
		fval >> val;
	}
	else
		throw (webserv::badConfFile());
	if (getline(f, s, ' '))
		url = s;
	else
		throw (webserv::badConfFile());
	this->_http_redirect = std::make_pair<unsigned int, std::string>(val, url);
}
void	directory::setAutoindex(std::string &autoindex)
{
	if (autoindex == "on")
		this->_autoindex = true;
	else if (autoindex == "off")
		this->_autoindex = false;
	else
		throw (webserv::badConfFile());
}

/*___ getters ___*/

std::string								directory::getName(void) const { return (_name); }
std::string								directory::getRoot(void) const { return (_root); }
std::string								directory::getIndex(void) const { return (_index); }
std::vector<std::string>				directory::getMethods(void) const { return (_methods); }
std::pair<unsigned int, std::string>	directory::getHttpRedirect(void) const { return (_http_redirect); }
bool									directory::getAutoindex(void) const { return (_autoindex); }

/*___ utils ___*/

void	directory::set(std::string &key, std::string &value)
{
	if (key == "server_name")
		this->setName(value);
	else if (key == "root")
		this->setRoot(value);
	else if (key == "index")
		this->setIndex(value);
	else if (key == "methods")
		setMethods(value);		//faire un constructor methods(std::string)
	else if (key == "redirect")
		setHttpRedirect(value);		//faire un splitter dans setHttpRedirect
	else if (key == "autoindex")
		setAutoindex(value);
}

