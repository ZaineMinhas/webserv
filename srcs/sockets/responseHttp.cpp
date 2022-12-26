/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHttp.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:20:33 by aliens            #+#    #+#             */
/*   Updated: 2022/12/26 18:19:05 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseHttp.hpp"
#include <dirent.h>
#include <iostream>
#include <unistd.h>

void    responseHttp::_getServerIndex(void)
{
	std::string	host = _request.at("Host:");

	this->_host.first = host.substr(0, host.find(":"));
	std::stringstream	ss(host.substr(host.find(":") + 1, host.size() - host.find(":") + 1));
	ss >> this->_host.second;
	
	for (std::vector<serverBlock>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++, this->_i_s++)
		if (this->_host == it->getListen())
			break ;
}

void    responseHttp::_getLocationIndex(void)
{
	std::string	url = this->_request.at("file:") + "/";
	if (_i_s == _servers.size())
		return ; // create an error of bad resquest
    this->_directories = this->_servers[this->_i_s].getDirectories();
	int	id = -1;

	for (std::vector<directory>::iterator it = this->_directories.begin(); it != this->_directories.end(); it++, this->_i_d++)
	{	
		if (url.find(it->getName() + "/") != std::string::npos)
		{
			if (id == -1)
				id = this->_i_d;
			else if (it->getName().size() > this->_directories[id].getName().size())
				id = this->_i_d;
		}
		else if (it->getName() == "/")
			id = this->_i_d;
	}
	id == -1 ? this->_i_d = this->_directories.size() : this->_i_d = id;
}

std::vector<std::string>	responseHttp::_generateRedirect(void)
{
	if (_redirect.first == 308)
		_htmlTxt = "HTTP/1.1 308 Permanent Redirect\nLocation: " + _redirect.second + "\r\n\r\n";
	else
		 _htmlTxt = "HTTP/1.1 " + sizeToString(_redirect.first) + "Moved Permanently\nLocation: " + _redirect.second + "\r\n\r\n";
	_makeResponseList();
	return (_responseList);
}

bool	responseHttp::_createAutoIndex(void)
{
	struct dirent	*d;
	DIR				*dr;
	dr = opendir(_fileName.c_str());
	std::string	tmp = _fileName.substr(0, _fileName.rfind("/"));
	_htmlTxt = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset='utf-8'/>\n<link rel='shortcut icon' type='image/x-icon' href='/images/favicon.ico'/>\n<title>Index</title>\n</head>\n<body>\n<h1>Index of " + tmp.substr(tmp.rfind("/")) + " :</h1>\n<hr/><ul>\n";
	std::string	dir = _servers[_i_s].getDirectories()[_i_d].getName() + "/";
	if (dir.empty())
		dir = _fileName;
	if (dr)
	{
		for (d = readdir(dr); d; d = readdir(dr))
			_htmlTxt += "<li><a href='" + dir + std::string(d->d_name) + "'>" + std::string(d->d_name) + "</a></li>\n";
		_htmlTxt += "</ul>\n</body>\n</html>";
		closedir(dr);
	}
	else
		return (errorPage("404"));
	_createHeader("200");
	return (false);
}

std::string	responseHttp::_getMsgCode(std::string code)
{
	if (code == "200")
		return ("Ok.");
	else if (code == "400")
		return ("Bad request.");
	else if (code == "401")
		return ("Unauthorized authentication request.");
	else if (code == "402")
		return ("Payment Required.");
	else if (code == "403")
		return ("You don't have permission to access this resource.");
	else if (code == "404")
		return ("Page not found.");
	else if (code == "405")
		return ("Method Not Allowed error.");
	else if (code == "406")
		return ("Not Acceptable.");
	else if (code == "407")
		return ("Proxy Authentication Required.");
	else if (code == "408")
		return ("Request Timeout.");
	else if (code == "409")
		return ("Conflict.");
	else if (code == "410")
		return ("Asset no longer exists.");
	else if (code == "411")
		return ("Length Required.");
	else if (code == "412")
		return ("Access to this resource has been denied.");
	else if (code == "413")
		return ("Request is too big.");
	else if (code == "414")
		return ("Request URL too large.");
	else if (code == "415")
		return ("Unsupported Media Type.");
	else if (code == "500")
		return ("Internal Server Error.");
	else if (code == "501")
		return ("Server do not support this request type.");
	else if (code == "502")
		return ("Bad Gateway server.");
	else if (code == "503")
		return ("Service Unavailable server.");
	else if (code == "504")
		return ("Gateway Timeout server.");
	else if (code == "505")
		return ("HTTP Version Not Supported.");
	else
		return ("");
}

bool	responseHttp::_findFileName(void)
{
	std::string	path = _request.at("file:");
	respConf	conf;
	conf.setServ(_servers[_i_s]);
	if (_i_d != _servers[_i_s].getDirectories().size()) // if location
	{
		directory	loc = _servers[_i_s].getDirectories()[_i_d];
		conf.setLoc(loc);
		if (!loc.getRoot().empty()) // if root in location
			conf.path = urlJoin(conf.path, path.substr(loc.getName().size()));
		else
			conf.path = urlJoin(conf.path, path);
		// set errors pages
	}
	else
	{
		conf.path += rtrim(path, "/");
		// set errors pages	
	}
	if (urlCompare(conf.path, conf.root) && !conf.index.empty()) // add index if exist
		conf.path = urlJoin(conf.path, conf.index);

	_fileName = conf.path;
	_autoindex = conf.autoindex;
	_redirect = conf.redirect;
	if (_fileName.find(".py") != std::string::npos)
		return (make_cgi());
	return (_getMime());
}

bool	responseHttp::_getMime(void)
{
	size_t		pos = _fileName.find_last_of(".");
	
	if (pos != std::string::npos) // get Content-type
	{
		std::string	fileType = _fileName.substr(pos, _fileName.size() - pos);
		if (fileType == ".css")
			_mime = "text/css";
		else if (fileType == ".html")
			_mime = "text/html";
		else if (fileType == ".gif")
			_mime = "image/gif";
		else if (fileType == ".png")
			_mime = "image/png";
		else if (fileType == ".jpeg" || fileType == ".jpg")
			_mime = "image/jpeg";
		else if (fileType == ".json")
			_mime = "application/json";
		else if (fileType == ".js")
			_mime = "application/javascript";
		else if (fileType == ".pdf")
			_mime = "application/pdf";
		else if (fileType == ".ico")
			_mime = "image/vnd.microsoft.icon";
		else if (fileType == ".woff")
			_mime = "font/woff";
		else if (fileType == ".woff2")
			_mime = "font/woff2";
	}
	if (_mime.empty())
	{

		if (!_autoindex)
			return (errorPage("404"));
		else
			_createAutoIndex();
		return (false);	
	}
	return (true);
}

bool    responseHttp::_createHeader(std::string code)
{
	this->_response = this->_request.at("version:") + " " + code + " " + this->_getMsgCode(code);

	std::stringstream	length;

	length << _htmlTxt.size();
	_response += "\nContent-Length: " + length.str();
	if (!_mime.empty())
		_response += "\nContent-Type: " + _mime;
	_response += "\r\n\r\n" + _htmlTxt;
	return (true);
}

bool    responseHttp::_addHtml(void)
{
    std::string		htmlTxt;
	std::ifstream	ftxt(this->_fileName.c_str());

	if (ftxt) {
		std::stringstream	ss;
		ss << ftxt.rdbuf();
		htmlTxt = ss.str();
		ftxt.close();
	}
	else
		return (this->errorPage("404")); // No page to return --> ERROR
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

char	**responseHttp::_createEnv()
{
	std::vector<std::string>	env;
	char						buffer[PATH_MAX];
	char						**ret;

	env.push_back("AUTH_TYPE=");
	env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	if (_request.at("method:") == "POST")
		env.push_back("PATH_INFO=" + _body);
	else
		env.push_back("PATH_INFO=" + _fileName.substr(_fileName.find("?") + 1));
	getcwd(buffer, PATH_MAX);
	env.push_back("PATH_TRANSLATED=" + std::string(buffer) + _request.at("file:"));
	if (_request.at("method:") == "POST")
		env.push_back("QUERY_STRING=" + _body);
	else
		env.push_back("QUERY_STRING=" + _fileName.substr(_fileName.find("?") + 1));
	env.push_back("REMOTE_ADDR=");
	env.push_back("REMOTE_HOST=");
	env.push_back("REMOTE_IDENT=");
	env.push_back("REMOTE_USER=");
	env.push_back("REQUEST_METHOD=" + _request.at("method:"));
	env.push_back("SCRIPT_NAME=" + _fileName.substr(0, _fileName.find("?")));
	if (!_servers[_i_s].getName().empty())
		env.push_back("SERVER_NAME=" + _servers[_i_s].getName());
	else
		env.push_back("SERVER_NAME=" + _servers[_i_s].getListen().first);
	std::string	str = sizeToString(_servers[_i_s].getListen().second);
	env.push_back("SERVER_PORTS=" + str);
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("SERVER_SOFTWARE=JoJo_SERVER/0.1");
	env.push_back("HTPP_ACCEPT=" + _request.at("Accept:")); // bien verifier que c'est le bon indice pour toute les requetes @!!!!!!!!!
	env.push_back("HTPP_ACCEPT_LANGUAGE=" + _request.at("Accept-Language:")); // bien verifier que c'est le bon indice pour toute les requetes @!!!!!!!!!
	env.push_back("HTTP_USER_AGENT=" + _request.at("User-Agent:"));
	env.push_back("HTTP_REFERER=" + _request.at("Referer:"));

	ret = new char *[env.size() + 1];
	int i = 0;
	for (std::vector<std::string>::iterator it = env.begin(); it != env.end(); it++)
	{
		ret[i] = new char[it->size() + 1];
		strcpy(ret[i], it->c_str());
		i++;
	}
	ret[env.size()] = NULL;
	return (ret);
}

/////////////////////////////////////////////////////////////////

responseHttp::responseHttp(std::string body, std::map<std::string, std::string> request, std::vector<serverBlock> servers) : _servers(servers), _request(request), _body(body), _i_s(0), _i_d(0) {}

responseHttp::~responseHttp(void) {}

const char  *responseHttp::toSend(void) const { return(this->_response.c_str()); }
std::string	responseHttp::getResponse(void) const { return (this->_response); }
int      	responseHttp::size(void) const { return(this->_response.size()); }

std::vector<std::string>    responseHttp::createResponse(void)
{
	if (!_redirect.second.empty())
		return (_generateRedirect());
	this->_getServerIndex();
	this->_getLocationIndex();
	if (this->_findFileName())
		if (this->_addHtml())
			this->_createHeader("200");
	this->_makeResponseList();
	return (this->_responseList);
}

bool	responseHttp::errorPage(std::string code)
{
	this->_fileName.clear();
	this->_response.clear();

	if (!_servers[_i_s].getErrorPages()[stringToSize(code)].empty())
		_fileName = _servers[_i_s].getErrorPages()[stringToSize(code)];
	else
	{
		this->_fileName = "./error_pages";
		struct dirent	*d;
		DIR				*dr;
		dr = opendir(_fileName.c_str());
		if (dr)
		{
			for (d = readdir(dr); d; d = readdir(dr))
				if (std::string(d->d_name).find(code) != std::string::npos)
				{
					this->_fileName += "/";
					this->_fileName += d->d_name;
					break ;
				}
		}
	}
	this->_addHtml();
	this->_createHeader(code);
	return (false);
}

bool	responseHttp::make_cgi()
{
	char	**env;
	int		fd[2];
	int 	status = 0;
	pid_t	pid;

	env = this->_createEnv();
	if (pipe(fd) == -1)
		return (true);
	
	if ((pid = fork()) == -1)
		return (true);
	else if (pid == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit(1);

		close(fd[0]);

		char *av[3];
		std::string tmp = "cgi-bin" + _request.at("file:").substr(0, _request.at("file:").find("?"));
		std::string exec_path = "/usr/bin/python3"; // pour les scripts en python, il faudra d'autres chemin pour d'autres scripts

		av[0] = (char *)exec_path.c_str();
		av[1] = (char *)tmp.c_str();
		av[2] = NULL;

		execve(av[0], av, env);
		exit(1);
	}
	else
	{
		char buffer[1024] = {0};
		close(fd[1]);
		for (int ret = 1; ret > 0; ret = read(fd[0], buffer, 1024))
		{
			buffer[ret] = '\0';
			_htmlTxt += buffer;
		}
	}
	close(fd[1]);
	close(fd[0]);
	waitpid(pid, &status, 0);
		
	for (int i = 0; env[i]; i++)
		delete env[i];
	delete env;
	
	_createHeader("401");
	return (false);
}
