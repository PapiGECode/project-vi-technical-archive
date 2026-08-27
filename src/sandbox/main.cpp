// Project VI Technical Archive — Lead developer: PapiGEGamer
#include "pvi/physics/vehicle_dynamics.hpp"
#include "pvi/world/map_streamer.hpp"

#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  pvi::physics::VehicleDynamics dynamics;
  pvi::physics::VehicleState vehicle;
  const pvi::physics::SurfaceSample wetAsphalt{.friction = 0.92F, .wetness = 0.35F};

  pvi::world::MapStreamer streamer({
      {.id = 100, .center = {0.0F, 0.0F, 0.0F}},
      {.id = 110, .center = {850.0F, 0.0F, 0.0F}},
      {.id = 120, .center = {1700.0F, 0.0F, 0.0F}},
  });

  constexpr float timeStep = 1.0F / 60.0F;
  for (int frame = 0; frame < 600; ++frame) {
    const pvi::physics::VehicleInput input{
        .throttle = frame < 420 ? 0.78F : 0.0F,
        .brake = frame >= 520 ? 0.45F : 0.0F,
        .steering = frame > 180 && frame < 320 ? 0.22F : 0.0F,
    };
    const auto telemetry = dynamics.step(vehicle, input, wetAsphalt, timeStep);
    const auto streamDelta = streamer.update(vehicle.position);

    if (frame % 60 == 0) {
      std::cout << std::fixed << std::setprecision(2)
                << "t=" << frame * timeStep
                << "s speed=" << telemetry.speedMps * 3.6F
                << "km/h position=(" << vehicle.position.x << ", " << vehicle.position.y << ")"
                << " loaded=" << streamDelta.loaded.size()
                << " unloaded=" << streamDelta.unloaded.size() << '\n';
    }
  }
}
