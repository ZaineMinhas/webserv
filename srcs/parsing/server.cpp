/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 14:04:18 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/01 15:06:47 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*___  ___*/
////////////////////
///              ///
////////////////////


////////////////////
///    SERVER    ///
////////////////////

/*___ constructors / destructor ___*/

server::server(void) {}

server::~server(void) {}

/*___ setters ___*/

void	server::setName(std::string &name) { _name = name; }
void	server::setRoot(std::string &root) { _root = root; }
void	server::setAutoindex(bool &autoindex) { _autoindex = autoindex; }
void	server::setPort(unsigned int &port) { _port = port; }
void	server::setBodySize(unsigned int &size) { _body_size = size; }
void	server::setDirectories(std::vector<directory> &directories) { _directories = directories; }
void	server::setMethods(std::vector<std::string> &methods) { _methods = methods; }
void	server::setErrorPages(std::pair<unsigned int, std::string> &error_pages) { _error_pages = error_pages; }

/*___ getters ___*/

std::string								server::getName(void) const { return (_name); }
std::string								server::getRoot(void) const { return (_root); }
bool									server::getAutoindex(void) const { return (_autoindex); }
unsigned int							server::getPort(void) const {return (_port); }
unsigned int							server::getBodySize(void) const { return (_body_size); }
std::vector<directory>					server::getDirectories(void) const { return (_directories); }
std::vector<std::string>				server::getMethods(void) const { return (_methods); }
std::pair<unsigned int, std::string>	server::getErrorPages(void) const { return (_error_pages); }


////////////////////
///  DIRECTORY   ///
////////////////////

/*___ constructors / destructor ___*/

directory::directory(void) {}

directory::~directory(void) {}

/*___ setters ___*/

void	directory::setName(std::string &name) { _name = name; }
void	directory::setRoot(std::string &root) { _root = root; }
void	directory::setIndex(std::string &index) { _index = index; }
void	directory::setMethods(std::vector<std::string> &methods) { _methods = methods; }
void	directory::setHttpRedirect(std::pair<unsigned int, std::string> &redirects) { _http_redirect = redirects; }
void	directory::setAutoindex(bool &autoindex) { _autoindex = autoindex; }

/*___ getters ___*/

std::string								directory::getName(void) const { return (_name); }
std::string								directory::getRoot(void) const { return (_root); }
std::string								directory::getIndex(void) const { return (_index); }
std::vector<std::string>				directory::getMethods(void) const { return (_methods); }
std::pair<unsigned int, std::string>	directory::getHttpRedirect(void) const { return (_http_redirect); }
bool									directory::getAutoindex(void) const { return (_autoindex); }
