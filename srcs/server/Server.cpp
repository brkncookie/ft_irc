#include "Server.hpp"
#include <sys/socket.h>
#include <poll.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>

Server::~Server() {}

Server::Server(size_t port, const std::string password):_port(port), _password(password) {}

Server::Server(const Server& instance)
    : _name(instance.getName()),
    _password(instance.getPassword()),
    _port(instance.getPort())
{}

const std::string&    Server::getName() const
{
    return (this->_name);
}

const std::string&    Server::getPassword() const
{
    return (this->_password);
}

const size_t  Server::getPort() const
{
    return (this->_port);
}

std::vector<User *>&  Server::getUsers()
{
    return (this->_users);
}

std::vector<Channel *>&    Server::getChannels()
{
    return (this->_channels);
}

int	Server::initListener(void)
{
	sockaddr_in	address;
	int		listener;
	int		opt = 1;

	listener = socket(AF_INET, SOCK_STREAM, 0);

	fcntl(listener, F_SETFL, O_NONBLOCK);
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_port  = htons(this->_port);
	address.sin_addr.s_addr = INADDR_ANY;

	bind(listener, (sockaddr *)&address, sizeof(address));
	listen(listener, SOMAXCONN);

	return(listener);
}

void	Server::startServer(void)
{
	std::vector<struct pollfd> pfds;
	struct	pollfd	tmp_pfd;
	int	fds_count;

	tmp_pfd.fd = this->initListener();
	tmp_pfd.events = POLLIN;

	pfds.push_back(tmp_pfd);

	while (1)
	{
		int fds_inc = 0;
		fds_count = poll(&pfds[0], pfds.size(), -1);

		for (int inx = 0; inx < pfds.size(); inx++)
		{
			if (fds_count == fds_inc)
				break;
			if (pfds[inx].revents & POLLIN)
			{
				if (pfds[inx].fd == pfds[0].fd)
				{
					tmp_pfd.fd = accept(pfds[0].fd, NULL, NULL);
					pfds.push_back(tmp_pfd)
				}
				else
				{
					if (!this->handleUser(pfds[inx].fd))
					{
						/* Announce that the user dropped the connection */
						close(pfds[inx].fd);
						delete this->_users[pfds[inx].fd];
						this->_users.erase[pfds[inx].fd];
						pfds.erase(pfds.begin() + inx);
					}
				}
				fds_inc++;
			}
		}
	}
}

int	Server::handleUser(int user_fd)
{
	std::string	*strings;
	std::string	cmd;
	std::stringstream	strm;
	std::map<int, User *>::iterator itr;

	itr = this->_users.find(user_fd);

	if (itr == this->_users.end())
	{
		this->_users[user_fd] = new User();
		this->_users[user_fd]->setUserfd(user_fd);
	}

	strings = parseIRCmd(this->_users[user_fd]);

	for (int i = 0; strings[i]; i++)
	{
		/* loop through each line and identify the CMD in it to call the appropriate function for it */
		strm.str(strings[i]);
		strm >> cmd;
		if (cmd[0] == ':')
			strm >> cmd;
		if (cmd == "NICK" || cmd == "PASS" || cmd == "USER")
			this->registerUser(strings[i], this->_users[user_fd]);
		else if (!(cmd == "NICK" || cmd == "PASS" || cmd == "USER") && !this->_users[user_fd]->isAuthenticated())
		{
			std::string reply = std::string(":").append(this->getName()) + \
			std::string(" 451 :You have not registered") + std::string("\r\n");
			send(this->_users[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);
		}
	}
	return (1);
}

void	Server::registerUser(std::string &cmd, User *user)
{
	std::stringstream	strm;
	std::string reply;

	strm.str(cmd)
	strm >> cmd;
	if (cmd[0] == ':')
		strm >> cmd;
	if (cmd == std::string("NICK"))
	{
		/* first check if the given nickname already exists in our database, aka not a unique nickname */

		/* if the nickname is unique then set it or change it (and declare the change)
		   depending on whether the user is a new user or alr an old one */
		strm >> cmd;
		for(std::map<int, User*> itr = this->_users.begin(); itr != this->_users.end(); itr++)
			if (itr->getNickname() == cmd)
			{
				reply = std::string(":").append(this->getName()) + std::string(" 443 ") + cmd \
				+ std::string(" :Nickname is already in use.") + std::string("\r\n");
				send(user[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);
				return ;
			}
		user->setNickname(cmd);
		if (user->isAuthenticated())
		{
			reply = std::string(":").append(this->getName()) + std::string(" 001 ") + std::string(user->getNickname()) \
			+ std::string(" :Welcome to the IRC Network, ") + std::string(user->getNickname()) + std::string("\r\n");
			send(user[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);
			/* user afterwards notifies the channel/memeber_of_a_private_conversation of the nickname change */
		}
	}
	else if (cmd == std::string("USER"))
	{
		if (user->isAuthenticated())
		{
			std::string reply = std::string(":").append(this->getName()) + \
			std::string(" 462 :Unauthorized command (already registered)") + std::string("\r\n");
			send(user[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);
			return ;
		}
		strm >> cmd;
		user->setUsername(cmd);
		strm >> cmd;
		user->setHostname(cmd);
		strm >> cmd;
		user->setFullname(std::string(strm.str().c_str() + strm.tellg + 1));
		/* set the parameters in this command accordingly only if the user is not yet authenticated, otherwise yell a 462 error */
	}
	else if (cmd == std::string("PASS"))
	{
		/* first check if the user is not authenticated, otherwise yell a 462 error */
		/* check if the password is correct before setting it*/
		strm >> cmd;
		if (user->isAuthenticated())
		{
			std::string reply = std::string(":").append(this->getName()) + \
			std::string(" 462 :Unauthorized command (already registered)") + std::string("\r\n");
			send(user[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);
			return ;
		}
		if (this->getPassword() != cmd)
		{
			std::string reply = std::string(":").append(this->getName) + \
			std::string(" 464 :Password incorrect") + std::string("\r\n");
			send(user[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);
			return ;
		}
		user->setPassoword(cmd);
	}

	if (!user->getUsername().empty() && !user->getNickname().empty() && !user->getPassword().empty() && !user->isAuthenticated())
	{
		/* set the user to authenticated and send replies indicating the succesful authentication of the user */
		user->setAuthenticated(true);

		reply = std::string(":").append(this->getName()) + std::string(" 001 ") + std::string(user->getNickname()) \
		+ std::string(" :Welcome to the IRC Network, ") + std::string(user->getNickname()) + std::string("\r\n");
		send(user[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);

		reply = std::string(":").append(this->getName()) + std::string(" 002 ") + std::string(user->getNickname()) \
		+ std::string(" :Your host is ") + std::string(this->getName()) + std::string(", running version 1.9") + std::string("\r\n");
		send(user[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);

		reply = std::string(":").append(this->getName()) + std::string(" 003 ") + std::string(user->getNickname()) \
		+ std::string(" :This server was created 12 12 2012") + std::string("\r\n");
		send(user[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);

		reply = std::string(":").append(this->getName()) + std::string(" 004 ") + std::string(user->getNickname()) +\
		std::string(" ").append(this->getName()) + std::string("  1.9 itkol") + std::string("\r\n");
		send(user[user_fd]->getUserfd, reply.c_str(), reply.size(), 0);
	}
}
