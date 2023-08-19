/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 02:34:30 by mbouthai          #+#    #+#             */
/*   Updated: 2023/07/26 03:18:31 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>
# include <vector>
# include "Channel.hpp"
# include "User.hpp"

class Server
{
	private:
		const std::string		_name;
		const std::string		_password;
		const size_t            _port;
		std::vector<User *>		_users;
		std::vector<Channel *>	_channels;
	public:
		Server();
		~Server();
		Server(const Server& instance);
		Server& operator=(const Server& instance);

		const std::string&			getName() const;
		const std::string&			getPassword() const;
		const size_t					getPort() const;
		
		std::vector<User *>&		getUsers();
		std::vector<Channel *>&	getChannels();

		void	establishConnection();
		void	configure();
		void	bindSocket();
		void	acceptClient();
};

std::ostream&	operator<<(std::ostream& outputStream, const Server& server);