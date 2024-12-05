#pragma once


#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>


#include <nlohmann/json.hpp>


#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <thread>


#include "Hendler.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


namespace Api {
	class Server {
	public:
		Server(net::ip::address addr, net::ip::port_type port)
			: m_ep{ addr, port }
			, m_ioc{ 1 }
		{
		}

		void Start() {
			// The acceptor receives incoming connections
			tcp::acceptor acceptor{ m_ioc, m_ep };

			std::cerr << "[SERVER] START\n";
			while (true) {
				// This will receive the new connection
				tcp::socket socket{ m_ioc };

				// Block until we get a connection
				acceptor.accept(socket);

				// Launch the session, transferring ownership of the socket
				std::thread{ Api::ClientSession, std::move(socket) }.detach();
			}
		}

	private:
		tcp::endpoint m_ep;
		net::io_context m_ioc;
	};
}