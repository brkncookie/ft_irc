#pragma once

# include <string>
# include <vector>
# include <map>
# include "Channel.hpp"
# include "User.hpp"

class Server
{
	private:
		const std::string		_name;
		const std::string		_password;
		const size_t            _port;
		std::map<int, User *>		_users;
		std::vector<Channel *>	_channels;
		int	initListener();
		void	registerUser(std::string	&cmd, User	*user);
		void	handleUser(int user_fd);
	public:
		~Server();
		Server(size_t port, const std::string password);
		Server(const Server& instance);
		Server& operator=(const Server& instance);

		const std::string&			getName() const;
		const std::string&			getPassword() const;
		const size_t					getPort() const;
		std::map<int, User *>&		getUsers();
		std::vector<Channel *>&	getChannels();

		void	startServer();
};
