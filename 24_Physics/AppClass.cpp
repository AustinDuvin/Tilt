#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	currentLevel = std::string( "0" );
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 5.0f, 10.0f), //Position
		playerLocation,	//Target
		AXIS_Y);					//Up

	//level manager load first level, hard coded for now
	m_pLevelMngr->SetLevel(currentLevel);

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	m_pEntityMngr->UsePhysicsSolver();
	std::vector<vector3> positions = m_pLevelMngr->GetBlockPositions();

	objectMap = m_pLevelMngr->getObjectMap();
	int nHoles = 0;
	
	for( int i = 0; i < objectMap.size(); i++ )
	{
		if( objectMap[i] == 'H' )
		{
			nHoles++;
		}
		if( objectMap[i] == 'G' )
		{
			goalIndex = i;
		}
	}
	
	//no clue if this is right every time but hopefully is
	int index = ((int)(m_pLevelMngr->GetLevelWidth() / 2))*m_pLevelMngr->GetLevelWidth() + (int)m_pLevelMngr->GetLevelHeight() / 2;
	float playerHeight = 2.0f;
	float playerWidth = 0.5f;
	steveID = m_pEntityMngr->GetEntityIndex("Steve");
	//puts player at middle of the map height offset by its dimensions

	int spawnIndex = m_pLevelMngr->GetSpawnIndex();

	//v3Position.x = -levelWidth / 2 + (float)(index % levelWidth);
	//v3Position.z = -levelHeight / 2 + (float)(index / levelWidth);

	spawnLocation = vector3();
	spawnLocation.x =-m_pLevelMngr->GetLevelWidth()/2 + (spawnIndex % m_pLevelMngr->GetLevelWidth()) + playerWidth;
	spawnLocation.y = positions[spawnIndex].y + (0.5f*playerHeight);
	spawnLocation.z = -m_pLevelMngr->GetLevelHeight()/2 + (spawnIndex / m_pLevelMngr->GetLevelWidth()) + playerWidth;

	goalPos = vector3();
	goalPos.x = -m_pLevelMngr->GetLevelWidth() / 2 + (goalIndex % m_pLevelMngr->GetLevelWidth()) + playerWidth;
	goalPos.y = positions[goalIndex].y + (0.5f*playerHeight);
	goalPos.z = -m_pLevelMngr->GetLevelHeight() / 2 + (goalIndex / m_pLevelMngr->GetLevelWidth()) + playerWidth;
	
	//m_pMeshMngr
	m_pEntityMngr->SetModelMatrix(glm::translate(spawnLocation.x,spawnLocation.y,spawnLocation.z) * glm::rotate(IDENTITY_M4, 180.0f, AXIS_Y), "Steve");

	for (int i = 0; i < positions.size() - nHoles; i++)
	{
		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_" + std::to_string(i));
		//vector3 v3Position = vector3(glm::sphericalRand(12.0f));
		//y position is at -1 so the example player model aligns perfectly 

		//matrix4 m4Position = glm::translate(positions[i]);

		/*
		v3Position.y =-1.0f;
		//make a grid of cubes to be the starter level in a way such that the middle is at X and Z of 0
		v3Position.x =  -5 + (float)(i / 10);
		v3Position.z =  -5 + (float)(i % 10);
		matrix4 m4Position = glm::translate(v3Position);
		*/
		//m_pEntityMngr->SetModelMatrix(m4Position);
		
		//m_pEntityMngr->UsePhysicsSolver();
		//m_pEntityMngr->SetMass(i+1);
	}
}
void Application::Update(void)
{
	std::vector<vector3> positions = m_pLevelMngr->GetBlockPositions();

	int levelHeight = m_pLevelMngr->GetLevelHeight();
	int levelWidth = m_pLevelMngr->GetLevelWidth();

	//MeshManager::GetInstance()->AddWireCubeToRenderList( glm::translate( IDENTITY_M4, origin ) *
	//	glm::scale( vector3( halfDimension.x * 2, halfDimension.y * 2, halfDimension.z * 2 ) ), vector3( 50, 0, 255 ), RENDER_WIRE );

	if (m_pEntityMngr->GetEntity(steveID)->GetPosition().y < -10.0f)
	{
		//reset
		m_pEntityMngr->GetEntity(steveID)->SetPosition(spawnLocation);

		m_pEntityMngr->GetEntity(steveID)->SetVelocity(vector3(0.0f,0.0f,0.0f));
		//reset level rotation too
	}
	//set matrices of level pieces
	int fuck = 0;
	for (int i = 0; i < positions.size(); i++)
	{
		//m_pEntityMngr->GetEntityIndex("Cube_" + std::to_string(i))
		//xRotation and yRotation are determined in app controls and applied here
		if(objectMap[i] =='H' )
		{
		}
		else
		{
			matrix4 rotation = glm::rotate(IDENTITY_M4, xRotation, AXIS_X) * glm::rotate(IDENTITY_M4, zRotation, AXIS_Z);

			//normalVector = vector3(xRotation, 0.0f, zRotation);

			//rotate around 0,0,0

			//rotation first
			matrix4 m4Position = rotation * glm::translate(positions[i]);

			m_pEntityMngr->SetModelMatrix(m4Position, "Cube_" + std::to_string(fuck));
			fuck++;
		}
	}

	m_pEntityMngr->SetModelMatrix(m_pEntityMngr->GetModelMatrix("Ball_" + std::to_string(0)), "Ball_" + std::to_string(0));
	
	//camera follow
	m_v3CharPos = m_pEntityMngr->GetRigidBody("Steve")->GetCenterGlobal(); //get character's position vector
	m_v3CamPos = m_pEntityMngr->GetRigidBody("Steve")->GetCameraFollow(); //get character's position vector


	if (!debugMode)
	{
		m_pCameraMngr->SetPositionTargetAndUp(
			m_v3CamPos, //Position
			m_v3CharPos,//Target
			AXIS_Y);    //Up
	}
	else
	{

		m_pCameraMngr->SetPositionTargetAndUp(
			vector3(0.0f, 5.0f, 25.0f), //Position
			vector3(0.0f, 0.0f, 0.0f),    //Target
			AXIS_Y);                    //Up

	}

	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager and Octant
	//m_uOctantLevels = 2;
	m_pRoot = new MyOctant(m_uOctantLevels, 5);
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

	//display octree
	if (m_uOctantID == -1)
		m_pRoot->Display();
	else
		m_pRoot->Display(m_uOctantID);

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

	//release octree
	SafeDelete(m_pRoot);

	//release GUI
	ShutdownGUI();
}