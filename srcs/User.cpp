#include "../include/User.hpp"

User::User():_authenticated(false) {}

User::~User() {}

User::User(const User& instance) {}

User& User::operator=(const User& instance) {}


std::string&    User::getUsername() const
{
	return (this->_username);
}

std::string&    User::getNickname() const
{
	return (this->_nickname);
}

std::string&    User::getFullname() const
{
	return (this->_fullname);
}

std::string&    User::getHostname() const
{
	return (this->_hostname);
}

std::string&    User::getPassword() const
{
	return (this->_password);
}

bool            User::isAuthenticated() const
{
	return (this->_authenticated)
}

bool            User::isOperator() const
{
	return (this->_operator);
}

std::string&    getMsgpartial() const
{
	return (this->_msg_partial);
}

int    getUserfd() const;
{
	return (this->_user_fd);
}

void   User::setUsername(std::string username)
{
	_username = username;
}

void   User::setNickname(std::string nickname)
{
	_nickname = nickname;
}

void    User::setFullname(std::string fullname)
{
	_fullname = fullname;
}

void    User::setHostname(std::string hostname)
{
	_hostname = hostname;
}

void    User::setPassword(std::string password)
{
	_password = password;
}

void    User::setAuthenticated(bool authenticated)
{
	_authenticated = authenticated;
}

void    User::setOperator(bool boolean)
{
	_operator = operator;
}
void    setMsgpartial(std::string msgpartial)
{
	_msg_partial = msgpartial;
}

void    setUserfd(int user_fd);
{
	_user_fd = user_fd;
}
