#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";
	m_Mesh = new MyMesh();
	m_Mesh->AddVertexPosition(vector3(0, 0, 0));
	m_Mesh->AddVertexPosition(vector3(1, 0, 0));
	m_Mesh->AddVertexPosition(vector3(0, 1, 0));

	m_Mesh->AddVertexPosition(vector3(0, 1, 0));
	m_Mesh->AddVertexPosition(vector3(1, 1, 0));
	m_Mesh->AddVertexPosition(vector3(1, 0, 0));

	m_Mesh->AddVertexColor(vector3(1, 1, 0));
	m_Mesh->AddVertexColor(vector3(0, 0, 1));
	m_Mesh->AddVertexColor(vector3(0, 0, 0));


	m_Mesh->CompileOpenGL3X();

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
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
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Model = ToMatrix4(m_qArcBall);

	m_Mesh->Render(m4Projection, m4View, m4View);
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_Mesh != nullptr)
	{
		delete m_Mesh;
		m_Mesh = nullptr;
	}
	//release GUI
	ShutdownGUI();
}