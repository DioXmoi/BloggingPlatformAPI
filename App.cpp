#include "Server.h"



int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	try {
		Api::Server server{ net::ip::make_address("127.0.0.1"), 80 };

		server.Start();
	}
	catch (std::exception& e) {
		std::cerr << "[SERVER] exception: " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return 0;
}