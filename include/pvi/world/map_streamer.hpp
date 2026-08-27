// Project VI Technical Archive — Lead developer: Cyberleek
#pragma once

#include "pvi/math/vec3.hpp"

#include <cstdint>
#include <unordered_set>
#include <vector>

namespace pvi::world {

struct StreamCell final {
  std::uint32_t id{};
  math::Vec3 center{};
  float loadRadiusM{600.0F};
  float unloadRadiusM{780.0F};
};

struct StreamDelta final {
  std::vector<std::uint32_t> loaded;
  std::vector<std::uint32_t> unloaded;
};

class MapStreamer final {
 public:
  explicit MapStreamer(std::vector<StreamCell> cells);

  [[nodiscard]] StreamDelta update(const math::Vec3& observer);
  [[nodiscard]] bool isLoaded(std::uint32_t cellId) const noexcept;

 private:
  std::vector<StreamCell> cells_;
  std::unordered_set<std::uint32_t> loaded_;
};

}  // namespace pvi::world
