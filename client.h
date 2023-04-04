#ifndef _CLIENT_H_
#define _CLIENT_H_

#ifndef _STD_CPP_
#define _STD_CPP_
#include <bits/stdc++.h>
#endif

#include <boost/asio.hpp>
#include "utils.h"


class Client {
private:
    boost::asio::io_context _io_context{};
    std::unique_ptr<boost::asio::ip::tcp::socket> _ptr_socket{};
public:
    Client(void);
    void connectByInput(bool isFirstConnect=true);
    void connect(std::string const& host, unsigned int port);
    void run();
};

Client::Client(void) {
    _ptr_socket.reset(new boost::asio::ip::tcp::socket(_io_context));
}

void Client::connect(std::string const& host, unsigned int port) {
    boost::system::error_code ec;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    _ptr_socket->connect(endpoint);
}

void Client::run() {
    keep_running {
        boost::system::error_code ec;
        std::string msg(1024, '\0');

        msg = input(">> ", "");

        msg = "<< Client: " + msg;
        msg.resize(1024);

        std::chrono::system_clock::time_point tp_start = std::chrono::system_clock::now();
        
        try {
            boost::asio::write(*_ptr_socket, boost::asio::buffer(msg, 1024));
        } catch (boost::wrapexcept<boost::system::system_error> e) {
            logInfo("[ Error ] Connection Broken. Please connect to other server.");
            connectByInput(false);
        }

        try {
            boost::asio::read(*_ptr_socket, boost::asio::buffer(msg, 1024));
        } catch (boost::wrapexcept<boost::system::system_error> e) {
            logInfo("[ Error ] Connection Broken. Please connect to other server.");
            connectByInput(false);
        }

        std::chrono::system_clock::time_point tp_end = std::chrono::system_clock::now();

        logInfo(msg);
        logInfo(
            "[ Debug ] Roundtrip time for acknowledgment: "
            + std::to_string(
                std::chrono::duration_cast<std::chrono::nanoseconds>(tp_end - tp_start).count() / MICRO
            ) 
            + "ms"
        );

        try {
            boost::asio::read(*_ptr_socket, boost::asio::buffer(msg, 1024));
        } catch (boost::wrapexcept<boost::system::system_error> e) {
            logInfo("[ Error ] Connection Broken. Please connect to other server.");
            connectByInput(false);
        }

        logInfo(msg);
    }
}

void Client::connectByInput(bool isFirstConnect) {
    if (not isFirstConnect) 
        _ptr_socket->close();

    std::string ipAddr = "127.0.0.1";
    std::string port_str = "8080";

    ipAddr = input("Input IP address (default: 127.0.0.1) >> ", "");

    if (ipAddr.empty())
        ipAddr = "127.0.0.1";

    port_str = input("Input port number (default: 8080) >> ", "");

    if (port_str.empty())
        port_str = "8080";

    this->connect(ipAddr, std::atoi(port_str.c_str()));
}

#endif