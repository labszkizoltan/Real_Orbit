
// I intend to create a more cache friendly version of my first OctTree implementation

#ifndef OCTTREE_H
#define OCTTREE_H

#define OCTREE_NODE_CAPACITY 32 // Arbitrary constant to indicate how many elements can be stored in this quad tree node

#include "Vector_3D.h"
#include <vector>
#include <array>
#include <memory>

//----- Helper Structures -----//

struct Box3D
{
	Vec3D center = { 0,0,0 };
	Vec3D radius = { 1,1,1 };

	Box3D() = default;
	bool ContainsPoint(const Vec3D& point);
	bool IntersectsBox3D(const Box3D& other);

};

template <class UserDataType>
struct Node
{
	Box3D m_Boundary;
	Vec3D m_Points[OCTREE_NODE_CAPACITY];
	UserDataType m_UserData[OCTREE_NODE_CAPACITY];
	unsigned char m_ElementCount = 0;

	// Index of children Nodes (short int restricts the whole octtree to have no more than 250k
	// elements, but in my use case that should be way more than enough)
	// also children should only be created at sub-division, and in that case always 8 of them
	// so it should be enough if only the first ones index is stored
	int m_FirstChildIndex = -1;
};


//--------------------//
//----- Oct Tree -----//
//--------------------//


// this is a tight container for Nodes
template <class UserDataType>
class OctTree
{
public:
	OctTree();
	OctTree(Box3D box);

	bool Insert(Vec3D point, UserDataType user_data);
	std::vector<UserDataType> QueryRange(Box3D range);
	void QueryRange(Box3D range, std::vector<UserDataType>& result, int node_idx);

	// clear the vector, but dont deallocate the memory, so we can spare time
	// on not having to reallocate it
	void Clear();// { m_Nodes.clear(); };

private:
	bool InsertIntoNode(const Vec3D& point, const UserDataType& user_data, int node_idx);
	void SubdivideNode(int node_idx);

private:
	std::vector<Node<UserDataType>> m_Nodes;
	Box3D m_Box; // box of the top node
};


//----- OctTree implementation -----//

template<class UserDataType>
OctTree<UserDataType>::OctTree()
{
	// with some extra code this could be created in-place
	// (Node constructor + std::vector<Node> constructor with init list)
	m_Box = Box3D();
	Node<UserDataType> first_node;
	first_node.m_Boundary = Box3D();
	m_Nodes.push_back(first_node);
}

template<class UserDataType>
OctTree<UserDataType>::OctTree(Box3D box)
{
	// with some extra code this could be created in-place
	// (Node constructor + std::vector<Node> constructor with init list)
	m_Box = box;
	Node<UserDataType> first_node;
	first_node.m_Boundary = box;
	m_Nodes.push_back(first_node);
}

template<class UserDataType>
bool OctTree<UserDataType>::Insert(Vec3D point, UserDataType user_data)
{
	return InsertIntoNode(point, user_data, 0);
}

template<class UserDataType>
inline bool OctTree<UserDataType>::InsertIntoNode(const Vec3D& point, const UserDataType& user_data, int node_idx)
{
	if (!m_Nodes[node_idx].m_Boundary.ContainsPoint(point))
		return false;

	// probably the second condition is not needed, as the first child idx will
	// always be -1 until the buffers are not full
	int nodeElementCount = m_Nodes[node_idx].m_ElementCount;
	if (nodeElementCount < OCTREE_NODE_CAPACITY)// && m_Nodes[node_idx].m_FirstChildIndex == -1)
	{
		m_Nodes[node_idx].m_Points[nodeElementCount] = point;
		m_Nodes[node_idx].m_UserData[nodeElementCount] = user_data;
		m_Nodes[node_idx].m_ElementCount++;
		return true;
	}

	if (m_Nodes[node_idx].m_FirstChildIndex == -1)
		SubdivideNode(node_idx);

	int child_idx = m_Nodes[node_idx].m_FirstChildIndex;
	if (InsertIntoNode(point, user_data, child_idx + 0)) { return true; }
	if (InsertIntoNode(point, user_data, child_idx + 1)) { return true; }
	if (InsertIntoNode(point, user_data, child_idx + 2)) { return true; }
	if (InsertIntoNode(point, user_data, child_idx + 3)) { return true; }
	if (InsertIntoNode(point, user_data, child_idx + 4)) { return true; }
	if (InsertIntoNode(point, user_data, child_idx + 5)) { return true; }
	if (InsertIntoNode(point, user_data, child_idx + 6)) { return true; }
	if (InsertIntoNode(point, user_data, child_idx + 7)) { return true; }

	return false;
}

template<class UserDataType>
inline void OctTree<UserDataType>::SubdivideNode(int node_idx)
{
	int firstChildIdx = m_Nodes.size();
	m_Nodes[node_idx].m_FirstChildIndex = firstChildIdx;
	Vec3D center = m_Nodes[node_idx].m_Boundary.center;
	Vec3D radius = m_Nodes[node_idx].m_Boundary.radius;
	Box3D box;
	box.radius = radius / 2.0f;

	for (int i = 0; i < 8; i++)
		m_Nodes.push_back(Node<UserDataType>());

	box.center = Vec3D(center.x - radius.x / 2.0f, center.y - radius.y / 2.0f, center.z - radius.z / 2.0f);
	m_Nodes[firstChildIdx + 0].m_Boundary = box;
	box.center = Vec3D(center.x - radius.x / 2.0f, center.y - radius.y / 2.0f, center.z + radius.z / 2.0f);
	m_Nodes[firstChildIdx + 1].m_Boundary = box;
	box.center = Vec3D(center.x - radius.x / 2.0f, center.y + radius.y / 2.0f, center.z - radius.z / 2.0f);
	m_Nodes[firstChildIdx + 2].m_Boundary = box;
	box.center = Vec3D(center.x - radius.x / 2.0f, center.y + radius.y / 2.0f, center.z + radius.z / 2.0f);
	m_Nodes[firstChildIdx + 3].m_Boundary = box;
	box.center = Vec3D(center.x + radius.x / 2.0f, center.y - radius.y / 2.0f, center.z - radius.z / 2.0f);
	m_Nodes[firstChildIdx + 4].m_Boundary = box;
	box.center = Vec3D(center.x + radius.x / 2.0f, center.y - radius.y / 2.0f, center.z + radius.z / 2.0f);
	m_Nodes[firstChildIdx + 5].m_Boundary = box;
	box.center = Vec3D(center.x + radius.x / 2.0f, center.y + radius.y / 2.0f, center.z - radius.z / 2.0f);
	m_Nodes[firstChildIdx + 6].m_Boundary = box;
	box.center = Vec3D(center.x + radius.x / 2.0f, center.y + radius.y / 2.0f, center.z + radius.z / 2.0f);
	m_Nodes[firstChildIdx + 7].m_Boundary = box;
}


template<class UserDataType>
std::vector<UserDataType> OctTree<UserDataType>::QueryRange(Box3D range)
{
	std::vector<UserDataType> result;
	QueryRange(range, result, 0); // start the querying at the top node
	return result;
}


template<class UserDataType>
void OctTree<UserDataType>::Clear()
{
	m_Nodes.clear();

	Node<UserDataType> top_node;
	top_node.m_Boundary = m_Box;
	m_Nodes.push_back(top_node);
}

template<class UserDataType>
void OctTree<UserDataType>::QueryRange(Box3D range, std::vector<UserDataType>& result, int node_idx)
{
	if (!m_Nodes[node_idx].m_Boundary.IntersectsBox3D(range))
		return;


	for (int i = 0; i < m_Nodes[node_idx].m_ElementCount; i++)
	{
		if (range.ContainsPoint(m_Nodes[node_idx].m_Points[i]))
			result.push_back(m_Nodes[node_idx].m_UserData[i]);
	}

	int firstChildIdx = m_Nodes[node_idx].m_FirstChildIndex;
	if (firstChildIdx == -1)
		return;

	QueryRange(range, result, firstChildIdx + 0);
	QueryRange(range, result, firstChildIdx + 1);
	QueryRange(range, result, firstChildIdx + 2);
	QueryRange(range, result, firstChildIdx + 3);
	QueryRange(range, result, firstChildIdx + 4);
	QueryRange(range, result, firstChildIdx + 5);
	QueryRange(range, result, firstChildIdx + 6);
	QueryRange(range, result, firstChildIdx + 7);
}







#endif // OCTTREE_H
