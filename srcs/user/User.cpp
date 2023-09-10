#include "User.hpp"

User::User():_authenticated(false) {}

User::~User() {}

User::User(const User& instance)
{
	(void) instance;
}

User& User::operator=(const User& instance)
{
	(void) instance;
	return (*this);
}


std::string   User::getUsername() const
{
	return (this->_username);
}

std::string    User::getNickname() const
{
	return (this->_nickname);
}

std::string    User::getFullname() const
{
	return (this->_fullname);
}

std::string    User::getIpaddress() const
{
	return (this->_ipaddress);
}

std::string    User::getPassword() const
{
	return (this->_password);
}

bool            User::isAuthenticated() const
{
	return (this->_authenticated);
}

bool            User::isOperator() const
{
	return (this->_operator);
}

std::string    User::getMsgpartial() const
{
	return (this->_msg_partial);
}

int    User::getUserfd() const
{
	return (this->_user_fd);
}

void   User::setUsername(std::string username)
{
	this->_username = username;
}

void   User::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void    User::setFullname(std::string fullname)
{
	this->_fullname = fullname;
}

void    User::setIpaddress(std::string &ipaddress)
{
	this->_ipaddress = ipaddress;
	delete &ipaddress;
}

void    User::setPassword(std::string password)
{
	this->_password = password;
}

void    User::setAuthenticated(bool authenticated)
{
	this->_authenticated = authenticated;
}

void    User::setOperator(bool _operator)
{
	this->_operator = _operator;
}
void    User::setMsgpartial(std::string msgpartial)
{
	this->_msg_partial = msgpartial;
}

void    User::setUserfd(int user_fd)
{
	this->_user_fd = user_fd;
}
