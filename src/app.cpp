#include <imgui.h>
#include <app.hpp>
#include <djson/json.hpp>
#include <log.hpp>

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
}

void App::run() {
	while (m_context.is_running()) {
		m_context.next_frame();

		ImGui::ShowDemoWindow();

		m_context.present();
	}
}

void App::bind_services() {
	m_services.bind(&m_context);
	// m_data_loader is bound to both the interface and the concrete class for use through either type.
	m_services.bind<le::IDataLoader>(&m_data_loader);
	m_services.bind<le::FileDataLoader>(&m_data_loader);
}
} // namespace miracle
