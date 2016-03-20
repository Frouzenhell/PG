#include "demo5.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_OBJ.h"

void Demo5::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag", 0);
	initShaderProgram("adsOBJ_v3_n3_t3.vert", "adsOBJ_v3_n3_t3.frag", 0);
	//initShaderProgram("bin_phong.vert", "bin_phong.frag", 0);
	resetResPath();
}

void Demo5::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("basic/vase.obj"));
	m_sceneData->models.push_back(m);

	resetResPath();
}

void Demo5::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	VAO* vao2 = new VAO();
	vao2->createFromModel(m_sceneData->models[0]);
	m_sceneData->vaos.push_back(vao2);
}

void Demo5::initMaterials()
{
	Material *m = new Material();
	m->setName("White_opaque");

	//TODO: material settings
	m->m_ambient[0] = 0.4f;
	m->m_ambient[1] = 0.4f;
	m->m_ambient[2] = 0.4f;
	m->m_ambient[3] = 1.f;
	m->m_diffuse[0] = 0.0f;
	m->m_diffuse[1] = 0.8f;
	m->m_diffuse[2] = 0.0f;
	m->m_diffuse[3] = 1.f;
	m->m_specular[0] = 0.9f;
	m->m_specular[1] = 0.9f;
	m->m_specular[2] = 0.9f;
	m->m_specular[3] = 1.f;
	m->m_transparency = 0.0f;
	m_sceneData->materials.push_back(m);


	Material *m2 = new Material();
	m2->setName("White_opaque");

	//TODO: material settings
	m2->m_ambient[0] = 0.8f;
	m2->m_ambient[1] = 0.8f;
	m2->m_ambient[2] = 0.8f;
	m2->m_ambient[3] = 1.f;
	m2->m_diffuse[0] = 1.0f;
	m2->m_diffuse[1] = 0.0f;
	m2->m_diffuse[2] = 0.0f;
	m2->m_diffuse[3] = 1.f;
	m2->m_specular[0] = 0.8f;
	m2->m_specular[1] = 0.8f;
	m2->m_specular[2] = 0.8f;
	m2->m_specular[3] = 1.f;
	m2->m_transparency = 0.5f;
	m_sceneData->materials.push_back(m2);
}

void Demo5::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Demo5::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0, 0, 0);
	obj->setOrientation(0, 0, 90);
	obj->setScale(0.5, 0.5, 0.5);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);


	Entity_OBJ *obj2 = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj2->setPosition(-2, 0, 0);
	obj2->setOrientation(0, 0, 90);
	obj2->m_material = m_sceneData->materials[1];
	obj2->init();
	m_sceneData->sceneEntities.push_back(obj2);

	Entity_OBJ *obj3 = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj3->setPosition(2, 0, 0);
	obj3->setOrientation(0, 0, 90);
	obj3->m_material = m_sceneData->materials[1];
	obj3->init();
	m_sceneData->sceneEntities.push_back(obj3);
}

void Demo5::render()
{
	Entity *e = nullptr;
	SceneSetting *ss = SceneSetting::GetInstance();

#pragma region Draw Info Entities

	ss->m_activeShader = m_sceneData->shaderPrograms[0];
	ss->m_activeShader->enable();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVPMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewProjectionMatrix());

	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
		m_sceneData->infoEntities[i]->draw();

#pragma endregion

#pragma region Draw Scene Entities

	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();

	//TODO
	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");
	//....

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	e = m_sceneData->sceneEntities[0];
	Material::setShaderUniform((e)->m_material, ss->m_activeShader, "material");
	e->draw();
	
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	e = m_sceneData->sceneEntities[1];
	Material::setShaderUniform((e)->m_material, ss->m_activeShader, "material");
	e->draw();
	e = m_sceneData->sceneEntities[2];
	e->draw();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

#pragma endregion
}
