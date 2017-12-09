
#include "stdafx.h"
#include "GeoExporter.h"
#include "../Game/GeoFileFormat.h"

void GeoExporter::doExport(const string& geoFileName)
{
	Tab<IGameNode*> nodes(_scene->GetIGameNodeByType(IGameObject::IGAME_MESH));
	for(int i=0;i<nodes.Count();++i)
		exportNode(nodes[i]);

	ofstream ofs(geoFileName,ios::out | ios::binary);
	if(ofs.good())
	{
		File::GeoFileHeader h;
		h._header=GEO_HEADER;
		h._vxCount=_builder.getVertices().getCount();
		h._triCount=_builder.getIndices().getCount() / 3;

		ofs.write((const char*)&h,sizeof(File::GeoFileHeader));
		ofs.write((const char*)_builder.getVertices().getBuffer(),sizeof(Core::Vx3D)*h._vxCount);
		ofs.write((const char*)_builder.getIndices().getBuffer(),sizeof(unsigned short)*3*h._triCount);
	}
}

void GeoExporter::exportNode(IGameNode* node)
{
	IGameMesh* o=(IGameMesh*)node->GetIGameObject();
	o->SetCreateOptimizedNormalList();
	o->SetUseWeightedNormals();
	o->InitializeData();

	_builder.processMesh(node,o,false);

	node->ReleaseIGameObject();
}
