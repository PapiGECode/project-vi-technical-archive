// Project VI Technical Archive — Lead developer: Cyberleek
#pragma once

#include "pvi/math/vec3.hpp"

namespace pvi::physics {

struct VehicleConfig final {
  float massKg{1680.0F};
  float engineForceN{8200.0F};
  float brakeForceN{12500.0F};
  float dragCoefficient{0.34F};
  float frontalAreaM2{2.18F};
  float rollingResistance{0.014F};
  float corneringStiffness{7.4F};
  float wheelbaseM{2.72F};
  float maxSteerRadians{0.58F};
};

struct VehicleInput final {
  float throttle{};
  float brake{};
  float steering{};
};

struct SurfaceSample final {
  float friction{1.0F};
  float wetness{};
  float slopeRadians{};
};

struct VehicleState final {
  math::Vec3 position{};
  math::Vec3 velocity{};
  float headingRadians{};
  float yawRateRadiansPerSecond{};
};

struct VehicleTelemetry final {
  float speedMps{};
  float longitudinalAccelerationMps2{};
  float lateralAccelerationMps2{};
  float effectiveGrip{};
  bool tractionLimited{};
};

class VehicleDynamics final {
 public:
  explicit VehicleDynamics(VehicleConfig config = {});

  [[nodiscard]] VehicleTelemetry step(
      VehicleState& state,
      const VehicleInput& input,
      const SurfaceSample& surface,
      float deltaSeconds) const;

 private:
  VehicleConfig config_;
};

}  // namespace pvi::physics
