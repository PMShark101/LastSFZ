#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <string>

namespace websocket {

    using tcp = boost::asio::ip::tcp;               // Simplify the type name
    namespace websocket = boost::beast::websocket;  // Simplify the type name

    class session : public std::enable_shared_from_this<session> {
    public:
        explicit session(tcp::socket socket);

        void start();
        void do_read();
        void do_write(std::string message);

    private:
        websocket::stream<tcp::socket> ws_;
        boost::beast::flat_buffer buffer_;
    };

    class server {
    public:
        server(boost::asio::io_context& ioc, unsigned short port);

        void run();
        void do_accept();

    private:
        boost::asio::io_context& ioc_;
        tcp::acceptor acceptor_;
    };

} // namespace websocket

#endif // WEBSOCKET_SERVER_H
