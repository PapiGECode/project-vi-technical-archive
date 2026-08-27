-- Project VI Technical Archive — Lead developer: Cyberleek
-- Original sandbox profile; no proprietary runtime or game data required.

local VehicleProfile = {}

VehicleProfile.schema = "pvi.vehicle-profile/v1"
VehicleProfile.id = "coastal_sport_coupe"
VehicleProfile.mass_kg = 1680.0
VehicleProfile.engine_force_n = 8200.0
VehicleProfile.brake_force_n = 12500.0
VehicleProfile.drag_coefficient = 0.34
VehicleProfile.frontal_area_m2 = 2.18
VehicleProfile.wheelbase_m = 2.72
VehicleProfile.max_steer_radians = 0.58

VehicleProfile.surfaces = {
  dry_asphalt = { friction = 1.00, wetness = 0.00 },
  wet_asphalt = { friction = 0.92, wetness = 0.35 },
  beach_sand = { friction = 0.56, wetness = 0.08 },
}

function VehicleProfile.validate(profile)
  assert(type(profile.id) == "string" and #profile.id > 0, "profile.id is required")
  assert(profile.mass_kg > 0, "mass_kg must be positive")
  assert(profile.engine_force_n >= 0, "engine_force_n cannot be negative")
  assert(profile.brake_force_n >= 0, "brake_force_n cannot be negative")
  assert(profile.wheelbase_m > 0, "wheelbase_m must be positive")

  for name, surface in pairs(profile.surfaces) do
    assert(surface.friction > 0, name .. ".friction must be positive")
    assert(surface.wetness >= 0 and surface.wetness <= 1, name .. ".wetness must be in [0, 1]")
  end
  return true
end

return VehicleProfile
