#include <imgui.h>
#include <app.hpp>
#include <djson/json.hpp>
#include <game.hpp>
#include <klib/visitor.hpp>
#include <log.hpp>
#include <util/random.hpp>

namespace miracle {
namespace {
constexpr auto context_ci = le::Context::CreateInfo{
	.window = le::WindowInfo{.size = {1280, 720}, .title = "miracle"},
};
} // namespace

App::App() : m_context(context_ci), m_data_loader(le::FileDataLoader::upfind("assets")) {
	bind_services();

	// test code, remove later.
	auto json = dj::Json{};
	if (m_services.get<le::IDataLoader>().load_json(json, "test_file.json")) { log.info("loaded JSON: {}", json); }
	log.debug("random_range(1, 100): {}", util::random_range(1, 100));
}

void App::run() {
	auto game = Game{&m_services};

	auto const event_visitor = klib::SubVisitor{
		[&game](le::event::CursorPos const& cursor_pos) { game.on_cursor_pos(cursor_pos); },
	};

	auto delta_time = kvf::DeltaTime{};
	while (m_context.is_running()) {
		m_context.next_frame();
		auto const dt = delta_time.tick();
		for (auto const& event : m_context.event_queue()) { std::visit(event_visitor, event); }
		game.tick(dt);
		if (auto renderer = m_context.begin_render()) { game.render(renderer); }
		m_context.present();
	}
	m_context.wait_idle();
}

void App::bind_services() {
	m_services.bind(&m_context);
	// m_data_loader is bound to both the interface and the concrete class for use through either type.
	m_services.bind<le::IDataLoader>(&m_data_loader);
	m_services.bind<le::FileDataLoader>(&m_data_loader);
}
} // namespace miracle
