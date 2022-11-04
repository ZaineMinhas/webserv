/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:05:28 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/04 14:21:21 by ctirions         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "server.hpp"
# include "utils.hpp"

////////////////////
///   WEBSERV    ///
////////////////////

class webserv {
private:
	std::vector<server>			_servers;

public:

	/*___ canonical form ___*/

	webserv(void);
	webserv(const webserv &src);
	~webserv(void);
	webserv	&operator=(const webserv &src);

	/*___ utils ___*/

	void	check_conf_file(std::string file, webserv &srv);
	void	add_server(server to_add);

	/*___ exceptions ___*/

	class	badConfFile : public std::exception {
	public:
		virtual const char	*what() const throw();
	};

	class	emptyConfFile : public std::exception {
	public:
		virtual const char	*what() const throw();
	};

	class	badFileName : public std::exception {
	public:
		virtual const char	*what() const throw();
	};

	class	badInitialization : public std::exception {
	public:
		virtual const char	*what() const throw();
	};
};

#endif
