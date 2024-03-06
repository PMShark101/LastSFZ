#include "websocket_server.h"
#include "stdafx.h"
namespace websocket {

    // session��Ĺ��캯������ͷ�ļ��ж��壬��������ʵ��start����
    void session::start() {
        // ִ��WebSocket����
        ws_.async_accept([self = shared_from_this()](boost::system::error_code ec) {
            if (!ec) {
                // ���ֳɹ������������￪ʼ��д����
                self->do_read(); // ������������һ����������ʼ��ȡ����
            }
            // ע�⣺ʵ�ʴ�����Ӧ����������
            });
    }

    // �����do_read���������ڿ�ʼ��ȡ����
    void session::do_read() {
        ws_.async_read(buffer_, [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                // �ɹ���ȡ��Ϣ�����������ﴦ����Ϣ
                // Ȼ�������ȡ��һ����Ϣ
                self->do_read();
            }
            // ע�⣺ʵ�ʴ�����Ӧ����������
            });
    }

    // server�๹�캯����run��������ͷ�ļ�������������ʵ������
    server::server(boost::asio::io_context& ioc, unsigned short port)
        : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

    void server::run() {
        ioc_.run();
    }

    void server::do_accept() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                // �ɹ�����������
                std::make_shared<session>(std::move(socket))->start();
            }
            // ����������һ������
            do_accept();
            });
    }

} // namespace websocket
