/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHttp.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens < aliens@student.s19.be >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:20:33 by aliens            #+#    #+#             */
/*   Updated: 2022/12/06 18:13:44 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseHttp.hpp"

void    responseHttp::_getServerIndex(void)
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

void    responseHttp::_getLocationIndex(void)
{
    this->_directories = this->_servers[this->_i_s].getDirectories();
	int	id = -1;

	for (std::vector<directory>::iterator it = this->_directories.begin(); it != this->_directories.end(); it++, this->_i_d++)
	{	
		std::string	toCompare = this->_request[1].substr(0, it->getName().size());
		if (it->getName() == toCompare)
		{
			if (id == -1)
				id = this->_i_d;
			else if (it->getName().size() > this->_directories[id].getName().size())
				id = this->_i_d;
		}
	}
	id == -1 ? this->_directories.size() : this->_i_d = id;
}


bool	responseHttp::_findFileName(void)
{
	if (this->_i_d == this->_directories.size()) // if no location
		this->_fileName = this->_servers[this->_i_s].getRoot() + this->_request[1];
	else // if location
	{
		if (!this->_directories[this->_i_d].getRoot().empty()) // if root
		{
			this->_fileName += this->_directories[this->_i_d].getRoot();
			if (this->_request[1] == this->_directories[this->_i_d].getName())
			{
				if (!this->_directories[this->_i_d].getIndex().empty())
					this->_fileName += "/" + this->_directories[this->_i_d].getIndex();
				else if (this->_directories[this->_i_d].getAutoindex())
					; // create autoindex
				else if (this->_servers[this->_i_s].getAutoindex())
					; // create autoindex
				else
					return (this->_errorPage("500"));
			}
			else
				this->_fileName += this->_request[1].substr(this->_directories[this->_i_d].getName().size(), this->_request[1].size() - this->_directories[this->_i_d].getName().size());			
		}
		else // if no root
		{
			size_t	untilSlash = this->_servers[this->_i_s].getRoot().find("/");
			this->_fileName += this->_servers[this->_i_s].getRoot();
			if (this->_servers[this->_i_s].getRoot().substr(untilSlash, this->_servers[this->_i_s].getRoot().size() - untilSlash) == this->_request[1] || \
				this->_directories[this->_i_d].getName() == this->_request[1])
			{
				if (!this->_directories[this->_i_d].getIndex().empty())
					this->_fileName += "/" + this->_directories[this->_i_d].getIndex();
				else if (this->_directories[this->_i_d].getAutoindex())
					; // create autoindex
				else if (this->_servers[this->_i_s].getAutoindex())
					; // create autoindex
				else
					return (this->_errorPage("500"));
			}
			else
			{
				if (this->_servers[this->_i_s].getRoot().substr(untilSlash, this->_servers[this->_i_s].getRoot().size() - untilSlash) == this->_request[1].substr(0, this->_servers[this->_i_s].getRoot().size() - untilSlash))
					this->_fileName += "/" + this->_request[1].substr(this->_servers[this->_i_s].getRoot().size(), this->_request[1].size() - this->_servers[this->_i_s].getRoot().size());
				else
					this->_fileName += this->_request[1];
			}
		}
	}
	return (true);
}

bool    responseHttp::_createHeader(void)
{
	this->_response += this->_request[2] + " 200 OK";

	std::string	mime = "";
	size_t		pos = _fileName.find_last_of(".");
	
	if (pos != std::string::npos) // get Content-type
	{
		std::string	fileType = _fileName.substr(pos, _fileName.size() - pos);
		if (fileType == ".css")
			mime = "text/css";
		else if (fileType == ".html")
			mime = "text/html";
		else if (fileType == ".gif")
			mime = "image/gif";
		else if (fileType == ".png")
			mime = "image/png";
		else if (fileType == ".jpeg" || fileType == ".jpg")
			mime = "image/jpeg";
		else if (fileType == ".json")
			mime = "application/json";
		else if (fileType == ".js")
			mime = "application/javascript";
		else if (fileType == ".pdf")
			mime = "application/pdf";
		else if (fileType == ".ico")
			mime = "image/vnd.microsoft.icon";
		else if (fileType == ".woff")
			mime = "font/woff";
		else if (fileType == ".woff2")
			mime = "font/woff2";
	}

	std::stringstream	length;

	length << _htmlTxt.size();
	_response += "\nContent-Length: " + length.str();
	if (!mime.empty())
		_response += "\nContent-Type: " + mime + "\r\n\r\n" + _htmlTxt;
	// for (size_t index = 0; _response.size() > index * 65536; index++)
	// 	std::cout << _response.substr(index * 65536, (index + 1) * 65536).c_str() << std::endl;
	return (true);
}

bool	responseHttp::_errorPage(std::string code)
{
	this->_fileName.clear();
	this->_response.clear();
	
	for (std::vector<directory>::iterator it = this->_servers[this->_i_s].getDirectories().begin(); it < this->_servers[this->_i_s].getDirectories().end(); it++ )
	{
		if (it->getName() == "error_pages")
		{
			!it->getRoot().empty() ? this->_fileName += it->getRoot() : this->_fileName += this->_servers[this->_i_s].getRoot();
			break;
		}
	}
	if (this->_fileName.empty())
		this->_fileName += "./error_pages";

	this->_fileName += "/" + code + ".html";
	this->_response += this->_request[2] + " " + code + ""/*size_t to string*/ + "\r\n\r\n";
	this->_addHtml();

	return (false);
}

bool    responseHttp::_addHtml(void)
{
    std::string		htmlTxt;
	std::ifstream	ftxt(this->_fileName.c_str());

	if (ftxt) {
		std::stringstream	ss;
		ss << ftxt.rdbuf();
		htmlTxt = ss.str();
	}
	else
		return (this->_errorPage("404")); // No page to return --> ERROR
	_htmlTxt = htmlTxt;
	return (true);
}

void	responseHttp::_makeResponseList(void)
{
	size_t	bufferSize = 65536;
	
	while (_response.size() > bufferSize)
	{
		_responseList.push_back(_response.substr(0, bufferSize));
		_response = _response.substr(bufferSize);
	}
	_responseList.push_back(_response);
	_response = "";
}

/////////////////////////////////////////////////////////////////

responseHttp::responseHttp(std::vector<std::string> request, std::vector<serverBlock> servers) : _servers(servers), _request(request), _i_s(0), _i_d(0) {}

responseHttp::~responseHttp(void) {}

const char  *responseHttp::toSend(void) const { return(this->_response.c_str()); }
std::string	responseHttp::getResponse(void) const { return (this->_response); }
int      	responseHttp::size(void) const { return(this->_response.size()); }

std::vector<std::string>    responseHttp::createResponse(void)
{
    this->_getServerIndex();
    this->_getLocationIndex();
	if (this->_findFileName() && this->_addHtml() && this->_createHeader())
		std::cout << "coucou" << std::endl; // trouver quoi faire !
	this->_makeResponseList();
	return (this->_responseList);
}