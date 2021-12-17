
#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

const float g_InitialZoomLevel = 1.5f;

const int g_SkyboxMeshResolution = 11; // used in the SceneSerializer

const size_t g_MaxInstanceCount = 100000; // 1000000; // this would be ~52Mb, so the GPU should be able to handle a few of these, but at that point the rendering time probably would take too long

// texture slot bindings:
const int g_MeshTextureSlot = 0;
const int g_RendererShadowDepthSlot = 1;
const int g_SkyboxTextureSlots[6] = { 2,3,4,5,6,7 };

const int g_RendererColorAttchSlot = 8;
const int g_RendererBrightColAttchSlot = 9;
const int g_RendererDepthAttchSlot = 10;


#endif // GLOBAL_CONSTANTS_H
