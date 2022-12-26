/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 15:24:57 by ctirions          #+#    #+#             */
/*   Updated: 2022/12/26 17:59:07 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_2_HPP
# define UTILS_2_HPP

# include <iostream>
# include "server.hpp"

typedef struct responseConfig
{
	bool							autoindex;
	size_t							bodySize;
	std::string						path;
	std::string						root;
	std::string						index;
	std::vector<std::string>		methods;
	std::pair<size_t, std::string>	redirect;

	void	setServ(serverBlock serv);
	void	setLoc(directory loc);

}	respConf;

bool		urlCompare(std::string url1, std::string url2);
std::string	urlJoin(std::string urlBase, std::string toAdd);
std::string	rtrim(std::string str, std::string toDel);
size_t		stringToSize(std::string str);
std::string	sizeToString(size_t nb);

#endif