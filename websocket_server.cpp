#include "stdafx.h"
#include "websocket_server.h"

namespace websocket {

    session::session(tcp::socket socket) : ws_(std::move(socket)) {}

    void session::start() {
        ws_.async_accept(
            [self = shared_from_this()](boost::system::error_code ec) {
                if (!ec) self->do_read();
                // ����������
            });
    }

    void session::do_read() {
        ws_.async_read(
            buffer_,
            [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    // ���ﴦ����յ�����Ϣ
                    self->do_read(); // ������ȡ��һ����Ϣ
                }
                // ����������
            });
    }

    void session::do_write(std::string message) {
        auto self = shared_from_this();
        ws_.async_write(
            boost::asio::buffer(message),
            [self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    // ��Ϣ���ͳɹ���Ĵ���
                }
                // ����������
            });
    }

    server::server(boost::asio::io_context& ioc, unsigned short port) : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

    void server::run() {
        ioc_.run();
    }

    void server::do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<session>(std::move(socket))->start();
                }
                do_accept(); // ���������µ�����
            });
    }

} // namespace websocket
