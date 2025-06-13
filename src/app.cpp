#include <imgui.h>
#include <app.hpp>

namespace miracle {
namespace {
constexpr auto context_ci = le::Context::CreateInfo{
	.window = le::WindowInfo{.size = {1280, 720}, .title = "miracle"},
};
} // namespace

App::App() : m_context(context_ci) {}

void App::run() {
	while (m_context.is_running()) {
		m_context.next_frame();

		ImGui::ShowDemoWindow();

		m_context.present();
	}
}
} // namespace miracle
