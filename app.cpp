#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;
using ip::tcp;

string read_file(const string& file_path) {
    ifstream file(file_path);
    if (file) {
        string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
        return content;
    }
    return "";
}

void handle_request(tcp::socket& socket) {
    try {
        // Read the request
        boost::asio::streambuf request;
        boost::asio::read_until(socket, request, "\r\n");

        // Send a response
        string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        response += read_file("index.html");

        boost::asio::write(socket, boost::asio::buffer(response));
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 8080));

        cout << "Server is running on http://localhost:8080" << endl;

        while (true) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            handle_request(socket);
        }
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
