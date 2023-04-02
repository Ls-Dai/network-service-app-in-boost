#ifndef _SESSION_H_
#define _SESSION_H_

#ifndef _STD_CPP_
#define _STD_CPP_
#include <bits/stdc++.h>
#endif

#include <boost/asio.hpp>

using namespace boost::asio;
using namespace std;


class Session: public enable_shared_from_this<Session> {
private:
    ip::tcp::socket _socket;
    string _buff;

    void _doRead(void);
    void _doWrite(size_t length);

public:
    Session(ip::tcp::socket socket);
    void start(void);
};

Session::Session(ip::tcp::socket socket): _socket(move(socket)) {}

void Session::start(void) {
    _doRead();
}

void Session::_doRead(void) {
    shared_ptr<Session> self(shared_from_this());
    _socket.async_read_some(
        buffer(_buff),
        [this, self](error_code ec, size_t length) {
            if (not ec) {
                _doWrite(length);
            }
        }
    );
    return;
}

void Session::_doWrite(size_t length) {
    shared_ptr<Session> self(shared_from_this());
    async_write(
        _socket,
        buffer(_buff, length),
        [this, self](error_code ec, size_t length) {
            if (not ec) {
                _doRead();
            }
        }
    );
    return;
}


#endif