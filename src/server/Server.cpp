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
					if (this->registerUser(tmp_pfd.fd))
						pfds.push_back(tmp_pfd)
					else
					{
						/* Announce that the user failed the registration */
						close(tmp_pfd.fd);
					}

				}
				else
				{
					if (!this->handleUser(pfds[inx].fd))
					{
						/* Announce that the user dropped the connection */
						close(pfds[inx].fd);
						pfds[inx].fd = -1;
					}
				}
				fds_inc++;
			}
		}
	}
}
