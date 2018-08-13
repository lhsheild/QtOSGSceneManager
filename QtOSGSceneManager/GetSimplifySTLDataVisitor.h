#ifndef GetSimplifySTLDataVisitor_H
#define GetSimplifySTLDataVisitor_H

#include <vector>
#include <osg/NodeVisitor>
#include <osg/Geometry>
#include "FindNodeVisitor.h"

struct Subset
{
	std::vector<float> vertexs;//λ��
	std::vector<float> normals;//����
	std::vector<float> texCoords;//����
	std::vector<unsigned int> indices;//�����±�
	std::vector<unsigned int> faceMtrls;//���������
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