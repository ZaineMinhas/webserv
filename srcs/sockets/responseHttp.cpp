/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHttp.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:20:33 by aliens            #+#    #+#             */
/*   Updated: 2022/11/23 15:06:00 by aliens           ###   ########.fr       */
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

	for (std::vector<directory>::iterator it = this->_directories.begin(); it != this->_directories.end(); it++, this->_i_d++)
		if (it->getName() == this->_request[1])
			break ;
}

void    responseHttp::_createHeader()
{
    if (this->_i_d == this->_directories.size())
	{
		this->_htmlFileName = this->_servers[this->_i_s].getRoot() + this->_request[1];
		std::cout << this->_htmlFileName << std::endl;
	}
	else
	{
		if (!this->_directories[this->_i_d].getRoot().empty())
			this->_htmlFileName += this->_directories[this->_i_d].getRoot();
		else
			this->_htmlFileName += this->_servers[this->_i_s].getRoot();
		if (!this->_directories[this->_i_d].getIndex().empty())
			this->_htmlFileName += "/" + this->_directories[this->_i_d].getIndex();
	}
    
    std::cout << "File name : " << this->_htmlFileName << std::endl;

	this->_response += this->_request[2] + " 200 Ok" + ""/*size_t to string*/ + "\r\n\r\n";
}

void    responseHttp::_addHtml()
{
    std::string		htmlTxt;
	std::ifstream	ftxt(this->_htmlFileName.c_str());
	if (ftxt) {
		std::ostringstream	ss;
		ss << ftxt.rdbuf();
		htmlTxt = ss.str();
	}
	else
		; // No page to return --> ERROR
	this->_response += htmlTxt;
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
    this->_createHeader();
    this->_addHtml();
}
