/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctirions <ctirions@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:05:28 by ctirions          #+#    #+#             */
/*   Updated: 2022/11/10 12:05:06 by ctirions         ###   ########.fr       */
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
	std::vector<size_t>			_ports;

public:

	/*___ canonical form ___*/

	webserv(void);
	webserv(const webserv &src);
	~webserv(void);
	webserv	&operator=(const webserv &src);

	/*___ utils ___*/

	void	check_conf_file(std::string file, webserv &srv);
	void	add_server(server to_add);
	void	stack_ports(void);

	/*___ exceptions ___*/

	struct	badConfFile : public std::exception { virtual const char	*what() const throw(); };
	struct	emptyConfFile : public std::exception { virtual const char	*what() const throw(); };
	struct	badFileName : public std::exception { virtual const char	*what() const throw(); };
	struct	badInitialization : public std::exception { virtual const char	*what() const throw(); };
};

#endif
