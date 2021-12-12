
#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

const float g_InitialZoomLevel = 1.5f;

const int g_SkyboxMeshResolution = 10; // used in the SceneSerializer

const size_t g_MaxInstanceCount = 100000; // 1000000; // this would be ~52Mb, so the GPU should be able to handle a few of these, but at that point the rendering time probably would take too long










#endif // GLOBAL_CONSTANTS_H
