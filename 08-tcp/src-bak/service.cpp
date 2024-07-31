#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

class Session : public boost::enable_shared_from_this<Session> {
public:
    Session(boost::asio::io_context& io_context)
        : socket_(io_context) {}

    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&Session::handle_read, shared_from_this(),
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

private:
    void handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred) {
        if (!error) {
            // 处理接收到的数据
            std::cout.write(data_, bytes_transferred);
            std::cout << "\n";

            // 发送数据回客户端
            socket_.async_write_some(boost::asio::buffer(data_, bytes_transferred),
                                     boost::bind(&Session::handle_write, shared_from_this(),
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred));
        } else {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/) {
        // 开始新的读取操作
        start();
    }

    enum { max_length = 1024 };
    char data_[max_length];
    tcp::socket socket_;
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

private:
    void start_accept() {
        boost::shared_ptr<Session> new_session = (boost::shared_ptr<Session>)new Session(io_context_);
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&Server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(boost::shared_ptr<Session> new_session,
                       const boost::system::error_code& error) {
        if (!error) {
            sessions_.push_back(new_session);
            new_session->start();
        }

        // delete new_session; // 如果接受失败，释放资源

        start_accept(); // 尝试接受下一个连接
    }

    boost::asio::io_context io_context_;
    tcp::acceptor acceptor_;
    std::vector<boost::shared_ptr<Session>> sessions_;
};

int main() {
    try {
        boost::asio::io_context io_context;

        Server s(io_context, 8010);

        std::vector<boost::shared_ptr<boost::thread> > threads;
        for (int i = 0; i < 5; ++i) {
            boost::shared_ptr<boost::thread> thread(
                new boost::thread(boost::bind(&boost::asio::io_context::run, &io_context)));
            threads.push_back(thread);
        }

        for (int i = 0; i < 5; ++i)
            threads[i]->join();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}