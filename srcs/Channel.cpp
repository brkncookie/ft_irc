#include "../include/Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const Channel& instance)
{
	(void) instance;
}

Channel& Channel::operator=(const Channel& instance)
{
	(void) instance;
	return (*this);
}


std::string                   Channel::getName() const
{
	return (this->_name);
}

std::string                    Channel::getTopic() const
{
	return (this->_topic);
}
size_t                          Channel::getMaximumCapacity() const
{
	return (this->_maximumCapacity);
}

std::vector<User *>&            Channel::getUsers()
{
	return (this->_users);
}

bool                            Channel::isPrivate() const
{
	return (this->_private);
}

bool                            Channel::isInviteOnly() const
{
	return (this->_inviteOnly);
}


void    Channel::setName(std::string name)
{
	this->_name = name;
}

void    Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void    Channel::setMaximumCapacity(size_t capacity)
{
	this->_maximumCapacity = capacity;
}

void    Channel::setInviteOnly(bool inviteOnly)
{
	this->_inviteOnly = inviteOnly;
}

void    Channel::setPrivate(bool _private)
{
	this->_private = _private;
}
