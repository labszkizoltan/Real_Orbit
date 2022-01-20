
#ifndef SCENE_UPDATER_CLIENT_H
#define SCENE_UPDATER_CLIENT_H

#include <core/scene/SceneUpdater.h>

class SceneUpdater_client : public SceneUpdater_core
{
public:
	static void UpdateScene(Scene& scene);
};


#endif // SCENE_UPDATER_CLIENT_H
