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
		if (fds_count < 0)
			throw std::runtime_error("poll failed");
		for (std::vector<struct pollfd>::iterator itr = pfds.begin(); itr != pfds.end(); itr++)
		{
			if (fds_count == fds_inc)
				break;
			if (itr->revents & (POLLIN | POLLHUP))
			{
				if (itr->revents & POLLHUP)
				{
					if (itr >= pfds.begin() && itr < pfds.end())
					{
						print_timestamped();
						std::cout << "User with descriptor " << itr->fd << \
						" disconnected" << std::endl;
						close(itr->fd);
						delete this->_users[itr->fd];
						this->_users.erase(itr->fd);
						clients_addr.erase(itr->fd);
						pfds.erase(itr);
					}
				}
				else if (itr->fd == pfds[0].fd)
				{

					/* check here for failure */
					tmp_pfd.fd = accept(pfds[0].fd, (sockaddr *)&client, &sizeof_client);
					fcntl(tmp_pfd.fd, F_SETFL, O_NONBLOCK);
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
						if (itr >= pfds.begin() && itr < pfds.end())
						{
							print_timestamped();
							std::cout << "User with descriptor " << itr->fd << \
							" disconnected" << std::endl;
							close(itr->fd);
							delete this->_users[itr->fd];
							this->_users.erase(itr->fd);
							clients_addr.erase(itr->fd);
							pfds.erase(itr);
						}
					}
				}
				fds_inc++;
			}
		}
	}
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
		this->_users[user_fd]->setIpaddress(obtain_ipaddress(user_addr));
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
		else if (cmd == "PRIVMSG")
			this->sendMsg(strings[i], this->_users[user_fd]);
	}
	return (1);
}
