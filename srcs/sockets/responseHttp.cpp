/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHttp.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:20:33 by aliens            #+#    #+#             */
/*   Updated: 2022/11/27 18:47:13 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseHttp.hpp"

void    responseHttp::_getServerIndex()
{
	for (std::vector<std::string>::iterator it = this->_request.begin(); it != this->_request.end(); it++)
	{
		if (*it == "Host:")
		{
			it++;
			this->_host.first = it->substr(0, it->find(":"));
			std::stringstream	ss(it->substr(it->find(":") + 1, it->size() - it->find(":") + 1));
			ss >> this->_host.second;
			break ;
		}
	}
	for (std::vector<serverBlock>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++, this->_i_s++)
		if (this->_host == it->getListen())
			break ;
}

void    responseHttp::_getLocationIndex()
{
    this->_directories = this->_servers[this->_i_s].getDirectories();
	int	id = -1;

	for (std::vector<directory>::iterator it = this->_directories.begin(); it != this->_directories.end(); it++, this->_i_d++) {	
		if (it->getName() == this->_request[1])
		{
			if (id == -1)
				id = this->_i_d;
			else if (it->getName().size() > this->_directories[id].getName().size())
				id = this->_i_d;
		}
	}
	id == -1 ? this->_directories.size() : this->_i_d = id;
}

bool    responseHttp::_createHeader()
{
	if (this->_i_d != this->_directories.size()) // if location
	{
		std::string	root;
		if (this->_directories[this->_i_d].getRoot().empty()) // if root
			root = this->_servers[this->_i_s].getRoot();

		this->_htmlFileName += root;
		if (this->_request[1].size() == this->_directories[this->_i_d].getName().size())
		{
			if (!this->_directories[this->_i_d].getIndex().empty())
				this->_htmlFileName += "/" + this->_directories[this->_i_d].getIndex();
			else if (this->_directories[this->_i_d].getAutoindex())
				; // create autoindex
			else if (this->_servers[this->_i_s].getAutoindex())
				; // create autoindex
			else
				return (this->_errorPage("500"));
		}
		else
			this->_htmlFileName += this->_request[1].substr(this->_directories[this->_i_d].getName().size(), this->_request[1].size() - this->_directories[this->_i_d].getName().size());
		
		std::cout << "File name : " << this->_htmlFileName << std::endl;
		std::cout << std::endl << "---------------------" << std::endl;

		this->_response += this->_request[2] + " 200 Ok" + ""/*size_t to string*/ + "\r\n\r\n";
		return (true);
	}
	
	size_t	untilSlash = this->_servers[this->_i_s].getRoot().find("/");
	this->_htmlFileName += this->_servers[this->_i_s].getRoot();
	if (this->_servers[this->_i_s].getRoot().substr(untilSlash, this->_servers[this->_i_s].getRoot().size() - untilSlash) == this->_request[1].substr(0, this->_servers[this->_i_s].getRoot().size() - 1))
		this->_htmlFileName += "/" + this->_request[1].substr(this->_servers[this->_i_s].getRoot().size(), this->_request[1].size() - this->_servers[this->_i_s].getRoot().size());
	else
		this->_htmlFileName += this->_request[1];
    
	std::cout << "File name : " << this->_htmlFileName << std::endl;
	std::cout << std::endl << "---------------------" << std::endl;

	this->_response += this->_request[2] + " 200 Ok" + ""/*size_t to string*/ + "\r\n\r\n";
	return (true);
}

bool	responseHttp::_errorPage(std::string code)
{
	this->_htmlFileName.clear();
	this->_response.clear();
	
	for (std::vector<directory>::iterator it = this->_servers[this->_i_s].getDirectories().begin(); it < this->_servers[this->_i_s].getDirectories().end(); it++ )
	{
		if (it->getName() == "error_pages")
		{
			!it->getRoot().empty() ? this->_htmlFileName += it->getRoot() : this->_htmlFileName += this->_servers[this->_i_s].getRoot();
			break;
		}
	}
	if (this->_htmlFileName.empty())
		this->_htmlFileName += "./error_pages";

	this->_htmlFileName += "/" + code + ".html";
	this->_response += this->_request[2] + " " + code + ""/*size_t to string*/ + "\r\n\r\n";
	this->_addHtml();

	std::cout << "Error page : " << this->_htmlFileName << std::endl;
	std::cout << std::endl << "---------------------" << std::endl;

	return (false);
}

bool    responseHttp::_addHtml()
{
    std::string		htmlTxt;
	std::ifstream	ftxt(this->_htmlFileName.c_str());
	if (ftxt) {
		std::ostringstream	ss;
		ss << ftxt.rdbuf();
		htmlTxt = ss.str();
	}
	else
		return (false); // No page to return --> ERROR
	
	this->_response += htmlTxt;

	return (true);
}

/////////////////////////////////////////////////////////////////

responseHttp::responseHttp(std::vector<std::string> request, std::vector<serverBlock> servers) : _servers(servers), _request(request), _i_s(0), _i_d(0) {}

responseHttp::~responseHttp() {}

const char  *responseHttp::toSend() const { return(this->_response.c_str()); }
size_t      responseHttp::size() const { return(this->_response.size()); }

void    responseHttp::createResponse()
{
    this->_getServerIndex();
    this->_getLocationIndex();
    if (!this->_createHeader())
		return ;
	if (!this->_addHtml())
		this->_errorPage("404");
}
