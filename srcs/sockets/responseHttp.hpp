/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHttp.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:15:55 by aliens            #+#    #+#             */
/*   Updated: 2022/12/07 14:05:31 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
#define RESPONSEHTTP_HPP

#include "server.hpp"

class responseHttp
{
private:
    std::vector<std::string>        _responseList;
    std::vector<serverBlock>        _servers;
    std::vector<directory>          _directories;
    std::vector<std::string>        _request;
    std::string						_response;
	  std::pair<std::string, size_t>	_host;
	  std::string						_fileName;
    std::string                     _htmlTxt;
	  size_t							_i_s;
  	size_t							_i_d;

    void    _getServerIndex();
    void    _getLocationIndex();

  	void	_createAutoIndex(void);
    
    bool    _findFileName();
    bool    _createHeader(std::string msg);
    bool    _errorPage(std::string code);
    bool    _addHtml();

    void	_makeResponseList();

public:
    responseHttp(std::vector<std::string> request, std::vector<serverBlock> servers);
    ~responseHttp();

    std::vector<std::string>    createResponse();

    const char  *toSend() const;
    int           size() const;
    std::string getResponse() const;
    
};

#endif