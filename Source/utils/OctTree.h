
// I almost copy-pasted the wikipedia description of this data structure
// https://en.wikipedia.org/wiki/Quadtree

// this library could be useful:
// https://pointclouds.org/documentation/classpcl_1_1octree_1_1_octree_base.html
// quote from https://pcl.readthedocs.io/projects/tutorials/en/master/walkthrough.html#octree:
// "Furthermore, a memory pool implementation reduces expensive memory allocation and deallocation operations in scenarios where octrees needs to be created at high rate."

#ifndef OCTTREE_H
#define OCTTREE_H

#define OCTREE_NODE_CAPACITY 4 // Arbitrary constant to indicate how many elements can be stored in this quad tree node

#include "Vector_3D.h"
#include <vector>

//----- Helper Structures -----//

struct Box3D
{
	Vec3D center;
	Vec3D radius;

    Box3D() = default;
	bool ContainsPoint(const Vec3D& point);
	bool IntersectsBox3D(const Box3D& other);

};


//--------------------//
//----- Oct Tree -----//
//--------------------//


template <class UserDataType>
class OctTree
{
public:

	// Methods
	OctTree() = default;
	OctTree(Box3D box);

	bool Insert(Vec3D point, UserDataType user_data);
	std::vector<UserDataType> QueryRange(Box3D range);

private:
	void Subdivide(); // create 8 children that fully divide this cube into 8 qubes of equal volume
	void QueryRange(Box3D range, std::vector<UserDataType>& result);

private:
	Box3D m_Boundary;
	// m_Points and m_UserData are stack allocated I think, which will cause overflow at high element counts
	Vec3D m_Points[OCTREE_NODE_CAPACITY];
	UserDataType m_UserData[OCTREE_NODE_CAPACITY];
	int m_ElementCount = 0;

	// Children
	std::shared_ptr<OctTree> m_Child_000 = nullptr;
	std::shared_ptr<OctTree> m_Child_001 = nullptr;
	std::shared_ptr<OctTree> m_Child_010 = nullptr;
	std::shared_ptr<OctTree> m_Child_011 = nullptr;
	std::shared_ptr<OctTree> m_Child_100 = nullptr;
	std::shared_ptr<OctTree> m_Child_101 = nullptr;
	std::shared_ptr<OctTree> m_Child_110 = nullptr;
	std::shared_ptr<OctTree> m_Child_111 = nullptr;

};

template<class UserDataType>
inline OctTree<UserDataType>::OctTree(Box3D box)
	: m_Boundary(box)
{
}

template<class UserDataType>
bool OctTree<UserDataType>::Insert(Vec3D point, UserDataType user_data)
{
	if (!m_Boundary.ContainsPoint(point))
		return false;

	if (m_ElementCount < OCTREE_NODE_CAPACITY && m_Child_000 == nullptr)
	{
		m_Points[m_ElementCount] = point;
		m_UserData[m_ElementCount] = user_data;
		m_ElementCount++;
		return true;
	}

	if (m_Child_000 == nullptr)
		Subdivide();


	if (m_Child_000->Insert(point, user_data)) { return true; }
	if (m_Child_001->Insert(point, user_data)) { return true; }
	if (m_Child_010->Insert(point, user_data)) { return true; }
	if (m_Child_011->Insert(point, user_data)) { return true; }
	if (m_Child_100->Insert(point, user_data)) { return true; }
	if (m_Child_101->Insert(point, user_data)) { return true; }
	if (m_Child_110->Insert(point, user_data)) { return true; }
	if (m_Child_111->Insert(point, user_data)) { return true; }

	return false;
}

template<class UserDataType>
inline void OctTree<UserDataType>::Subdivide()
{
	Vec3D center = m_Boundary.center;
	Vec3D radius = m_Boundary.radius;
	Box3D box;
	box.radius = radius / 2.0f;

	box.center = Vec3D(center.x - radius.x / 2.0f, center.y - radius.y / 2.0f, center.z - radius.z / 2.0f);
	m_Child_000 = std::make_shared<OctTree>(box);
	box.center = Vec3D(center.x - radius.x / 2.0f, center.y - radius.y / 2.0f, center.z + radius.z / 2.0f);
	m_Child_001 = std::make_shared<OctTree>(box);
	box.center = Vec3D(center.x - radius.x / 2.0f, center.y + radius.y / 2.0f, center.z - radius.z / 2.0f);
	m_Child_010 = std::make_shared<OctTree>(box);
	box.center = Vec3D(center.x - radius.x / 2.0f, center.y + radius.y / 2.0f, center.z + radius.z / 2.0f);
	m_Child_011 = std::make_shared<OctTree>(box);
	box.center = Vec3D(center.x + radius.x / 2.0f, center.y - radius.y / 2.0f, center.z - radius.z / 2.0f);
	m_Child_100 = std::make_shared<OctTree>(box);
	box.center = Vec3D(center.x + radius.x / 2.0f, center.y - radius.y / 2.0f, center.z + radius.z / 2.0f);
	m_Child_101 = std::make_shared<OctTree>(box);
	box.center = Vec3D(center.x + radius.x / 2.0f, center.y + radius.y / 2.0f, center.z - radius.z / 2.0f);
	m_Child_110 = std::make_shared<OctTree>(box);
	box.center = Vec3D(center.x + radius.x / 2.0f, center.y + radius.y / 2.0f, center.z + radius.z / 2.0f);
	m_Child_111 = std::make_shared<OctTree>(box);
}

template<class UserDataType>
inline std::vector<UserDataType> OctTree<UserDataType>::QueryRange(Box3D range)
{
	std::vector<UserDataType> result;
	QueryRange(range, result);

	return result;
}

template<class UserDataType>
void OctTree<UserDataType>::QueryRange(Box3D range, std::vector<UserDataType>& result)
{
	if (!m_Boundary.IntersectsBox3D(range))
		return;

	for (int i = 0; i < m_ElementCount; i++)
	{
		if (range.ContainsPoint(m_Points[i]))
			result.push_back(m_UserData[i]);
	}

	if (m_Child_000 == nullptr)
		return;

	m_Child_000->QueryRange(range, result);
	m_Child_001->QueryRange(range, result);
	m_Child_010->QueryRange(range, result);
	m_Child_011->QueryRange(range, result);
	m_Child_100->QueryRange(range, result);
	m_Child_101->QueryRange(range, result);
	m_Child_110->QueryRange(range, result);
	m_Child_111->QueryRange(range, result);
}



#endif // OCTTREE_H

