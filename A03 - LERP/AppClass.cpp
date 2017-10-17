#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Austin Duvin - axd6329@g.rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits
	float fRadius = 0.95f; //initial orbit radius

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		std::vector<vector3> stops; //vector to hold the stops of the current orbit, it will eventually be added to a vector of vectors 
		for (int j = 0; j < i; j++)
		{
			stops.push_back(vector3(std::cos(2 * PI / i * j) * fRadius, std::sin(2 * PI / i * j) * fRadius, 0)); //add a stop to the stops vector
		}
		stopList.push_back(stops); //add the vector of stops to a vector containing vectors of stops
		fSize += 0.5f; //increment the size for the next orbit
		fRadius += 0.5f; //increment the radius for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	//m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);

	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	static std::vector<int> currentStartList;

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));

		//calculate the current position
		vector3 v3CurrentPos = ZERO_V3;

		if(currentStartList.size() < m_uOrbits)
			currentStartList.push_back(0); // Keeps track of where in the vector of positions it currently is

		vector3 v3Start; // Starting point of the current route
		vector3 v3Stop; // Ending point of the current route

		v3Start = stopList[i][currentStartList[i]]; // Sets the start equal to the current start

											 // Sets the endpoint equal to the next point or 0 if it is at the end of the vector
		if (currentStartList[i] == stopList[i].size() - 1)
			v3Stop = stopList[i][0];
		else
			v3Stop = stopList[i][currentStartList[i] + 1];

		float fPercent = MapValue(fTimer, 0.0f, 1.0f, 0.0f, 1.0f); // How far along the path it is

		v3CurrentPos = glm::lerp(v3Start, v3Stop, fPercent); // Lerp

															 // Checks to see if it is at the endpoint, if it is, go onto the next point and reset deltatime
		if (fPercent > 1.0f)
		{
			for (uint j = 0; j < m_uOrbits; ++j)
			{
				currentStartList[j]++;
				if (currentStartList[j] == stopList[j].size())
					currentStartList[j] = 0;
			}
			fTimer = m_pSystem->GetDeltaTime(uClock);
		}

		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

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
	//release GUI
	ShutdownGUI();
}