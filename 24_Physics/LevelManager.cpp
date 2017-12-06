#include "LevelManager.h"

using namespace Simplex;

Simplex::LevelManager* Simplex::LevelManager::m_pInstance = nullptr;
int Simplex::LevelManager::GetLevelWidth(void)
{
	return levelWidth;
}
void Simplex::LevelManager::SetLevel(char * filename)
{
	//file io

	std::ifstream in("Levels/Level_0.txt");

	if (!in)
	{
		printf("Failed to open file");
	}

	//number of terms
	int nBlocks = 0;
	for (char c = in.get(); in.good(); c = in.get())
	{
		loadedLevel.push_back(c);
		if (c == ',')
			nBlocks++;
		if(c == '\n')
			nBlocks++;
	}
	nBlocks += 1;

	int width = 0;
	for (int index = 0; index < loadedLevel.size(); index++)
	{
		if (loadedLevel[index] == ',')
			width++;
		if (loadedLevel[index] == '\n')
				break;
	}
	width++;

	levelWidth = width;
	levelHeight = nBlocks / width;

	//chars in an entire row of the level file
	int charWidth = (width * 3);

	//fill out height map
	for( int outer = 0; outer < levelHeight; outer++)
	{
		for (int inner = 0; inner < charWidth; inner+=3 )
		{
			//outer * charwidth is current row
			//inner + 0/1 is current char in row
			heightMap.push_back(loadedLevel[outer * charWidth + (inner + 0)]);
			objectMap.push_back(loadedLevel[outer * charWidth + (inner + 1)]);
		}
	}

	putchar('\n');

	if (in.bad())
	{
		printf("Something went wrong reading the level data");
	}
	else if (in.eof())
	{
		printf("Successfully reached end of file");
	}
	else
	{
		printf("Unexpected error");
	}

	for (int index = 0; index < nBlocks; index++)
	{
		vector3 v3Position = vector3();
		//0 is char to int conversion
		v3Position.y = -1.0f + (heightMap[index] -'0' );
		v3Position.x = -levelWidth/2 + (float)(index % levelWidth);
		v3Position.z = -levelHeight/2 + (float)(index / levelWidth);
		blockPositions.push_back(v3Position);
	}
}

std::vector<vector3> Simplex::LevelManager::GetBlockPositions()
{
	return blockPositions;
}

LevelManager * Simplex::LevelManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new LevelManager();
	}
	return m_pInstance;
}

void Simplex::LevelManager::ReleaseInstance(void)
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

int Simplex::LevelManager::GetLevelHeight(void)
{
	return levelHeight;
}

Simplex::LevelManager::LevelManager() { Init(); }
Simplex::LevelManager::LevelManager(LevelManager const& a_pOther) {}
Simplex::LevelManager& Simplex::LevelManager::operator=(LevelManager const& a_pOther) { return *this; }
Simplex::LevelManager::~LevelManager() { Release(); }


void Simplex::LevelManager::Release(void)
{
}

void Simplex::LevelManager::Init(void)
{
	levelHeight = 0;
	levelWidth = 0;
};