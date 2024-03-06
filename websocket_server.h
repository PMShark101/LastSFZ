// websocket_server.h
#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include "stdafx.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace websocket {

    using tcp = boost::asio::ip::tcp;               // 使用类型别名简化类型名称
    namespace websocket = boost::beast::websocket;  // 使用类型别名简化类型名称

    class session : public std::enable_shared_from_this<session> {
    public:
        explicit session(tcp::socket socket) : ws_(std::move(socket)) {}

        websocket::stream<tcp::socket> ws_;
        boost::beast::flat_buffer buffer_;

        void start() {
            // 在这里执行WebSocket握手并开始读取或写入操作
        }

    private:
        websocket::stream<tcp::socket> ws_; // WebSocket流
    };

    class server {
    public:
        server(boost::asio::io_context& ioc, unsigned short port)
            : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)) {
            do_accept();
        }

        void run() {
            ioc_.run();
        }

    private:
        void do_accept() {
            acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    // 成功接受连接后，创建一个新的session
                    std::make_shared<session>(std::move(socket))->start();
                }
                do_accept(); // 继续接受新的连接
                });
        }

        boost::asio::io_context& ioc_;
        tcp::acceptor acceptor_;
    };

} // namespace websocket

#endif // WEBSOCKET_SERVER_H
