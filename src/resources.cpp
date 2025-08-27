#include <resources.hpp>

namespace miracle {
Resources::Resources(le::AssetLoader asset_loader) : m_asset_loader(std::move(asset_loader)) {}

void Resources::erase(std::string_view const uri) {
	if (auto const it = m_map.find(uri); it != m_map.end()) { m_map.erase(it); }
}

auto Resources::find(std::string_view const uri, std::type_index const type) const -> le::IAsset* {
	auto const it = m_map.find(uri);
	if (it == m_map.end() || it->second.type != type) { return nullptr; }
	return it->second.asset.get();
}
} // namespace miracle
