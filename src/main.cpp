#include <app.hpp>
#include <log.hpp>

auto main() -> int {
	try {
		auto app = miracle::App{};
		app.run();
	} catch (std::exception const& e) {
		miracle::log.error("PANIC: {}", e.what());
		return EXIT_FAILURE;
	} catch (...) {
		miracle::log.error("PANIC: unknown exception");
		return EXIT_FAILURE;
	}
}
