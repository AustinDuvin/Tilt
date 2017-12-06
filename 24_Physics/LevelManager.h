#pragma once
#include "Definitions.h"
#include <fstream>
#include <vector>

namespace Simplex
{
	class LevelManager
	{
		int levelHeight;
		int levelWidth;

		static LevelManager* m_pInstance;

		//For future milestone. Need to make functions to draw a few new shapes first
		std::vector<char> fullLevel;

		std::vector<char> loadedLevel;
		std::vector<char> heightMap;
		std::vector<char> objectMap;

		std::vector<vector3> blockPositions;
		
	public:
		static LevelManager* GetInstance();
		static void ReleaseInstance(void);
		int GetLevelHeight(void);
		int GetLevelWidth(void);
		void SetLevel(char* filename);
		std::vector<vector3> GetBlockPositions();
		int GetSpawnIndex();

	private:
		// For the next milestone I will expan the level and smooth out all blocks using newly programmed shapes
		
		LevelManager(void);
		LevelManager(LevelManager const& a_pOther);
		LevelManager& operator=(LevelManager const& a_pOther);
		~LevelManager(void);
		void Release(void);
		void Init(void);
		
	};

}