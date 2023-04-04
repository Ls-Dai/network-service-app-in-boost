#ifndef _SERVER_H_
#define _SERVER_H_

#ifndef _STD_CPP_
#define _STD_CPP_
#include <bits/stdc++.h>
#endif

#include <boost/asio.hpp>
#include "utils.h"


class Server {
private:
    boost::asio::io_context _io_context{};
    std::unique_ptr<boost::asio::ip::tcp::socket> _ptr_socket;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> _ptr_acceptor;
public:
    Server(void);
    void setPort(unsigned int port);
    void run(void);
    void reset(void);
};

Server::Server(void) {
    _ptr_socket.reset(new boost::asio::ip::tcp::socket(_io_context));
}

void Server::setPort(unsigned int port) {
    _ptr_acceptor.reset(new boost::asio::ip::tcp::acceptor(_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)));
}

void Server::run(void) {
start_running:
    logInfo("[ Info ] Waiting for an incoming client...");
    _ptr_acceptor->accept(*_ptr_socket);
    logInfo("[ Info ] Connection built.");

    keep_running {
        boost::system::error_code ec;
        std::string msg(1024, '\0');

        try {
            boost::asio::read(*_ptr_socket, boost::asio::buffer(msg, 1024));
        } catch (boost::wrapexcept<boost::system::system_error> e) {
            reset();
            goto start_running;
        }

        logInfo(msg);

        msg = "[ Info ] Message Received.";
        msg.resize(1024);

        try {
            boost::asio::write(*_ptr_socket, boost::asio::buffer(msg, 1024));
        } catch (boost::wrapexcept<boost::system::system_error> e) {
            reset();
            goto start_running;
        }

        msg = msg = input(">> ", "");

        msg = "<< Server: " + msg;
        msg.resize(1024);

        try {
            boost::asio::write(*_ptr_socket, boost::asio::buffer(msg, 1024));
        } catch (boost::wrapexcept<boost::system::system_error> e) {
            reset();
            goto start_running;
        }
    }
}

void Server::reset(void) {
    logInfo("[ Error ] Connection broken, reset the server...");
    _ptr_socket->close();
}

#endif