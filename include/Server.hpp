#pragma once

# include <string>
# include <sstream>
# include <vector>
# include <map>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <vector>
#include <poll.h>
#include <iostream>
# include "Channel.hpp"
# include "User.hpp"

class Server
{
	private:
		const size_t			_port;
		const std::string		_password;
		const std::string		_name;
		std::map<int, User *>		_users;
		std::vector<Channel *>	_channels;
		int	initListener();
		void	registerUser(std::string	&cmd, User	*user);
		int	handleUser(int user_fd);
		std::vector<std::string> *parseIRCmd(User *user);
	public:
		~Server();
		Server(const size_t port, const std::string password);
		Server(const Server& instance);
		Server& operator=(const Server& instance);
		std::string			getName() const;
		std::string			getPassword() const;
		size_t				getPort() const;
		std::map<int, User *>&		getUsers();
		std::vector<Channel *>&	getChannels();

		void	startServer();
};
