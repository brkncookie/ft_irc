#include "../include/Server.hpp"
#include <sys/socket.h>
#include <poll.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>

Server::~Server() {}

Server::Server(const size_t port, const std::string password):_port(port), _password(password), _name("ircserv.1337.ma") {}

Server::Server(const Server& instance): _port(instance.getPort()), _password(instance.getPassword()), _name(instance.getName())
{}

std::string    Server::getName() const
{
    return (this->_name);
}

std::string    Server::getPassword() const
{
    return (this->_password);
}

size_t		Server::getPort() const
{
    return (this->_port);
}

std::map<int, User *>&		Server::getUsers()
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

	/* check here for failure */
	listener = socket(AF_INET, SOCK_STREAM, 0);

	fcntl(listener, F_SETFL, O_NONBLOCK);
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_port  = htons(this->_port);
	address.sin_addr.s_addr = INADDR_ANY;

	/* check here for failure */
	bind(listener, (sockaddr *)&address, sizeof(address));
	/* check here for failure */
	listen(listener, SOMAXCONN);

	return(listener);
}

void	Server::startServer(void)
{
	std::vector<struct pollfd> pfds;
	std::map<int, sockaddr_in> clients_addr;
	sockaddr_in	client;
	socklen_t	sizeof_client = sizeof(client);
	struct	pollfd	tmp_pfd;
	int	fds_count;

	/* check here for failure */
	tmp_pfd.fd = this->initListener();
	tmp_pfd.events = POLLIN;

	pfds.push_back(tmp_pfd);

	while (1)
	{
		int fds_inc = 0;
		/* check here for failure */
		fds_count = poll(&pfds[0], pfds.size(), -1);

		for (std::vector<struct pollfd>::iterator itr = pfds.begin(); itr != pfds.end(); itr++)
		{
			if (fds_count == fds_inc)
				break;
			if (itr->revents & (POLLIN | POLLHUP))
			{
				if (itr->revents & POLLHUP)
				{
					print_timestamped();
					std::cout << "User with descriptor " << itr->fd << " disconnected" << std::endl;
					close(itr->fd);
					delete this->_users[itr->fd];
					this->_users.erase(itr->fd);
					clients_addr.erase(itr->fd);
					pfds.erase(itr);
				}
				else if (itr->fd == pfds[0].fd)
				{

					/* check here for failure */
					tmp_pfd.fd = accept(pfds[0].fd, (sockaddr *)&client, &sizeof_client);
					clients_addr[tmp_pfd.fd] = client;
					pfds.push_back(tmp_pfd);
					print_timestamped();
					std::cout << "User with descriptor " << tmp_pfd.fd << " initiated a connection" << std::endl;
				}
				else
				{
					if (!this->handleUser(itr->fd, clients_addr[itr->fd]))
					{
						/* for compatibility with linux as POLLHUP doesn't work for linux */
						print_timestamped();
						std::cout << "User with descriptor " << itr->fd << " disconnected" << std::endl;
						close(itr->fd);
						delete this->_users[itr->fd];
						this->_users.erase(itr->fd);
						clients_addr.erase(itr->fd);
						pfds.erase(itr);
					}
				}
				fds_inc++;
			}
		}
	}
}

std::vector<std::string> *Server::parseIRCmd(User *user)
{
	char	buff[1024];
	std::string	str;
	std::stringstream	strm;
	std::vector<std::string>	*vec_of_strings = new std::vector<std::string>;
	int	first_time = 1;

	bzero(buff, 1024);
	if (recv(user->getUserfd(), buff, 1024, 0) < 1)
	{
		delete vec_of_strings;
		return (NULL);
	}
	strm.str(std::string(buff));
	while(std::getline(strm, str) && !str.empty())
	{
		if (first_time && !user->getMsgpartial().empty())
		{
			str = user->getMsgpartial() + str;
			user->setMsgpartial(std::string(""));
			first_time = 0;
		}
		if (str[str.size() - 1] != '\r')
		{
			user->setMsgpartial(str);
			break;
		}
		vec_of_strings->push_back((str.erase(str.size() - 1, 1)));
	}
	return(vec_of_strings);
}

int	Server::handleUser(int user_fd, sockaddr_in user_addr)
{
	std::vector<std::string> *vec_of_strings;
	std::string	*strings;
	std::string	cmd;
	std::stringstream	strm;
	std::map<int, User *>::iterator itr;

	itr = this->_users.find(user_fd);

	if (itr == this->_users.end())
	{
		this->_users[user_fd] = new User();
		this->_users[user_fd]->setUserfd(user_fd);
		this->_users[user_fd]->setHostname(obtain_hostname(user_addr));
	}
	if (!(vec_of_strings = parseIRCmd(this->_users[user_fd])))
		return (0);
	strings = &(*vec_of_strings)[0];
	for (unsigned int i = 0; i < vec_of_strings->size(); i++)
	{
		/* loop through each line and identify the CMD in it to call the appropriate function for it */
		strm.str(strings[i]);
		strm >> cmd;
		if (cmd[0] == ':')
			strm >> cmd;
		if (cmd == "NICK" || cmd == "PASS" || cmd == "USER")
			this->registerUser(strings[i], this->_users[user_fd]);
		else if (!(cmd == "NICK" || cmd == "PASS" || cmd == "USER") && 
				!this->_users[user_fd]->isAuthenticated())
		{
			std::string reply = std::string(":").append(this->getName()) + \
			std::string(" 451 ").append(this->_users[user_fd]->getNickname()) \
			+ std::string(" :You have not registered") + std::string("\r\n");
			send(this->_users[user_fd]->getUserfd(), reply.c_str(), reply.size(), 0);
		}
	}
	return (1);
}

void	Server::registerUser(std::string &cmd, User *user)
{
	std::stringstream	strm;
	std::string reply;

	strm.str(cmd);
	strm >> cmd;
	if (cmd[0] == ':')
		strm >> cmd;
	if (cmd == std::string("NICK"))
	{
		/* first check if the given nickname already exists in our database, aka not a unique nickname */

		/* if the nickname is unique then set it or change it (and declare the change)
		   depending on whether the user is a new user or alr an old one */
		strm >> cmd;
		for(std::map<int, User*>::iterator itr = this->_users.begin(); itr != this->_users.end(); itr++)
			if (itr->second->getNickname() == cmd)
			{
				reply = std::string(":").append(this->getName()) + std::string(" 433 ") + cmd \
				+ std::string(" :Nickname is already in use.") + std::string("\r\n");
				send(user->getUserfd(), reply.c_str(), reply.size(), 0);
				return ;
			}
		user->setNickname(cmd);
		if (user->isAuthenticated())
		{
			reply = std::string(":").append(this->getName()) + std::string(" 001 ") + \
			std::string(user->getNickname()) + std::string(" :Welcome to the IRC Network, ") \
			+ std::string(user->getNickname()) + std::string("\r\n");
			send(user->getUserfd(), reply.c_str(), reply.size(), 0);
			/* either the server or the user afterwards notifies the 
			 * channel/memeber_of_a_private_conversation of the nickname change */
		}
	}
	else if (cmd == std::string("USER"))
	{
		if (user->isAuthenticated())
		{
			std::string reply = std::string(":").append(this->getName()) + \
			std::string(" 462 ").append(user->getNickname()) + \
			std::string(" :Unauthorized command (already registered)") + std::string("\r\n");
			send(user->getUserfd(), reply.c_str(), reply.size(), 0);
			return ;
		}
		strm >> cmd;
		user->setUsername(cmd);
		strm >> cmd;
		strm >> cmd;
		user->setFullname(std::string(strm.str().c_str() + strm.tellg() + 1));
		/* set the parameters in this command accordingly only if \
		 * the user is not yet authenticated, otherwise yell a 462 error */
	}
	else if (cmd == std::string("PASS"))
	{
		/* first check if the user is not authenticated, otherwise yell a 462 error */
		/* check if the password is correct before setting it*/
		strm >> cmd;
		if (user->isAuthenticated())
		{
			std::string reply = std::string(":").append(this->getName()) + \
			std::string(" 462 ").append(user->getNickname()) + \
			std::string(" :Unauthorized command (already registered)") + std::string("\r\n");
			send(user->getUserfd(), reply.c_str(), reply.size(), 0);
			return ;
		}
		if (this->getPassword() != cmd)
		{
			std::string reply = std::string(":").append(this->getName()) + \
			std::string(" 464 :Password incorrect") + std::string("\r\n");
			send(user->getUserfd(), reply.c_str(), reply.size(), 0);
			return ;
		}
		user->setPassword(cmd);
	}

	if (!user->getUsername().empty() && !user->getNickname().empty() && \
			!user->getPassword().empty() && !user->isAuthenticated())
	{
		/* set the user to authenticated and send replies indicating the succesful \
		 * authentication of the user */

		print_timestamped();
		std::cout << "User with descriptor " << user->getUserfd() \
		<< " succesfully authenticated and these are their attributes:" << std::endl;
		std::cout << "Nickname: " << user->getNickname() << std::endl;
		std::cout << "Username: " << user->getUsername() << std::endl;
		std::cout << "Realname: " << user->getFullname() << std::endl;
		std::cout << "Hostname: " << user->getHostname() << std::endl;
		user->setAuthenticated(true);

		reply = std::string(":").append(this->getName()) + std::string(" 001 ") + \
		std::string(user->getNickname()) + std::string(" :Welcome to the IRC Network, ") \
		+ std::string(user->getNickname()) + std::string("\r\n");
		send(user->getUserfd(), reply.c_str(), reply.size(), 0);

		reply = std::string(":").append(this->getName()) + std::string(" 002 ") + \
		std::string(user->getNickname()) + std::string(" :Your host is ") + \
		std::string(this->getName()) + std::string(", running version 1.9") + std::string("\r\n");
		send(user->getUserfd(), reply.c_str(), reply.size(), 0);

		reply = std::string(":").append(this->getName()) + std::string(" 003 ") \
		+ std::string(user->getNickname()) + std::string(" :This server was created 12 12 2012") \
		+ std::string("\r\n");
		send(user->getUserfd(), reply.c_str(), reply.size(), 0);

		reply = std::string(":").append(this->getName()) + std::string(" 004 ") + \
		std::string(user->getNickname()) + std::string(" ").append(this->getName()) \
		+ std::string(" 1.9 itkol") + std::string("\r\n");
		send(user->getUserfd(), reply.c_str(), reply.size(), 0);
	}
}
