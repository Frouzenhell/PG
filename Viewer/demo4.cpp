#include "demo4.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_OBJ.h"

void Demo4::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag", 0);
	initShaderProgram("adsOBJ_v3_n3_t3.vert", "adsOBJ_v3_n3_t3.frag", 0);
	//initShaderProgram("bin_phong.vert", "bin_phong.frag", 0);
	resetResPath();
}

void Demo4::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("basic/vase.obj"));
	m_sceneData->models.push_back(m);

	resetResPath();
}

void Demo4::initVAOs()
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

void Demo4::initMaterials()
{
	Material *m = new Material();
	m->setName("White_opaque");

	//TODO: material settings
	m->m_ambient[0] = (0.2f, 0.2f, 0.2f, 1.0f);
	m->m_diffuse[0] = (0.0f, 1.f, 0.0f, 1.f);
	m->m_specular[0] = (0.9f, 0.9f, 0.9f, 1.f);
	m->m_transparency = 0.0f;
	m_sceneData->materials.push_back(m);
}

void Demo4::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Demo4::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0, 0, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
}

void Demo4::render()
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

	/*for (Entity **e = m_sceneData->sceneEntities.front(); e <= m_sceneData->sceneEntities.back(); e++){
		glm::mat4 tmp = ss->m_activeCamera->getVM() * (*e)->m_modelMatrix;								//pronasobi matici do oka kamery, budu se divat okem kamery
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVPMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&tmp[0]);

		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&(glm::inverseTranspose(glm::mat3(tmp)))[0]);

		
		(*e)->draw();
	}*/
	e = m_sceneData->sceneEntities[0];
	Material::setShaderUniform((e)->m_material, ss->m_activeShader, "material");
	//Material::
	e->draw();
	
#pragma endregion
}
