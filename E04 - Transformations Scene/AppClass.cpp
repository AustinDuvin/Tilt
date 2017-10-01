#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Austin Duvin - axd6329@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//init the mesh
	objectNumber = 46;
	// Creates the list of meshes with a for loop
	for (int i = 0; i < objectNumber; i++)
	{
		MyMesh* m_pMesh = new MyMesh();
		objectList.push_back(m_pMesh);
		objectList[i]->GenerateCube(1.0f, C_BLACK);
	}

	// Checks the 2d array that holds the shape of the object
	// Creates a new position vector if there is a 1 in the current
	// index
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if(shape[i][j] == 1)
			{
				translationList.push_back(vector3(j, i, 0.0f));
			}
		}
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	static float value = 0.0f;
	
	// Loops through the objects and updates the transform
	for (int i = 0; i < objectNumber; i++)
	{
		matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(translationList[i].x + value, translationList[i].y, translationList[i].z));
		matrix4 m4Model = m4Translate;
		objectList[i]->Render(m4Projection, m4View, m4Model);
	}
	// Updates the position every frame
	value += 0.01f;
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	for (int i = 0; i < objectNumber; i++)
	{
		SafeDelete(objectList[i]);
	}

	//release GUI
	ShutdownGUI();
}