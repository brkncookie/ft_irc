#include "../include/Channel.hpp"

Channel::Channel():_inviteOnly(false), _topicOpr(false) {}

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

std::vector<User *>&            Channel::getChanop()
{
	return (this->_chanop);
}


bool                            Channel::isTopicOpr() const
{
	return (this->_topicOpr);
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

void    Channel::setPassword(std::string password)
{
	this->_password = password;
}

void    Channel::setMaximumCapacity(size_t capacity)
{
	this->_maximumCapacity = capacity;
}

void    Channel::setInviteOnly(bool inviteOnly)
{
	this->_inviteOnly = inviteOnly;
}

void    Channel::setTopicOpr(bool topicOpr)
{
	this->_topicOpr = topicOpr;
}
