#include "MeshLibrary.h"

void MeshLibrary::Clear()
{
	for(int i=0; i< m_MeshTransforms.size(); i++)
		m_MeshTransforms[i].clear();
}
