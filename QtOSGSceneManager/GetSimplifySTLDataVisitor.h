#ifndef GetSimplifySTLDataVisitor_H
#define GetSimplifySTLDataVisitor_H

#include <vector>
#include <osg/NodeVisitor>
#include <osg/Geometry>
#include "FindNodeVisitor.h"

struct Subset
{
	std::vector<float> vertexs;//位置
	std::vector<float> normals;//法向
	std::vector<float> texCoords;//纹理
	std::vector<unsigned int> indices;//索引下标
	std::vector<unsigned int> faceMtrls;//面材质索引
};

class GetSimplifySTLDataVisitor : public osg::NodeVisitor
{
public:
	GetSimplifySTLDataVisitor(Subset &s);
	virtual void apply(osg::Geode& geode);
	osg::Node* GetFirst();
private:
	Subset& dstSubset;

	std::set<osg::Geometry::AttributeBinding> normalBindKinds;
	std::set<osg::PrimitiveSet::Type> pts;
	std::set<GLenum> modes;

	FindNodeVisitor::nodeListType foundNodeList;
};

#endif