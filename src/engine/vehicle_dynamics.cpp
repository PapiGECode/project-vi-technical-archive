// Project VI Technical Archive — Lead developer: PapiGEGamer
#include "pvi/physics/vehicle_dynamics.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace pvi::physics {
namespace {

constexpr float kAirDensityKgM3 = 1.225F;
constexpr float kGravityMps2 = 9.80665F;

float saturate(float value) noexcept {
  return std::clamp(value, 0.0F, 1.0F);
}

}  // namespace

VehicleDynamics::VehicleDynamics(VehicleConfig config) : config_(config) {
  if (config_.massKg <= 0.0F || config_.wheelbaseM <= 0.0F) {
    throw std::invalid_argument("vehicle mass and wheelbase must be positive");
  }
}

VehicleTelemetry VehicleDynamics::step(
    VehicleState& state,
    const VehicleInput& input,
    const SurfaceSample& surface,
    float deltaSeconds) const {
  if (!(deltaSeconds > 0.0F && deltaSeconds <= 0.1F)) {
    throw std::invalid_argument("deltaSeconds must be in (0, 0.1]");
  }

  const float throttle = saturate(input.throttle);
  const float brake = saturate(input.brake);
  const float steer = std::clamp(input.steering, -1.0F, 1.0F) * config_.maxSteerRadians;
  const float speed = state.velocity.length();
  const math::Vec3 forward{std::cos(state.headingRadians), std::sin(state.headingRadians), 0.0F};
  const math::Vec3 right{-forward.y, forward.x, 0.0F};

  const float longitudinalSpeed = state.velocity.dot(forward);
  const float lateralSpeed = state.velocity.dot(right);
  const float wetGripLoss = std::clamp(surface.wetness, 0.0F, 1.0F) * 0.38F;
  const float grip = std::clamp(surface.friction * (1.0F - wetGripLoss), 0.15F, 1.35F);
  const float normalForce = config_.massKg * kGravityMps2 * std::cos(surface.slopeRadians);
  const float tractionLimitN = normalForce * grip;

  const float requestedDriveN = throttle * config_.engineForceN;
  const float driveN = std::min(requestedDriveN, tractionLimitN);
  const float brakeDirection = longitudinalSpeed >= 0.0F ? -1.0F : 1.0F;
  const float brakeN = brake * config_.brakeForceN * brakeDirection;
  const float dragN = 0.5F * kAirDensityKgM3 * config_.dragCoefficient *
                      config_.frontalAreaM2 * speed * speed;
  const float resistanceDirection = longitudinalSpeed >= 0.0F ? -1.0F : 1.0F;
  const float resistanceN = resistanceDirection *
      (dragN + config_.rollingResistance * normalForce);
  const float slopeN = -config_.massKg * kGravityMps2 * std::sin(surface.slopeRadians);
  const float longitudinalForceN = driveN + brakeN + resistanceN + slopeN;
  const float longitudinalAcceleration = longitudinalForceN / config_.massKg;

  const float desiredYawRate = speed > 0.2F
      ? longitudinalSpeed * std::tan(steer) / config_.wheelbaseM
      : 0.0F;
  const float yawBlend = 1.0F - std::exp(-config_.corneringStiffness * grip * deltaSeconds);
  state.yawRateRadiansPerSecond +=
      (desiredYawRate - state.yawRateRadiansPerSecond) * yawBlend;

  const float lateralDamping = std::clamp(config_.corneringStiffness * grip * deltaSeconds, 0.0F, 1.0F);
  const float correctedLateralSpeed = lateralSpeed * (1.0F - lateralDamping);
  const float nextLongitudinalSpeed = longitudinalSpeed + longitudinalAcceleration * deltaSeconds;

  state.headingRadians += state.yawRateRadiansPerSecond * deltaSeconds;
  const math::Vec3 nextForward{std::cos(state.headingRadians), std::sin(state.headingRadians), 0.0F};
  const math::Vec3 nextRight{-nextForward.y, nextForward.x, 0.0F};
  state.velocity = nextForward * nextLongitudinalSpeed + nextRight * correctedLateralSpeed;
  state.position += state.velocity * deltaSeconds;

  return {
      .speedMps = state.velocity.length(),
      .longitudinalAccelerationMps2 = longitudinalAcceleration,
      .lateralAccelerationMps2 = (correctedLateralSpeed - lateralSpeed) / deltaSeconds,
      .effectiveGrip = grip,
      .tractionLimited = requestedDriveN > tractionLimitN,
  };
}

}  // namespace pvi::physics
