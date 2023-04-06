#ifndef _SERVER_H_
#define _SERVER_H_

#ifndef _STD_CPP_
#define _STD_CPP_
#include <bits/stdc++.h>
#endif

#include <optional>
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
    int send(std::string msg);
    std::optional<std::string> receive(void);
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
        std::string msg;
        std::optional<std::string> msg_optional = receive();
        if (not msg_optional.has_value()) {
            reset();
            goto start_running;
        }
        msg = msg_optional.value();

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
        
        if (send(msg)) {
            reset();
            goto start_running;
        }
    }
}

int Server::send(std::string msg) {
    std::size_t length = msg.size();
    char length_bytes[8] = {};
    size2chars(length, length_bytes);

    try {
        boost::asio::write(*_ptr_socket, boost::asio::buffer(length_bytes, 8));
    } catch (boost::wrapexcept<boost::system::system_error> e) {
        return -1;
    }

    try {
        boost::asio::write(*_ptr_socket, boost::asio::buffer(msg));
    } catch (boost::wrapexcept<boost::system::system_error> e) {
        return -1;
    }

    return 0;
}

std::optional<std::string> Server::receive(void) {
    std::string msg{};
    char length_bytes[8] = {};
    std::size_t length = 0;

    try {
        boost::asio::read(*_ptr_socket, boost::asio::buffer(length_bytes, 8));
    } catch (boost::wrapexcept<boost::system::system_error> e) {
        return std::nullopt;
    }

    length = chars2size(length_bytes);

    msg.resize(length);
    
    try {
        boost::asio::read(*_ptr_socket, boost::asio::buffer(msg, msg.size()));
    } catch (boost::wrapexcept<boost::system::system_error> e) {
        return std::nullopt;
    }

    return std::optional<std::string>(msg);
}

void Server::reset(void) {
    logInfo("[ Error ] Connection broken, reset the server...");
    _ptr_socket->close();
}

#endif