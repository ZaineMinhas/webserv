/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 14:04:18 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/21 14:14:09 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

/*___  ___*/

////////////////////
///              ///
////////////////////


////////////////////
///    SERVER    ///
////////////////////

/*___ canonical form ___*/

serverBlock::serverBlock(void) : _autoindex(false), _body_size(0) {}

serverBlock::serverBlock(const serverBlock &src) { *this = src; }

serverBlock::~serverBlock(void) {}

serverBlock	&serverBlock::operator=(const serverBlock &src) {
	_name = src._name;
	_root = src._root;
	_listen = src._listen;
	_autoindex = src._autoindex;
	_body_size = src._body_size;
	_directories = src._directories;
	_methods = src._methods;
	_error_pages = src._error_pages;
	return (*this);
}

/*___ setters ___*/

void	serverBlock::setName(std::string &name) { _name = name; }
void	serverBlock::setRoot(std::string &root) { _root = root; }

void	serverBlock::setAutoindex(std::string &autoindex)
{
	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
	else
		throw (config::badConfFile("autoindex"));
}

void	serverBlock::setListen(std::string &listen)
{
	std::istringstream	f(listen);
	std::string			s;
	while (getline(f, s, ':'))
	{
		if (s.find('.') != std::string::npos)
			_listen.first = s;
		else
			_listen.second = atol(s.c_str());
	}
}

void	serverBlock::setBodySize(std::string &size) { _body_size = atol(size.c_str()); }
void	serverBlock::setDirectories(std::vector<directory> &directories) { _directories = directories; }

void	serverBlock::setMethods(std::string &methods)
{
	std::istringstream	f(methods);
	std::string			s;
	while (getline(f, s, ' '))
	{
		if (s != "GET" && s != "POST" && s != "DELETE")
			throw (config::badConfFile("methods"));
		_methods.push_back(s);
	}
}

void	serverBlock::setErrorPages(std::string &error_pages)
{
	std::istringstream	f(error_pages);
	std::string			s;
	std::string			url;
	size_t				nb = 0;
	if (getline(f, s, ' '))
		nb = atol(s.c_str());
	if (getline(f, s, ' '))
		url = s;
	_error_pages.insert(std::make_pair(nb, url));
}

/*___ getters ___*/

std::string						serverBlock::getName(void) const { return (_name); }
std::string						serverBlock::getRoot(void) const { return (_root); }
std::pair<std::string, size_t>	serverBlock::getListen(void) const {return (_listen); }
bool							serverBlock::getAutoindex(void) const { return (_autoindex); }
size_t							serverBlock::getBodySize(void) const { return (_body_size); }
std::vector<directory>			serverBlock::getDirectories(void) const { return (_directories); }
std::vector<std::string>		serverBlock::getMethods(void) const { return (_methods); }
std::map<size_t, std::string>	serverBlock::getErrorPages(void) const { return (_error_pages); }

/*___ utils ___*/

void	serverBlock::set(std::string &key, std::string &value)
{
	if (key == "server_name")
		setName(value);
	else if (key == "root")
		setRoot(value);
	else if (key == "autoindex")
		setAutoindex(value);
	else if (key == "listen")
		setListen(value);
	else if (key == "body_size")
		setBodySize(value);
	else if (key == "method")
		setMethods(value);
	else if (key == "error_page")
		setErrorPages(value);
	else
		throw (config::badConfFile(std::string("bad key: ") + key));
}

void	serverBlock::add_directory(directory to_add, std::string &name) {
	_directories.push_back(to_add);
	_directories.back().setName(name);
}

void	serverBlock::checkValues(void) {
	if (this->_name.empty())
		this->_name.assign("localhost");
	if (this->_listen.first.empty())
		this->_listen.first.assign("127.0.0.1");
	if (!this->_listen.second)
		this->_listen.second = 8080;
}

////////////////////
///  DIRECTORY   ///
////////////////////

/*___ canonical form ___*/

directory::directory(void) {}

directory::directory(const directory &src) { *this = src; }

directory::~directory(void) {}

directory	&directory::operator=(const directory &src) {
	_name = src._name;
	_root = src._root;
	_index = src._index;
	_methods = src._methods;
	_http_redirect = src._http_redirect;
	_autoindex = src._autoindex;
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
			throw (config::badConfFile("methods"));
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
	_http_redirect = std::make_pair(val, url);
}
void	directory::setAutoindex(std::string &autoindex)
{
	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
	else
		throw (config::badConfFile("autoindex"));
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
	else if (key == "method")
		setMethods(value);
	else if (key == "redirect")
		setHttpRedirect(value);
	else if (key == "autoindex")
		setAutoindex(value);
	else
		throw (config::badConfFile(std::string("bad key: ") + key));
}

void	directory::checkValues(void) {
	if (_name.empty())
		throw (config::badConfFile("location name"));
}
