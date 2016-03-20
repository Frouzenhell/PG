#ifndef __DEMOPROJ_H_
#define __DEMOPROJ_H_

#include <sceneInitializer.h>

class DemoProj : public SceneInitializer
{
private:
	void initShaders();
	void initModels();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

public:
	DemoProj(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif