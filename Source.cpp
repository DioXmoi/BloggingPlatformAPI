//#include <iostream>
//#include <string>
//#include <vector>
//#include <memory>
//#include <thread>
//#include <algorithm>
//
//#include <boost/asio.hpp>
//#include <boost/beast.hpp>
//
//
//#include "Utility.h"
//
//using SocketPtr = std::shared_ptr<boost::asio::ip::tcp::socket>;
//
//void ClientSession(SocketPtr sock);
//
//int main() {
//	boost::asio::io_service service;
//
//	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 80);
//	boost::asio::ip::tcp::acceptor acc(service, ep);
//
//	std::cout << "[SERVER] Start\n";
//
//	while (true) {
//		SocketPtr sock(std::make_shared<boost::asio::ip::tcp::socket>(service));
//		acc.accept(*sock);
//
//		std::cout << "[SERVER] Connection\n";
//
//		std::thread thread{ ClientSession, sock };
//		thread.join();
//	}
//
//
//	std::cout << "[SERVER] End\n";
//
//	return 0;
//}
//
//void ClientSession(SocketPtr sock) {
//	std::string buff("", 1024);
//	try {
//		while (true) {
//			std::size_t bytes_transferred = sock->read_some(boost::asio::buffer(buff.data() + buff.size() - 1024, 1024));
//			buff.resize(buff.size() - 1024 + bytes_transferred); // Adjust size after read
//			if (bytes_transferred == 0) { break; }
//			buff.resize(buff.size() + 1024); // Increase buffer size as needed
//		}
//	}
//	catch (std::exception& e) {
//		std::cout << "[SERVER] Exception: " << e.what() << "\n";
//	}
//
//	auto it{ std::find(buff.begin(), buff.end(), '\0') };
//	buff.erase(it, buff.end());
//	std::cout << "[SERVER] Received: " << buff.size() << " bytes: " << buff << "\n";
//}










//// Returns a not found response
	//template <class Body, class Allocator>
	//inline http::response<http::string_body> NotFound(
	//	const http::request<Body, http::basic_fields<Allocator>>& req,
	//	std::string_view target) {

	//	return http::response<http::string_body> { http::status::bad_request, req.version() };
	//}


	//// Returns a server error response
	//template <class Body, class Allocator>
	//inline http::response<http::string_body> ServerError(
	//	const http::request<Body, http::basic_fields<Allocator>>& req,
	//	std::string_view what) {

	//	return http::response<http::string_body> { http::status::bad_request, req.version() };
	//}