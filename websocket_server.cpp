#include "websocket_server.h"
#include "stdafx.h"
namespace websocket {

    // session类的构造函数已在头文件中定义，这里我们实现start方法
    void session::start() {
        // 执行WebSocket握手
        ws_.async_accept([self = shared_from_this()](boost::system::error_code ec) {
            if (!ec) {
                // 握手成功，可以在这里开始读写操作
                self->do_read(); // 假设你有这样一个方法来开始读取操作
            }
            // 注意：实际代码中应处理错误情况
            });
    }

    // 假设的do_read方法，用于开始读取数据
    void session::do_read() {
        ws_.async_read(buffer_, [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                // 成功读取消息，可以在这里处理消息
                // 然后继续读取下一个消息
                self->do_read();
            }
            // 注意：实际代码中应处理错误情况
            });
    }

    // server类构造函数和run方法已在头文件中声明，这里实现它们
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
                // 成功接受新连接
                std::make_shared<session>(std::move(socket))->start();
            }
            // 继续接受下一个连接
            do_accept();
            });
    }

} // namespace websocket
