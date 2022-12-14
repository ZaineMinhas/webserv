/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 14:04:18 by ctirions          #+#    #+#             */
/*   Updated: 2023/01/10 17:45:37 by aliens           ###   ########.fr       */
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

serverBlock::serverBlock(void) {}

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
	if (_listen.second <= 2)
		throw (config::badPort());
}

void	serverBlock::setBodySize(std::string &size) {
	for (size_t i = 0; i < size.size(); i++)
		if (isalpha(size[i]) || size[i] == '-')
			throw (config::badConfFile("body size"));
	_body_size = atol(size.c_str());
}
void	serverBlock::setDirectories(std::vector<directory> &directories) { _directories = directories; }

void	serverBlock::setMethods(std::string &methods)
{
	std::istringstream	f(methods);
	std::string			s;
	while (getline(f, s, ' '))
	{
		if (s != "GET" && s != "POST" && s != "DELETE")
			throw (config::badConfFile("methods"));
		std::vector<std::string>::iterator	it = _methods.begin();
		for (; it != _methods.end(); it++)
			if (s == *it)
				break ;
		if (it == _methods.end())
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

std::string						serverBlock::getName(void) { return (_name); }
std::string						serverBlock::getRoot(void) { return (_root); }
std::pair<std::string, size_t>	serverBlock::getListen(void) {return (_listen); }
bool							serverBlock::getAutoindex(void) { return (_autoindex); }
size_t							serverBlock::getBodySize(void) { return (_body_size); }
std::vector<directory>			&serverBlock::getDirectories(void) { return (_directories); }
std::vector<std::string>		serverBlock::getMethods(void) { return (_methods); }
std::map<size_t, std::string>	serverBlock::getErrorPages(void) { return (_error_pages); }

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
	if (value.empty())
		throw (config::emptyValue());
}

void	serverBlock::add_directory(const directory &to_add, std::string &name) {
	_directories.push_back(to_add);
	_directories.back().setName(name);
}

void	serverBlock::checkValues(void) {
	if (_name.empty())
		_name.assign("localhost");
	if (_listen.first.empty())
		_listen.first.assign("127.0.0.1");
	if (!_listen.second)
		_listen.second = 1919;
	if (_root.empty())
		_root = "./";
}

////////////////////
///  DIRECTORY   ///
////////////////////

/*___ canonical form ___*/

directory::directory(void)
{
	_autoindex = false;
	_autoindexIsSet = false;
}

directory::directory(const directory &src) { *this = src; }

directory::~directory(void) {}

directory	&directory::operator=(const directory &src) {
	_name = src._name;
	_root = src._root;
	_index = src._index;
	_methods = src._methods;
	_http_redirect = src._http_redirect;
	_autoindex = src._autoindex;
	_autoindexIsSet = src._autoindexIsSet;
	_upload = src._upload;
	return (*this);
}

/*___ setters ___*/

void	directory::setName(std::string &name) { _name = name; }
void	directory::setRoot(const std::string &root) { _root = root; }
void	directory::setIndex(std::string &index) { _index = index; }

void	directory::setMethods(std::string &methods)
{
	std::istringstream	f(methods);
	std::string			s;
	while (getline(f, s, ' '))
	{
		if (s != "GET" && s != "POST" && s != "DELETE")
			throw (config::badConfFile("methods"));
		std::vector<std::string>::iterator	it = _methods.begin();
		for (; it != _methods.end(); it++)
			if (s == *it)
				break ;
		if (it == _methods.end())
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
	_autoindexIsSet = true;
	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
	else
		throw (config::badConfFile("autoindex"));
}

void	directory::setUpload(std::string &upload) { _upload = upload; }


/*___ getters ___*/

std::string						directory::getName(void) { return (_name); }
std::string						directory::getRoot(void) { return (_root); }
std::string						directory::getIndex(void) { return (_index); }
std::vector<std::string>		directory::getMethods(void) { return (_methods); }
std::pair<size_t, std::string>	directory::getHttpRedirect(void) { return (_http_redirect); }
bool							directory::getAutoindex(void) { return (_autoindex); }
bool							directory::getAutoindexIsSet(void) { return (_autoindexIsSet); }
std::string						directory::getUpload(void) { return (_upload); }

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
	else if (key == "upload") {
		std::string	meth("POST");
		setUpload(value);
		setMethods(meth);
	}
	else
		throw (config::badConfFile(std::string("bad key: ") + key));
	if (value.empty())
		throw (config::emptyValue());
}

void	directory::checkValues(void) {
	if (_name.empty())
		throw (config::badConfFile("location name"));
	if (_upload.empty()) {
		std::string	path("./website/site/uploads/");
		setUpload(path);
	}
	else if (_upload.find("/") != _upload.size() - 1)
		_upload += "/";
}
