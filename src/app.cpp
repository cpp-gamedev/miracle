#include <imgui.h>
#include <app.hpp>
#include <djson/json.hpp>
#include <game.hpp>
#include <log.hpp>

namespace miracle {
namespace {
constexpr auto window_size = glm::ivec2{800, 800};
constexpr auto window_flags = le::default_window_flags_v & ~le::WindowFlag::Resizeable;
constexpr auto context_ci = le::Context::CreateInfo{
	.window = le::WindowInfo{.size = window_size, .title = "miracle", .flags = window_flags},
};
} // namespace

App::App() : m_context(context_ci), m_data_loader(le::FileDataLoader::upfind("assets")), m_asset_loader(m_context.create_asset_loader(&m_data_loader)) {
	bind_services();
}

void App::run() {
	auto game = Game{&m_services};

	auto delta_time = kvf::DeltaTime{};
	while (m_context.is_running()) {
		m_context.next_frame();
		auto const dt = delta_time.tick();
		m_input_router.dispatch(m_context.event_queue());
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

	m_services.bind(&m_asset_loader);

	m_services.bind(&m_input_router);
}
} // namespace miracle
