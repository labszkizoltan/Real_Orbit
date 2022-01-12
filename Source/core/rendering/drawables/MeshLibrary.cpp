#include "MeshLibrary.h"

void MeshLibrary::Clear()
{
	for(int i=0; i < m_MeshTransforms.size(); i++)
		m_MeshTransforms[i].clear();

	for (int i = 0; i < m_ColourBuffers.size(); i++)
		m_ColourBuffers[i].clear();
}
