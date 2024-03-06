// websocket_server.h
#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include "stdafx.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace websocket {

    using tcp = boost::asio::ip::tcp;               // ʹ�����ͱ�������������
    namespace websocket = boost::beast::websocket;  // ʹ�����ͱ�������������

    class session : public std::enable_shared_from_this<session> {
    public:
        explicit session(tcp::socket socket) : ws_(std::move(socket)) {}

        websocket::stream<tcp::socket> ws_;
        boost::beast::flat_buffer buffer_;

        void start() {
            // ������ִ��WebSocket���ֲ���ʼ��ȡ��д�����
        }

    private:
        websocket::stream<tcp::socket> ws_; // WebSocket��
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
                    // �ɹ��������Ӻ󣬴���һ���µ�session
                    std::make_shared<session>(std::move(socket))->start();
                }
                do_accept(); // ���������µ�����
                });
        }

        boost::asio::io_context& ioc_;
        tcp::acceptor acceptor_;
    };

} // namespace websocket

#endif // WEBSOCKET_SERVER_H
