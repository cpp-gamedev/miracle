#pragma once
#include <djson/string_table.hpp>
#include <klib/assert.hpp>
#include <le2d/asset/asset_loader.hpp>
#include <concepts>

namespace miracle {
class Resources {
  public:
	explicit Resources(le::AssetLoader asset_loader);

	template <std::derived_from<le::IAsset> AssetTypeT>
	[[nodiscard]] auto insert(std::string uri, std::unique_ptr<AssetTypeT> asset) -> AssetTypeT& {
		KLIB_ASSERT(asset);
		auto& ret = *asset;
		m_map.insert_or_assign(std::move(uri), Entry::create(std::move(asset)));
		return ret;
	}

	void erase(std::string_view uri);

	[[nodiscard]] auto contains(std::string_view const uri) const -> bool { return m_map.contains(uri); }

	template <std::derived_from<le::IAsset> AssetTypeT>
	[[nodiscard]] auto peek(std::string_view const uri) const -> AssetTypeT* {
		auto const it = m_map.find(uri);
		if (it == m_map.end() || it->second.type != typeid(AssetTypeT)) { return nullptr; }
		return static_cast<AssetTypeT*>(it->second.asset.get());
	}

	template <std::derived_from<le::IAsset> AssetTypeT>
	[[nodiscard]] auto at(std::string_view const uri) const -> AssetTypeT& {
		auto* ret = peek<AssetTypeT>(uri);
		KLIB_ASSERT(ret);
		return *ret;
	}

	template <std::derived_from<le::IAsset> AssetTypeT>
	[[nodiscard]] auto load(std::string_view const uri) -> AssetTypeT* {
		if (auto* ret = peek<AssetTypeT>(uri); ret) { return ret; }
		std::unique_ptr<AssetTypeT> asset = m_asset_loader.load<AssetTypeT>(uri);
		if (!asset) { return nullptr; }
		return &insert(std::string{uri}, std::move(asset));
	}

	void clear() { m_map.clear(); }

	[[nodiscard]] auto size() const -> std::size_t { return m_map.size(); }
	[[nodiscard]] auto is_empty() const -> bool { return m_map.empty(); }

  private:
	struct Entry {
		template <typename T>
		[[nodiscard]] static auto create(std::unique_ptr<T> asset) -> Entry {
			return Entry{.type = typeid(T), .asset = std::move(asset)};
		}

		std::type_index type;
		std::unique_ptr<le::IAsset> asset{};
	};

	[[nodiscard]] auto find(std::string_view uri, std::type_index type) const -> le::IAsset*;

	le::AssetLoader m_asset_loader;
	dj::StringTable<Entry> m_map{};
};
} // namespace miracle
