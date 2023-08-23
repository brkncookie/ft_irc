#include "../include/Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const Channel& instance) {}

Channel& Channel::operator=(const Channel& instance) {}


std::string&                    Channel::getName() const {}

std::string&                    Channel::getTopic() const {}

size_t                          Channel::getMaximumCapacity() const {}

std::vector<User *>&            Channel::getUsers() {}

bool                            Channel::isPrivate() const {}

bool                            Channel::isInviteOnly() const {}


void    Channel::setName(std::string name) {}

void    Channel::setTopic(std::string topic) {}

void    Channel::setMaximumCapacity(size_t capacity) {}

void    Channel::setInviteOnly(bool inviteOnly) {}

std::ostream&	operator<<(std::ostream& outputStream, const Channel& channel) {}
