/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHttp.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:15:55 by aliens            #+#    #+#             */
/*   Updated: 2022/11/23 15:05:16 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHTTP_HPP
#define RESPONSEHTTP_HPP

#include "server.hpp"

class responseHttp
{
private:
    std::vector<serverBlock>        _servers;
    std::vector<directory>          _directories;
    std::vector<std::string>        _request;
    std::string						_response;
	std::pair<std::string, size_t>	_host;
	std::string						_htmlFileName;
	size_t							_i_s;
	size_t							_i_d;

    void    _getServerIndex();
    void    _getLocationIndex();

    void    _createHeader();

    void    _addHtml();

public:
    responseHttp(std::vector<std::string> request, std::vector<serverBlock> servers);
    ~responseHttp();

    void    createResponse();

    const char  *toSend() const;
    size_t      size() const;
    
};

#endif