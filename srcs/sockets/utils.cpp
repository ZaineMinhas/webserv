/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 15:24:41 by ctirions          #+#    #+#             */
/*   Updated: 2022/12/12 16:58:45 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void	respConf::setServ(serverBlock serv)
{
	autoindex = serv.getAutoindex();
	bodySize = serv.getBodySize();
	root = serv.getRoot();
	path = root;
	methods = serv.getMethods();
}

void	respConf::setLoc(directory loc)
{
	if (loc.getAutoindexIsSet())
		autoindex = loc.getAutoindex();
	if (!loc.getRoot().empty())
		root = loc.getRoot();
	if (!loc.getIndex().empty())
		index = loc.getIndex();
	if (!loc.getMethods().empty())
		methods = loc.getMethods();
}

bool	urlCompare(std::string url1, std::string url2)
{
	if (url1.find_last_of("/") != url1.size() - 1)
		url1 += "/";
	if (url2.find_last_of("/") != url2.size() - 1)
		url2 += "/";
	return (url1 == url2);
}

std::string	urlJoin(std::string url, std::string toAdd)
{
	if (url.find_last_of("/") == url.size() - 1 || !toAdd.find("/"))
		return (url + toAdd);
	return (url + "/" + toAdd);
}

std::string	rtrim(std::string str, std::string toDel)
{
	size_t	end = str.find_last_not_of(toDel);
	return ((end != std::string::npos) ? str.substr(0, end + 1) : "");
}