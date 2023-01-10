/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHttp.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:20:33 by aliens            #+#    #+#             */
/*   Updated: 2023/01/10 16:31:59 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "responseHttp.hpp"
#include <dirent.h>
#include <iostream>
#include <unistd.h>

void    responseHttp::_getServerIndex(void)
{
	if (_header.find("Host:") == _header.end())
	{
		_i_s = _servers.size();
		errorPage("400");
		return ;
	}
	std::string	host = _header.at("Host:");

	if (host.find(":") != std::string::npos)
	{
		_host.first = host.substr(0, host.find(":"));
		std::stringstream	ss(host.substr(host.find(":") + 1, host.size() - host.find(":") + 1));
		ss >> _host.second;
	}
	else
	{
		_host.first = host.substr(0, host.size() - 1);
		_host.second = 80;
	}

	for (std::vector<serverBlock>::iterator it = _servers.begin(); it != _servers.end(); it++, _i_s++)
		if ((_host.first == it->getListen().first && _host.second == it->getListen().second) || (_host.first == it->getName() && _host.second == it->getListen().second))
			break ;
}

bool    responseHttp::_getLocationIndex(void)
{
	if (_header.find("file:") == _header.end() || _i_s == _servers.size()) {
		errorPage("400");
		_makeResponseList();
		return (false);
	}
	std::string	url = _header.at("file:") + "/";
    _directories = _servers[_i_s].getDirectories();
	int	id = -1;

	for (std::vector<directory>::iterator it = _directories.begin(); it != _directories.end(); it++, _i_d++)
	{	
		if (url.find(it->getName() + "/") != std::string::npos)
		{
			if (id == -1)
				id = _i_d;
			else if (it->getName().size() > _directories[id].getName().size())
				id = _i_d;
		}
		else if (it->getName() == "/" && id == -1)
			id = _i_d;
	}
	id == -1 ? _i_d = _directories.size() : _i_d = id;
	return (true);
}

std::vector<std::string>	responseHttp::_generateRedirect(void)
{
	if (_redirect.first == 308)
		_response = "HTTP/1.1 308 Permanent Redirect\nLocation: " + _redirect.second + "\n\r\n\r\n\r";
	else
		_response = "HTTP/1.1 " + sizeToString(_redirect.first) + " Moved Permanently\nLocation: " + _redirect.second + "\n\r\n\r\n\r";
	_makeResponseList();
	return (_responseList);
}

bool	responseHttp::_createAutoIndex(void)
{

	struct dirent	*d;
	DIR				*dr;
	dr = opendir(_fileName.c_str());
	std::string	tmp = _fileName.substr(0, _fileName.rfind("/") + 1);
	_htmlTxt = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset='utf-8'/>\n<link rel='shortcut icon' type='image/x-icon' href='/images/favicon.ico'/>\n<title>Index</title>\n</head>\n<body>\n<h1>Index of " + tmp.substr(tmp.rfind("/")) + " :</h1>\n<hr/><ul>\n";
	std::string	dir = tmp;
	if (dr)
	{
		for (d = readdir(dr); d; d = readdir(dr))
		{
			if (dir == "//")
				dir = "/";
			_htmlTxt += "<li><a href='" + dir + std::string(d->d_name) + "'>" + std::string(d->d_name) + "</a></li>\n";
		}
		_htmlTxt += "</ul>\n</body>\n</html>";
		closedir(dr);
	}
	else
		return (errorPage("404"));
	_createHeader("200");
	return (false);
}

bool	responseHttp::_findFileName(void)
{
	std::string	path = _header.at("file:");
	respConf	conf;
	conf.setServ(_servers[_i_s]);
	if (_header.find("version:") == _header.end())
		return (errorPage("400"));
	if (_header.at("version:") != "HTTP/1.1")
		return (errorPage("505"));
	if (_i_d != _servers[_i_s].getDirectories().size()) // if location
	{
		directory	loc = _servers[_i_s].getDirectories()[_i_d];
		conf.setLoc(loc);
		if (!loc.getRoot().empty()) // if root in location
			conf.path = urlJoin(conf.path, path.substr(loc.getName().size()));
		else
			conf.path = urlJoin(conf.path, path);
	}
	else
		conf.path += rtrim(path, "/");
	if (urlCompare(conf.path, conf.root) && !conf.index.empty()) // add index if exist
		conf.path = urlJoin(conf.path, conf.index);

	_bodySize = conf.bodySize;
	_fileName = conf.path;
	_methods = conf.methods;
	_autoindex = conf.autoindex;
	_redirect = conf.redirect;
	_upload = conf.upload;

	if (_fileName.size() > 2083)
		return (errorPage("414"));
	if (_header.find("method:") == _header.end())
		return (errorPage("400"));
	if (!_checkMethods())
		return (false);
	if (_header.at("method:") == "POST") {
		if (_header.find("Content-Length:") == _header.end())
			return (errorPage("411"));
		_htmlTxt = make_cgi(".py");
		if (!_htmlTxt.empty())
			_createHeader("201");
		return (false);
	}
	else if (_fileName.find(".py") != std::string::npos || _fileName.find(".php") != std::string::npos) {
		_htmlTxt = make_cgi(_fileName.find(".py") != std::string::npos ? ".py" : ".php");
		if (!_htmlTxt.empty())
			_createHeader("200");
		return (false);
	}
	return (_getMime());
}

bool	responseHttp::_checkMethods(void)
{
	std::string allMethods[6] = {"HEAD", "PUT", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
	for (int i = 0; i < 9; i++)
		if (allMethods[i] == _header.at("method:"))
			return (errorPage("501"));
	if (_header.at("method:") != "POST" && _header.at("method:") != "DELETE" && _header.at("method:") != "GET")
		return (errorPage("400"));
	std::vector<std::string>::iterator	it = _methods.begin();
	for (; it != _methods.end(); it++)
		if (*it == _header.at("method:"))
			break ;
	if (it == _methods.end())
	{
		_unauthorized = true;
		return (errorPage("405"));
	}
	return (true);
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
		else if (fileType == ".txt")
			_mime = "text/plain";
		else if (fileType == ".cpp")
			_mime = "text/plain";
		else if (fileType == ".conf")
			_mime = "text/plain";
		else if (fileType == ".gif")
			_mime = "image/gif";
		else if (fileType == ".png")
			_mime = "image/png";
		else if (fileType == ".jpeg" || fileType == ".jpg")
			_mime = "image/jpeg";
		else if (fileType == ".webp")
			_mime = "image/webp";
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
	if (_header.find("version:") == _header.end())
		return (errorPage("400"));
	_response = _header.at("version:") + " " + code + " " + getMsgCode(code);

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
	std::ifstream	ftxt(_fileName.c_str());

	if (ftxt) {
		std::stringstream	ss;
		ss << ftxt.rdbuf();
		_htmlTxt = ss.str();
		ftxt.close();
	}
	else
		return (errorPage("404")); // No page to return --> ERROR
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
	_response.clear();
}

char	**responseHttp::_createEnv()
{
	std::vector<std::string>	env;
	char						buffer[PATH_MAX];
	char						**ret;

	env.push_back("AUTH_TYPE=");

	env.push_back("GATEWAY_INTERFACE=CGI/1.1");

	env.push_back("UPLOAD_DIR=" + _upload);
	
	if (_header.at("method:") == "POST") {
		env.push_back("PATH_INFO=" + _body);
		env.push_back("QUERY_STRING=" + _body);
		if (_header.find("Content-Length:") == _header.end() ||  _header.find("Content-Type:") == _header.end()) {
			errorPage("400");
			return (NULL);
		}
		env.push_back("CONTENT_LENGTH=" + _header.at("Content-Length:"));
		env.push_back("CONTENT_TYPE=" + _header.at("Content-Type:"));
	}
	else {
		env.push_back("PATH_INFO=" + _fileName.substr(_fileName.find("?") + 1));
		env.push_back("QUERY_STRING=" + _fileName.substr(_fileName.find("?") + 1));
		env.push_back("CONTENT_LENGTH=");
	}
	
	getcwd(buffer, PATH_MAX);
	env.push_back("PATH_TRANSLATED=" + std::string(buffer) + _header.at("file:"));

	env.push_back("REMOTE_ADDR=");
	env.push_back("REMOTE_HOST=");
	env.push_back("REMOTE_IDENT=");
	env.push_back("REMOTE_USER=");
	env.push_back("REQUEST_METHOD=" + _header.at("method:"));
	env.push_back("SCRIPT_NAME=" + _fileName.substr(0, _fileName.find("?")));

	if (!_servers[_i_s].getName().empty())
		env.push_back("SERVER_NAME=" + _servers[_i_s].getName());
	else
		env.push_back("SERVER_NAME=" + _servers[_i_s].getListen().first);

	std::string	str = sizeToString(_servers[_i_s].getListen().second);
	env.push_back("SERVER_PORTS=" + str);

	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("SERVER_SOFTWARE=JoJo_SERVER/0.1");

	if (_header.find("Accept:") == _header.end() ||  _header.find("Accept-Language:") == _header.end() || _header.find("User-Agent:") == _header.end() || _header.find("Referer:") == _header.end())
	{
		errorPage("400");
		return (NULL);
	}
	env.push_back("HTPP_ACCEPT=" + _header.at("Accept:"));
	env.push_back("HTPP_ACCEPT_LANGUAGE=" + _header.at("Accept-Language:"));
	env.push_back("HTTP_USER_AGENT=" + _header.at("User-Agent:"));
	env.push_back("HTTP_REFERER=" + _header.at("Referer:"));

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

responseHttp::responseHttp(std::string body, std::map<std::string, std::string> header, std::vector<serverBlock> servers) : _header(header), _servers(servers), _body(body), _i_s(0), _i_d(0), _unauthorized(false) {}

responseHttp::~responseHttp(void) {}

const char  *responseHttp::toSend(void) const { return(_response.c_str()); }
std::string	responseHttp::getResponse(void) const { return (_response); }
int      	responseHttp::size(void) const { return(_response.size()); }

std::vector<std::string>    responseHttp::createResponse(void)
{
	_getServerIndex();
	if (!_getLocationIndex())
		return (_responseList);
	if (_findFileName())
		if (_addHtml())
			_createHeader("200");
	if (!_redirect.second.empty())
		return (_generateRedirect());
	if (_header.at("method:") == "DELETE" && !_unauthorized)
	{
		if (!remove(_fileName.c_str()))
			_response = "HTTP/1.1 204 No Content\nContent-Length: 0\r\n\r\n";
		else
			errorPage("404");
	}
	if (_htmlTxt.size() > _bodySize)
		errorPage("413");
	_makeResponseList();
	return (_responseList);
}

bool	responseHttp::errorPage(std::string code)
{
	_mime = "text/html";
	_fileName.clear();
	_response.clear();

	if (_i_s != _servers.size() && !_servers[_i_s].getErrorPages()[stringToSize(code)].empty())
		_fileName = _servers[_i_s].getErrorPages()[stringToSize(code)];
	else
	{
		_fileName = "./error_pages";
		struct dirent	*d;
		DIR				*dr;
		dr = opendir(_fileName.c_str());
		if (dr)
		{
			for (d = readdir(dr); d; d = readdir(dr))
			{
				if (std::string(d->d_name).find(code) != std::string::npos)
				{
					_fileName += "/";
					_fileName += d->d_name;
					break ;
				}
			}
		}
	}
	_addHtml();
	_createHeader(code);
	return (false);
}

std::string	responseHttp::make_cgi(std::string ext)
{
	char		**env;
	char		buffer[1024] = {0};
	int			fd_in[2];
	int			fd_out[2];
	int 		status = 0;
	int			ret = 1;
	std::string	str;
	pid_t		pid;

	if (!(env = _createEnv()))
		return ("");
	if (pipe(fd_in) == -1 || pipe(fd_out) == -1)
		return ("");
	
	if ((pid = fork()) == -1)
		return ("");
	else if (pid == 0)
	{
		if (dup2(fd_in[0], STDIN_FILENO) == -1)
			exit(1);
		if (dup2(fd_out[1], STDOUT_FILENO) == -1)
			exit(1);

		close(fd_in[0]);
		close(fd_in[1]);
		close(fd_out[0]);
		close(fd_out[1]);

		char *av[3];
		std::string	tmp, exec_path;
		if (ext == ".php")
		{
			tmp = _fileName;
			exec_path = "/usr/bin/php";
		}
		else
		{
			tmp = "cgi-bin" + _header.at("file:").substr(_header.at("file:").rfind("/"));
			exec_path = "/usr/bin/python3";
		}
		
		if (tmp.find("?") != std::string::npos)
			tmp.erase(tmp.find("?"));

		av[0] = (char *)exec_path.c_str();
		av[1] = (char *)tmp.c_str();
		av[2] = NULL;

		execve(av[0], av, env);
		for (int i = 0; env[i]; i++)
			delete env[i];
		delete env;
		
		exit(1);
	}
	if (dup2(fd_in[0], STDIN_FILENO) == -1)
		return ("");
	if (_header.at("method:") == "POST") {
		if (write(fd_in[1], _body.c_str(), _body.size()) <= 0) {
			errorPage("500");
			for (int i = 0; env[i]; i++)
				delete env[i];
			delete env;
			return ("");
		}
	}

	close(fd_out[1]);
	close(fd_in[0]);
	close(fd_in[1]);
	for (int i = 0; env[i]; i++)
		delete env[i];
	delete env;
	
	waitpid(pid, &status, 0);
	while (ret > 0)
	{
		ret = read(fd_out[0], buffer, 1024);
		if (ret == -1)
			buffer[0] = '\0';
		else
			buffer[ret] = '\0';
		str += std::string(buffer);
	}
	close(fd_out[0]);
	return (str);
}

std::string responseHttp::getMsgCode(std::string code)
{
	if (code == "200")
		return ("Ok.");
	else if (code == "201")
		return ("Created.");
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
