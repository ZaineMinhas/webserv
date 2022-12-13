/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 15:24:57 by ctirions          #+#    #+#             */
/*   Updated: 2022/12/12 16:58:37 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_2_HPP
# define UTILS_2_HPP

# include <iostream>
# include "server.hpp"

typedef struct responseConfig
{
	std::string					path;
	bool						autoindex;
	size_t						bodySize;
	std::string					root;
	std::string					index;
	std::vector<std::string>	methods;

	void	setServ(serverBlock serv);
	void	setLoc(directory loc);

}	respConf;

bool		urlCompare(std::string url1, std::string url2);
std::string	urlJoin(std::string urlBase, std::string toAdd);
std::string	rtrim(std::string str, std::string toDel);

#endif