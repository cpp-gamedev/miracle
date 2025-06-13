#pragma once
#include <le2d/context.hpp>
#include <le2d/file_data_loader.hpp>

namespace miracle {
class App {
  public:
	explicit App();

	void run();

  private:
	le::Context m_context;
	le::FileDataLoader m_data_loader{};
};
} // namespace miracle
