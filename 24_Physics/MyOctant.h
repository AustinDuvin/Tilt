//Programmer: Eleanor Post (eap9238@g.rit.edu)
//Date 12/5/2017

#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_ 

#include "MyEntityManager.h"

namespace Simplex
{

	//System Class
	class MyOctant 
	{
		static uint m_uOctantCount; //stores number of octants
		static uint m_uMaxLevel; //stores maximum octant level
		static uint m_uIdealEntityCount; //stores how many objects SHOULD be in a subdivision

		uint m_uID = 0; //stores current ID of octant
		uint m_uLevel = 0; //will store current level of octant
		uint m_uChildren = 0; //Number of children on the octant (0, 8)

		float m_fSize = 0.0f; //size of octant

		MeshManager* m_pMeshMngr = nullptr; //mesh manager singleton
		MyEntityManager* m_pEntityMngr = nullptr; //Entity manager singleton

		vector3 m_v3Center = vector3(0.0f); //stores center point of octant
		vector3 m_v3Min = vector3(0.0f); //minimum vector of octant
		vector3 m_v3Max = vector3(0.0f); //maximum vector of octant

		MyOctant* m_pParent = nullptr; //stores the parent of the current octant
		MyOctant* m_pChild[8]; //stores children of current octant

		std::vector<uint> m_EntityList; //list of entities within this octant

		MyOctant* m_pRoot = nullptr; //root octant
		std::vector<MyOctant*> m_lChild; //list of nodes that contain objects (applied to root only)

	public:
		
		MyOctant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);
		MyOctant(vector3 a_v3Center, float a_fSize);
		MyOctant(MyOctant const& other);
		MyOctant& operator=(MyOctant const& other);
		~MyOctant(void);
		void Swap(MyOctant& other);
		float GetSize(void);
		vector3 GetCenterGlobal(void);
		vector3 GetMinGlobal(void);
		vector3 GetMaxGlobal(void);
		bool IsColliding(uint a_uRBIndex);
		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);
		void Display(vector3 a_v3Color = C_BLUE_CORNFLOWER);
		void DisplayLeafs(vector3 a_v3Color = C_GREEN_LIME);
		void ClearEntityList(void);
		void Subdivide(void);
		MyOctant* GetChild(uint a_nChild);
		MyOctant* GetParent(void);
		bool IsLeaf(void);
		bool ContainsMoreThan(uint a_nEntities);
		void KillBranches(void);
		void ConstructTree(uint a_nMaxLevel = 3);
		void AssignIDtoEntity(void);
		uint GetOctantCount(void);

	private:

		void Release(void);
		void Init(void);
		void ConstructList(void);

	};

}

#endif // ! __MYOCTANTCLASS_H_
