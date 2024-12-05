#pragma once


#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>


#include <string_view>
#include <cstdlib>
#include <iostream>
#include <string>


#include "Post.h"
#include "PostJSONConverter.h"


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


namespace Api {
	// Report a failure
	void Fail(beast::error_code ec, std::string_view what);

	bool IsEndOfStream(const beast::error_code& ec);

	// Handles an HTTP server connection
	void ClientSession(tcp::socket socket);

	bool IsValidHttpMethod(http::verb method);



	// Returns a bad request response
	template <class Body, class Allocator>
	inline http::response<http::string_body> GenerateBadRequest(
		const http::request<Body, http::basic_fields<Allocator>>& req,
		std::string_view why) {
		http::response<http::string_body> res{ http::status::bad_request, req.version() };

		res.set(http::field::content_type, "application/json");
		res.keep_alive(req.keep_alive());
		res.body() = nlohmann::json{ why }.dump(4);
		res.prepare_payload();

		return res;
	}


	// 405 Method Not Allowed
	template <class Body, class Allocator>
	inline http::response<http::string_body> GenerateMethodNotAllowed(
		const http::request<Body, http::basic_fields<Allocator>>& req) {
		http::response<http::string_body> res{ http::status::method_not_allowed, req.version() };

		res.set(http::field::content_type, "application/json");
		res.keep_alive(req.keep_alive());
		res.prepare_payload();

		std::string message = "Method not allowed for this endpoint.";
		res.body() = nlohmann::json{ message }.dump(4);

		return res;
	}

	//404 NOT FOUND
	template <class Body, class Allocator>
	inline http::response<http::string_body> GenerateNotFound(
		const http::request<Body, http::basic_fields<Allocator>>& req, std::string_view why) {
		http::response<http::string_body> res{ http::status::not_found, req.version() };

		res.set(http::field::content_type, "application/json");
		res.keep_alive(req.keep_alive());
		res.prepare_payload();

		res.body() = nlohmann::json{ why }.dump(4);

		return res;
	}


	// Handle POST /posts (create a new post)
	template <class Body, class Allocator>
	inline http::response<http::string_body> HandlerPostPosts(
		const http::request<Body, http::basic_fields<Allocator>>& req) {
		if (req.body().empty()) {
			return GenerateBadRequest(req, "Empty request body");
		}

		try {
			Post newPost{ PostJSONConverter::Parse(req.body()) };

			//Add method add DB

			http::response<http::string_body> res{ http::status::created, req.version() };
			res.set(http::field::content_type, "application/json");
			res.keep_alive(req.keep_alive());
			res.body() = PostJSONConverter::Serialize(newPost);
			res.prepare_payload();
			return res;
		}
		catch (const nlohmann::json::out_of_range& e) {
			std::cerr << "[SERVER] Exception: Key not found: " << e.what() << "\n";
			return GenerateBadRequest(req, "Invalid post data");
		}
		catch (const std::exception& e) {
			std::cerr << "[SERVER] Exception: during POST request processing: " << e.what() << "\n";
			return GenerateBadRequest(req, "Failed to process request");
		}
	}


	// Handle GET /posts (list posts)
	template <class Body, class Allocator>
	inline http::response<http::string_body> HandleGetPosts(
		const http::request<Body, http::basic_fields<Allocator>>& req) {


		return GenerateMethodNotAllowed(req);
	}

	// Function to handle requests to /posts
	template <class Body, class Allocator>
	inline http::response<http::string_body> HandlePostsRequest(
		const http::request<Body, http::basic_fields<Allocator>>& req) {
		if (req.method() == http::verb::get) {
			return HandleGetPosts(req);
		}
		else if (req.method() == http::verb::post) {
			return HandlerPostPosts(req);
		}
		else {
			return GenerateMethodNotAllowed(req);
		}
	}

	// Handle GET /posts/{ ID }
	template <class Body, class Allocator>
	inline http::response<http::string_body> HandleGetPostsByID(
		const http::request<Body, http::basic_fields<Allocator>>& req, ID id) {

		id += 1;
		return GenerateMethodNotAllowed(req);
	}

	// Handle PUT /posts/{ ID }
	template <class Body, class Allocator>
	inline http::response<http::string_body> HandlePutPostsByID(
		const http::request<Body, http::basic_fields<Allocator>>& req, ID id) {

		id += 1;
		return GenerateMethodNotAllowed(req);
	}

	// Handle DELETE /posts/{ ID }
	template <class Body, class Allocator>
	inline http::response<http::string_body> HandleDeletePostsByID(
		const http::request<Body, http::basic_fields<Allocator>>& req, ID id) {

		id += 1;
		return GenerateMethodNotAllowed(req);
	}

	// Function to handle requests to /posts/{ ID }
	template <class Body, class Allocator>
	inline http::response<http::string_body> HandlePostsIDRequest(
		const http::request<Body, http::basic_fields<Allocator>>& req) {
		try {
			ID id{ std::stoi(req.target().substr(7)) };

			if (req.method() == http::verb::get) {
				return HandleGetPostsByID(req, id);
			}
			else if (req.method() == http::verb::put) {
				return HandlePutPostsByID(req, id);
			}
			else if (req.method() == http::verb::delete_) {
				return HandleDeletePostsByID(req, id);
			}
		}
		catch (const std::invalid_argument&) {
			return GenerateBadRequest(req, "Invalid post ID");
		}
		catch (const std::out_of_range&) {
			return GenerateBadRequest(req, "Post ID out of range");
		}

		return GenerateMethodNotAllowed(req);
	}
	
	// Return a response for the given request.
	//
	// The concrete type of the response message (which depends on the request), is type-erased in message_generator.
	template <class Body, class Allocator>
	inline http::message_generator HandleEndpoint(
		http::request<Body, http::basic_fields<Allocator>>&& req) {

		auto target{ req.target() };
		if (target == "/posts") {
			return HandlePostsRequest(req);
		}
		else if (target.starts_with("/posts/")) {
			return HandlePostsIDRequest(req);
		}
		else {
			return GenerateNotFound(req, "Endpoint was not found");
		}
	}
}