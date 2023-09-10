#include "Server.hpp"

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
			reply = std::string(":").append(this->getName()) + \
			std::string(" 462 ").append(user->getNickname()) + \
			std::string(" :Unauthorized command (already registered)") + std::string("\r\n");
			send(user->getUserfd(), reply.c_str(), reply.size(), 0);
			return ;
		}
		if (this->getPassword() != cmd)
		{
			reply = std::string(":").append(this->getName()) + \
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
		std::cout << "			Nickname: " << user->getNickname() << ", Username: " << user->getUsername() \
		<< ", Realname: " << user->getFullname() <<  ", Ipaddress: " << user->getIpaddress() << std::endl;
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

void	Server::sendMsg(std::string &cmd, User *user)
{
	std::stringstream strm;
	std::string	reply;
	std::map<int, User*>::iterator itr;

	strm.str(cmd);
	strm >> cmd;
	strm >> cmd;

	/* first of all differntiate between the target being a channel or a user, but for now we only gonna handle users */
	for(itr = this->_users.begin(); itr != this->_users.end(); itr++)
		if (itr->second->getNickname() == cmd)
		{
			reply = std::string(":").append(user->getNickname()) + std::string(" ") + strm.str() +std::string("\r\n");
			send(itr->second->getUserfd(), reply.c_str(), reply.size(), 0);
			return ;
		}
	if (itr == this->_users.end())
	{
		reply = std::string(":").append(this->getName()) + std::string(" 401 ") + cmd \
		+ std::string(" :No such nick/channel") + std::string("\r\n");
		send(user->getUserfd(), reply.c_str(), reply.size(), 0);
		return ;
	}
}

void	Server::joinChannel(std::string &cmd, User *user)
{
	std::string		target;
	std::string		password;
	std::stringstream	targets;
	std::stringstream	passwords;
	bool			create;
	std::vector<Channel *>::iterator itr;
	std::string		reply;

	std::cout << "cmd => " << cmd << std::endl;

	passwords.str(cmd);
	if (cmd[0] == ':')
		passwords >> password;
	passwords >> password;
	passwords >> password;
	passwords >> password;
	if (!passwords.fail())
	{
		passwords.str(password);
	}
	else
	{
		passwords.str("");
		password.assign("");
		passwords.clear();
	}

	targets.str(cmd);
	if (cmd[0] == ':')
		targets >> target;
	targets >> target;
	targets >> target;
	targets.str(target);


	std::cout << "targets are: " << targets.str() << std::endl;
	std::cout << "passowrds are: " << passwords.str() << std::endl;
	while(std::getline(targets, target, ',') && !target.empty())
	{
		std::cout << "first target is: " <<  target << std::endl;
		create = true;
		for (size_t inx = 1; inx < target.size(); inx++)
			target[inx] = std::tolower(target[inx]);
		for (itr = this->_channels.begin(); itr != this->_channels.end(); itr++)
			if ((*itr)->getName() == target)
			{
				create = false;
				break;
			}
		if (!create)
		{
			std::getline(passwords, password, ',');
			if (!password.empty() && (*itr)->getPassword() != password)
			{
				/* send a reply indicating that the password for the channel is incorrect */
				reply = std::string(":").append(this->getName()) + std::string(" 475 ") + target \
				+ std::string(" :Cannot join channel (+k)") + std::string("\r\n");
				send(user->getUserfd(), reply.c_str(), reply.size(), 0);
				break;
			}
			if ((*itr)->getUsers().size() + 1 > (*itr)->getMaximumCapacity())
			{
				/* send a reply indicating that the channel reached its maximum capacity */
				reply = std::string(":").append(this->getName()) + std::string(" 471 ") + target \
				+ std::string(" :Cannot join channel (+l)") + std::string("\r\n");
				send(user->getUserfd(), reply.c_str(), reply.size(), 0);
				break;
			}
			if ((*itr)->isInviteOnly())
			{
				/* send a reply indicating that the channel is invite only */
				reply = std::string(":").append(this->getName()) + std::string(" 473 ") + target \
				+ std::string(" :Cannot join channel (+i)") + std::string("\r\n");
				send(user->getUserfd(), reply.c_str(), reply.size(), 0);
				break;
			}
			(*itr)->addUser(user);
			/* broadcast the join command to all users in the channel */
			reply = std::string(":").append(user->getNickname()) + "!" + user->getUsername() + "@" + \
			user->getIpaddress() + " JOIN " + target + std::string("\r\n");
			(*itr)->announce(reply);
			/* send the channel topic to the user */
			if(!(*itr)->getTopic().empty())
			{
				reply = std::string(":").append(this->getName()) + std::string(" 332 ") + target \
				+ std::string(" :") + (*itr)->getTopic() + std::string("\r\n");
				send(user->getUserfd(), reply.c_str(), reply.size(), 0);
			}
			/* send a list of users that are in the channel */
			reply = std::string(":").append(this->getName()) + std::string(" 353 ") + user->getNickname() + " = "\
			+ target + std::string(" :") + (*itr)->getUserNicknames() + std::string("\r\n");
			send(user->getUserfd(), reply.c_str(), reply.size(), 0);
			reply = std::string(":").append(this->getName()) + std::string(" 366 ") + user->getNickname() + " "\
			+ target + std::string(" :End of /NAMES list.") + std::string("\r\n");
			send(user->getUserfd(), reply.c_str(), reply.size(), 0);

		}
		else
		{
			Channel	*channel = new Channel();
			channel->setName(target);
			std::getline(passwords, password, ',');
			if (!password.empty())
				channel->setPassword(password);
			/* not sure yet of the default maximum capacity of a channel */
			channel->setMaximumCapacity(1337);
			channel->addUser(user, true);
			/* send the appropriate replies when a user succesufully create and joins */
			this->_channels.push_back(channel);
			/* broadcast the join command to all users in the channel */
			reply = std::string(":").append(user->getNickname()) + "!" + user->getUsername() + "@" + \
			user->getIpaddress() + " JOIN " + target + std::string("\r\n");
			channel->announce(reply);
			/* send a list of users that are in the channel */
			reply = std::string(":").append(this->getName()) + std::string(" 353 ") + user->getNickname() + " = "\
			+ target + std::string(" :") + channel->getUserNicknames() + std::string("\r\n");
			send(user->getUserfd(), reply.c_str(), reply.size(), 0);
			reply = std::string(":").append(this->getName()) + std::string(" 366 ") + user->getNickname() + " "\
			+ target + std::string(" :End of /NAMES list.") + std::string("\r\n");
			send(user->getUserfd(), reply.c_str(), reply.size(), 0);
		}
	}
}
