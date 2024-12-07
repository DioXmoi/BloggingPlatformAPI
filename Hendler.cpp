#include "Hendler.h"


namespace Api {
	// Report a failure
	void Fail(beast::error_code ec, std::string_view what) {
		std::cerr << "[SERVER] " << what << " exception: " << ec.message() << "\n";
	}

	bool IsEndOfStream(const beast::error_code& ec) {
		return ec == http::error::end_of_stream;
	}

	// Handles an HTTP server connection
	void ClientSession(tcp::socket socket) {
		beast::error_code ec;

		// This buffer is required to persist across reads
		beast::flat_buffer buffer;

		while (true) {
			// Read a request
			http::request<http::string_body> req;
			http::read(socket, buffer, req, ec);
			if (IsEndOfStream(ec)) { break; }
			if (ec) { return Fail(ec, "read"); }

			// Handle request
			http::message_generator msg{ Api::HandleEndpoint(std::move(req)) };

			// Determine if we should close the connection
			bool keepAlive = msg.keep_alive();

			// Send the response
			beast::write(socket, msg, ec);

			if (ec) { return Fail(ec, "write"); }
			if (!keepAlive) {
				// This means we should close the connection, usually because
				// the response indicated the "Connection: close" semantic.
				break;
			}
		}

		// Send a TCP shutdown
		socket.shutdown(tcp::socket::shutdown_send, ec);
	}
}