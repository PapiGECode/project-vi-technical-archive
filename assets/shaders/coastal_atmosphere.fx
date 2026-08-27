// Project VI Community Reconstruction
// Original Direct3D 11 reference shader for the standalone visual sandbox.

Texture2D SceneColor : register(t0);
Texture2D SceneDepth : register(t1);
SamplerState LinearClamp : register(s0);

cbuffer FrameConstants : register(b0) {
  float4x4 InverseViewProjection;
  float3 CameraWorldPosition;
  float TimeSeconds;
  float3 SunDirection;
  float Exposure;
  float3 HorizonColor;
  float HazeDensity;
  float3 ZenithColor;
  float SunDiskPower;
};

struct VertexOutput {
  float4 position : SV_Position;
  float2 uv : TEXCOORD0;
};

VertexOutput FullscreenVertex(uint vertexId : SV_VertexID) {
  VertexOutput output;
  output.uv = float2((vertexId << 1) & 2, vertexId & 2);
  output.position = float4(output.uv * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);
  return output;
}

float3 ReconstructWorldPosition(float2 uv, float depth) {
  float4 clip = float4(uv * float2(2.0, -2.0) + float2(-1.0, 1.0), depth, 1.0);
  float4 world = mul(clip, InverseViewProjection);
  return world.xyz / max(world.w, 1.0e-5);
}

float3 AcesApprox(float3 color) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return saturate((color * (a * color + b)) / (color * (c * color + d) + e));
}

float4 CoastalAtmospherePixel(VertexOutput input) : SV_Target {
  const float depth = SceneDepth.SampleLevel(LinearClamp, input.uv, 0).r;
  const float3 world = ReconstructWorldPosition(input.uv, depth);
  const float3 viewRay = normalize(world - CameraWorldPosition);
  const float distanceToSurface = length(world - CameraWorldPosition);

  const float horizon = pow(saturate(1.0 - abs(viewRay.z)), 1.7);
  const float3 skyGradient = lerp(ZenithColor, HorizonColor, horizon);
  const float sunAlignment = saturate(dot(viewRay, normalize(-SunDirection)));
  const float3 sunDisk = pow(sunAlignment, SunDiskPower) * float3(1.0, 0.62, 0.31) * 8.0;

  const float aerialPerspective = 1.0 - exp(-distanceToSurface * max(HazeDensity, 0.0));
  const float tidePulse = 0.96 + 0.04 * sin(TimeSeconds * 0.12);
  const float3 scene = SceneColor.Sample(LinearClamp, input.uv).rgb;
  const float3 atmosphericColor = (skyGradient * tidePulse) + sunDisk;
  const float3 composited = lerp(scene, atmosphericColor, saturate(aerialPerspective));
  return float4(AcesApprox(composited * exp2(Exposure)), 1.0);
}

technique11 CoastalAtmosphere {
  pass P0 {
    SetVertexShader(CompileShader(vs_5_0, FullscreenVertex()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_5_0, CoastalAtmospherePixel()));
  }
}
