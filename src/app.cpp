#include <imgui.h>
#include <app.hpp>
#include <djson/json.hpp>
#include <game.hpp>
#include <klib/visitor.hpp>
#include <log.hpp>

namespace miracle {
namespace {
constexpr auto window_size = glm::ivec2{800, 800};
constexpr auto window_flags = le::default_window_flags_v & ~le::WindowFlag::Resizeable;
constexpr auto context_ci = le::Context::CreateInfo{
	.window = le::WindowInfo{.size = window_size, .title = "miracle", .flags = window_flags},
};
} // namespace

App::App() : m_context(context_ci), m_data_loader(le::FileDataLoader::upfind("assets")) { bind_services(); }

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
