/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliens <aliens@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:05:28 by ctirions          #+#    #+#             */
/*   Updated: 2023/01/10 17:05:37 by aliens           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "serverBlock.hpp"
# include "utils.hpp"

////////////////////
///   WEBSERV    ///
////////////////////

class config {
private:
	std::vector<size_t>			_ports;
	std::vector<serverBlock>	_servers;

public:

	/*___ canonical form ___*/

	config(void);
	config(const config &src);
	~config(void);
	config	&operator=(const config &src);

	/*___ getters ___*/

	std::vector<serverBlock>	getServers(void);
	std::vector<size_t>			getPorts(void);

	/*___ utils ___*/

	void	check_conf_file(std::string file, config &srv);
	void	add_server(serverBlock to_add);
	void	stack_ports(void);
	void	check_double(void);

	/*___ exceptions ___*/

	class	badConfFile : public std::exception {
		std::string	_msg;
	public:
		badConfFile(const std::string &msg) : _msg(std::string("Bad configuration file: ") + msg) {}
		~badConfFile(void) throw() {}
		virtual const char	*what() const throw() {
			return (_msg.c_str());
		}
	};

	struct	emptyConfFile : public std::exception { virtual const char	*what() const throw(); };
	struct	emptyServer : public std::exception { virtual const char	*what() const throw(); };
	struct	emptyValue : public std::exception { virtual const char	*what() const throw(); };
	struct	badFileName : public std::exception { virtual const char	*what() const throw(); };
	struct	badPort : public std::exception { virtual const char	*what() const throw(); };
	struct	badInitialization : public std::exception { virtual const char	*what() const throw(); };
};

#endif
