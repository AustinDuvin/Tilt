#include "AppClass.h"
using namespace Simplex;

void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 5.0f, 10.0f), //Position
		playerLocation,	//Target
		AXIS_Y);					//Up

	//Level Manager load first level, hard coded for now
	m_pLevelMngr->SetLevel("Level_0");

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	//m_pEntityMngr->SetModelMatrix(glm::translate(vector3(0, 3, 0)));
	m_pEntityMngr->UsePhysicsSolver();

	std::vector<vector3> positions = m_pLevelMngr->GetBlockPositions();
	for (int i = 0; i < positions.size(); i++)
	{
		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_" + std::to_string(i));
		//y position is at -1 so the example player model aligns perfectly 
		
		matrix4 m4Position = glm::translate(positions[i]);
		m_pEntityMngr->SetModelMatrix(m4Position);
		
		//m_pEntityMngr->UsePhysicsSolver();
		//m_pEntityMngr->SetMass(i+1);
	}


	
}
void Application::Update(void)
{

	std::vector<vector3> positions = m_pLevelMngr->GetBlockPositions();
	//set matrices of level pieces
	for (int i = 0; i < positions.size(); i++)
	{
		//m_pEntityMngr->GetEntityIndex("Cube_" + std::to_string(i))
		
		//xRotation and yRotation are determined in app controls and applied here
		matrix4 rotation = glm::rotate(IDENTITY_M4, xRotation, AXIS_X) * glm::rotate(IDENTITY_M4, zRotation, AXIS_Z);

		//rotate around 0,0,0

		//rotation first
		matrix4 m4Position = rotation * glm::translate(positions[i]);

		m_pEntityMngr->SetModelMatrix(m4Position, "Cube_" + std::to_string(i));

	}

	//camera follow
	m_v3CharPos = m_pEntityMngr->GetRigidBody("Steve")->GetCenterGlobal(); //get character's position vector
	m_v3CamPos = m_pEntityMngr->GetRigidBody("Steve")->GetCameraFollow(); //get character's position vector

	if (!debugMode)
	{
		m_pCameraMngr->SetPositionTargetAndUp(
			m_v3CamPos, //Position
			m_v3CharPos,//Target
			AXIS_Y);	//Up
	}
	else
	{
		m_pCameraMngr->SetPositionTargetAndUp(
			vector3(0.0f, 5.0f, 25.0f), //Position
			vector3(0.0f, 0.0f, 0.0f),	//Target
			AXIS_Y);					//Up

	}


	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Set the model matrix for the main object
	//m_pEntityMngr->SetModelMatrix(m_m4Steve, "Steve");

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
	//m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	
	// Clear the screen
	ClearScreen();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui,
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release MyEntityManager
	MyEntityManager::ReleaseInstance();
	LevelManager::ReleaseInstance();

	//release GUI
	ShutdownGUI();
}