#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <iostream>


int init_listener(void)
{
	struct	addrinfo hint, *ai;	
	int		listener;
	int		opt = 1;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "6667", &hint, &ai);

	listener = socket(hint.ai_family, hint.ai_socktype, hint.ai_protocol);

	fcntl(listener, F_SETFL, O_NONBLOCK);
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	bind(listener, ai->ai_addr, ai->ai_addrlen);

	listen(listener, SOMAXCONN);

	freeaddrinfo(ai);

	return(listener);
}


int	handle_client(int	client_sock)
{
	char	buff[512];
	memset(buff, 0, sizeof(buff));
	if (recv(client_sock, buff, sizeof(buff), 0) <= 0)
		return (0);
	else
		std::cout << "client " << client_sock << " sent: " << buff << std::endl;
	return 1;
}


void	establish_connection(void)
{
	fd_set	main_fds;
	fd_set	read_fds;
	int		max_fd;
	int		listener;
	int		client;

	listener = init_listener();
	FD_ZERO(&main_fds);
	FD_ZERO(&read_fds);

	FD_SET(listener, &main_fds);

	max_fd = listener + 1;
	while (1)
	{
		read_fds = main_fds;
		select(max_fd, &read_fds, NULL, NULL, NULL);
		for (int inx = 3; inx < max_fd; inx++)
		{
			if (FD_ISSET(inx, &read_fds))
			{
				if (inx == listener)
				{
					client = accept(listener, NULL, NULL);
					FD_SET(client, &main_fds);
					if (client > max_fd - 1)
						max_fd = client + 1;
				}
				else
				{
					if (!handle_client(inx))
					{
						std::cout << "client " << inx << " dropped the connection" << std::endl;
						close(inx);
						FD_CLR(inx, &main_fds);
					}
				}
			}
		}
	}
}

int main(void)
{
	establish_connection();
	return (0);
}
