#pragma once
#include <le2d/context.hpp>

namespace miracle {
class App {
  public:
	explicit App();

	void run();

  private:
	le::Context m_context;
};
} // namespace miracle
