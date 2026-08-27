// Project VI Technical Archive — Lead developer: Cyberleek
#include "pvi/world/map_streamer.hpp"

#include <stdexcept>
#include <utility>

namespace pvi::world {

MapStreamer::MapStreamer(std::vector<StreamCell> cells) : cells_(std::move(cells)) {
  std::unordered_set<std::uint32_t> ids;
  for (const auto& cell : cells_) {
    if (cell.loadRadiusM <= 0.0F || cell.unloadRadiusM < cell.loadRadiusM) {
      throw std::invalid_argument("stream cell radii are invalid");
    }
    if (!ids.insert(cell.id).second) {
      throw std::invalid_argument("stream cell ids must be unique");
    }
  }
}

StreamDelta MapStreamer::update(const math::Vec3& observer) {
  StreamDelta delta;
  for (const auto& cell : cells_) {
    const float distance = (cell.center - observer).length();
    const bool loaded = loaded_.contains(cell.id);
    if (!loaded && distance <= cell.loadRadiusM) {
      loaded_.insert(cell.id);
      delta.loaded.push_back(cell.id);
    } else if (loaded && distance > cell.unloadRadiusM) {
      loaded_.erase(cell.id);
      delta.unloaded.push_back(cell.id);
    }
  }
  return delta;
}

bool MapStreamer::isLoaded(std::uint32_t cellId) const noexcept {
  return loaded_.contains(cellId);
}

}  // namespace pvi::world
