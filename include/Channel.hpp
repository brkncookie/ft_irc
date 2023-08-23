#pragma once

# include <iostream>
# include <string>
# include <vector>
# include "User.hpp"

class Channel
{
    private:
        std::string                         _name;
        std::string                         _topic;
        size_t                              _maximumCapacity;
        std::vector<User *>                 _users;
        bool                                _inviteOnly;

    public:
        Channel();
        ~Channel();
        Channel(const Channel& instance);
        Channel& operator=(const Channel& instance);

        std::string&                    getName() const;
        std::string&                    getTopic() const;
        size_t                          getMaximumCapacity() const;
        std::vector<User *>&            getUsers();
        bool                            isPrivate() const;
        bool                            isInviteOnly() const;

        void    setName(std::string name);
        void    setTopic(std::string topic);
        void    setMaximumCapacity(size_t capacity);
        void    setInviteOnly(bool inviteOnly);
};

std::ostream&	operator<<(std::ostream& outputStream, const Channel& channel);
