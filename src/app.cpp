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
	// test code, remove later.
	if (auto json = dj::Json{}; m_data_loader.load_json(json, "test_file.json")) { log.info("loaded JSON: {}", json); }
}

void App::run() {
	while (m_context.is_running()) {
		m_context.next_frame();

		ImGui::ShowDemoWindow();

		m_context.present();
	}
}
} // namespace miracle
