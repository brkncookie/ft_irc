#pragma once

# include <iostream>
# include <string>
# include <vector>
#include <sys/socket.h>
# include "User.hpp"

class Channel
{
    private:
        std::string	_name;
        std::string	_topic;
        std::string	_password;
        size_t		_maximumCapacity;
        std::vector<User *>	_users;
        std::vector<User *>	_chanop;
        bool			_inviteOnly;
	bool			_topicOpr;

    public:
        Channel();
        ~Channel();
        Channel(const Channel& instance);
        Channel& operator=(const Channel& instance);

        std::string                    getName() const;
        std::string                    getTopic() const;
        std::string                    getPassword() const;
        size_t                          getMaximumCapacity() const;
        std::vector<User *>&            getUsers();
        std::vector<User *>&            getChanop();
        bool                            isInviteOnly() const;
        bool                            isTopicOpr() const;

        void    setName(std::string name);
        void    setTopic(std::string topic);
        void    setPassword(std::string password);
        void    setMaximumCapacity(size_t capacity);
        void    setInviteOnly(bool inviteOnly);
	void    setTopicOpr(bool topicOpr);
	void	distributeMsg(std::string msg);
	std::string	getNicknames(void);
};

std::ostream&	operator<<(std::ostream& outputStream, const Channel& channel);
