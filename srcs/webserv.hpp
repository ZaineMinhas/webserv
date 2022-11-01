/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:05:28 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/01 17:29:15 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "parsing/server.hpp"

////////////////////
///   WEBSERV    ///
////////////////////

class webserv {
private:
	std::vector<server>	servers;

public:

	/*___ canonical form ___*/

	webserv(void);
	webserv(const webserv &src);
	~webserv(void);
	webserv	&operator=(const webserv &src);

	/*___ utils ___*/

	void	check_conf_files(std::string file);

	/*___ exceptions ___*/

	class	badConfFile : public std::exception {
	public:
		virtual const char	*what() const throw();
	};

	class	badInitialization : public std::exception {
	public:
		virtual const char	*what() const throw();
	};
};

#endif
