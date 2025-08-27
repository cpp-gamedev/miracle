#pragma once
#include <le2d/context.hpp>
#include <le2d/file_data_loader.hpp>
#include <le2d/service_locator.hpp>
#include <resources.hpp>

namespace miracle {
class App {
  public:
	explicit App();

	void run();

  private:
	void bind_services();

	le::Context m_context;
	le::FileDataLoader m_data_loader{};
	Resources m_resources;

	le::ServiceLocator m_services{};
};
} // namespace miracle
