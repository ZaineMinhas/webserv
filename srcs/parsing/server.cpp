/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 14:04:18 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/04 14:39:50 by ctirions         ###   ########.fr       */
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

server::server(void) : _autoindex(false), _port(0), _body_size(0) {}

server::server(const server &src) {}

server::~server(void) {}

server	&server::operator=(const server &src) {
	return (*this);
}

/*___ setters ___*/

void	server::setName(std::string &name) { _name = name; }
void	server::setRoot(std::string &root) { _root = root; }

void	server::setAutoindex(std::string &autoindex)
{
	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
	else
		throw (webserv::badConfFile());
}

void	server::setPort(std::string &port) { _port = atol(port.c_str()); }
void	server::setBodySize(std::string &size) { _body_size = atol(size.c_str()); }
void	server::setDirectories(std::vector<directory> &directories) { _directories = directories; }

void	server::setMethods(std::string &methods)
{
	std::istringstream	f(methods);
	std::string			s;
	while (getline(f, s, ' '))
	{
		if (s != "GET" && s != "POST" && s != "DELETE")
			throw (webserv::badConfFile());
		_methods.push_back(s);
	}
}

void	server::setErrorPages(std::string &error_pages)
{
	std::istringstream	f(error_pages);
	std::string			s;
	std::string			url;
	size_t				nb = 0;
	if (getline(f, s, ' '))
		nb = atol(s.c_str());
	if (getline(f, s, ' '))
		url = s;
	_error_pages.insert(std::make_pair<size_t, std::string>(nb, url));
}

/*___ getters ___*/

std::string						server::getName(void) const { return (_name); }
std::string						server::getRoot(void) const { return (_root); }
bool							server::getAutoindex(void) const { return (_autoindex); }
size_t							server::getPort(void) const {return (_port); }
size_t							server::getBodySize(void) const { return (_body_size); }
std::vector<directory>			server::getDirectories(void) const { return (_directories); }
std::vector<std::string>		server::getMethods(void) const { return (_methods); }
std::map<size_t, std::string>	server::getErrorPages(void) const { return (_error_pages); }

/*___ utils ___*/

void	server::set(std::string &key, std::string &value)
{
	if (key == "server_name")
		setName(value);
	else if (key == "root")
		setRoot(value);
	else if (key == "autoindex")
		setAutoindex(value);
	else if (key == "listen")
		setPort(value);
	else if (key == "body_size")
		setBodySize(value);
	else if (key == "method")
		setMethods(value);
	else if (key == "error_page")
		setErrorPages(value);
}

void	server::add_directory(directory to_add) { _directories.push_back(to_add); }

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
		_methods.push_back(s);
	}
}

void	directory::setHttpRedirect(std::string &redirects)
{
	std::istringstream	f(redirects);
	std::string			s;
	std::string			url;
	size_t				val = 0;
	if (getline(f, s, ' '))
		val = atol(s.c_str());
	if (getline(f, s, ' '))
		url = s;
	_http_redirect = std::make_pair<size_t, std::string>(val, url);
}
void	directory::setAutoindex(std::string &autoindex)
{
	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
	else
		throw (webserv::badConfFile());
}

/*___ getters ___*/

std::string						directory::getName(void) const { return (_name); }
std::string						directory::getRoot(void) const { return (_root); }
std::string						directory::getIndex(void) const { return (_index); }
std::vector<std::string>		directory::getMethods(void) const { return (_methods); }
std::pair<size_t, std::string>	directory::getHttpRedirect(void) const { return (_http_redirect); }
bool							directory::getAutoindex(void) const { return (_autoindex); }

/*___ utils ___*/

void	directory::set(std::string &key, std::string &value)
{
	if (key == "server_name")
		setName(value);
	else if (key == "root")
		setRoot(value);
	else if (key == "index")
		setIndex(value);
	else if (key == "methods")
		setMethods(value);
	else if (key == "redirect")
		setHttpRedirect(value);		//faire un splitter dans setHttpRedirect
	else if (key == "autoindex")
		setAutoindex(value);
	else
		throw (webserv::badConfFile());
}
