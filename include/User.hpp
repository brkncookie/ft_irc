#pragma once

# include <iostream>
# include <string>

class User
{
    private:
        std::string     _username;
        std::string     _fullname;
        std::string     _ipaddress;
        std::string     _nickname;
        std::string     _password;
	std::string	_msg_partial;
	int		_user_fd;
        bool            _authenticated;
        bool            _operator;

    public:
        User();
        ~User();
        User(const User& instance);
        User& operator=(const User& instance);

        std::string    getUsername() const;
        std::string    getNickname() const;
        std::string    getFullname() const;
        std::string    getIpaddress() const;
        std::string    getPassword() const;
        std::string    getMsgpartial() const;
        int		getUserfd() const;
        bool            isAuthenticated() const;
        bool            isOperator() const;

        void    setUsername(std::string username);
        void    setNickname(std::string nickname);
        void    setFullname(std::string fullname);
        void    setIpaddress(std::string &ipaddress);
        void    setPassword(std::string password);
        void    setMsgpartial(std::string msgpartial);
        void    setUserfd(int user_fd);
        void    setAuthenticated(bool authenticated);
        void    setOperator(bool boolean);
};
