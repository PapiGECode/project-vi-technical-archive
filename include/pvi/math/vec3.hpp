// Project VI Technical Archive — Lead developer: PapiGEGamer
#pragma once

#include <cmath>

namespace pvi::math {

struct Vec3 final {
  float x{};
  float y{};
  float z{};

  [[nodiscard]] constexpr Vec3 operator+(const Vec3& rhs) const noexcept {
    return {x + rhs.x, y + rhs.y, z + rhs.z};
  }

  [[nodiscard]] constexpr Vec3 operator-(const Vec3& rhs) const noexcept {
    return {x - rhs.x, y - rhs.y, z - rhs.z};
  }

  [[nodiscard]] constexpr Vec3 operator*(float scalar) const noexcept {
    return {x * scalar, y * scalar, z * scalar};
  }

  constexpr Vec3& operator+=(const Vec3& rhs) noexcept {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  [[nodiscard]] constexpr float dot(const Vec3& rhs) const noexcept {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  [[nodiscard]] float length() const noexcept {
    return std::sqrt(dot(*this));
  }

  [[nodiscard]] Vec3 normalized(float epsilon = 1.0e-5F) const noexcept {
    const float magnitude = length();
    return magnitude > epsilon ? *this * (1.0F / magnitude) : Vec3{};
  }
};

}  // namespace pvi::math
