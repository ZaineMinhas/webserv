/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHttp.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:15:55 by aliens            #+#    #+#             */
/*   Updated: 2023/01/07 16:07:00 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
#define RESPONSEHTTP_HPP

#include "server.hpp"
#include "utils.hpp"
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

class responseHttp
{
private:
    std::map<std::string, std::string>  _header;
	std::pair<std::string, size_t>	    _host;
	std::pair<size_t, std::string>	    _redirect;
    std::vector<std::string>            _responseList;
    std::vector<std::string>            _methods;
    std::vector<serverBlock>            _servers;
    std::vector<directory>              _directories;
    std::string						    _response;
	std::string						    _mime;
	std::string						    _upload;
    std::string                         _body;
	std::string						    _fileName;
    std::string                         _htmlTxt;
	size_t							    _i_s;
  	size_t							    _i_d;
    size_t                              _bodySize;
	bool							    _autoindex;
	bool							    _unauthorized;

    void        _getServerIndex();
    bool        _getLocationIndex();

	std::vector<std::string>	_generateRedirect(void);
    bool                        _checkMethods(void);
  	bool						_createAutoIndex(void);
    bool    					_findFileName(void);
	bool						_getMime(void);
    bool    					_createHeader(std::string msg);
    bool    					_addHtml(void);

    void    _makeResponseList(void);

    char    **_createEnv(void);

public:
    responseHttp(std::string body, std::map<std::string, std::string> request, std::vector<serverBlock> servers);
    ~responseHttp(void);

    std::vector<std::string>    createResponse(void);

    const char  *toSend(void) const;
    int         size(void) const;
    
    std::string getResponse(void) const;
    
    bool    errorPage(std::string code);
    
    std::string    make_cgi(std::string ext);
    
    static std::string getMsgCode(std::string code);
};

#endif