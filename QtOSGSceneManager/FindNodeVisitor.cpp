#include "FindNodeVisitor.h"
#include <osg/Geometry>
#include <iostream>

FindNodeVisitor::FindNodeVisitor(const std::string &searchName) :osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), searchForName(searchName) {

}

void FindNodeVisitor::apply(osg::Node &searchNode)
{
	if (searchNode.getName()==searchForName)
	{
		foundNodeList.push_back(&searchNode);
	}
	traverse(searchNode);
}

void FindNodeVisitor::apply(osg::Geode &geode) {
	apply((osg::Node&)geode);
	traverse((osg::Node&)geode);
	int count = geode.getNumDrawables();
	for (int i=0;i<count;i++)
	{
		osg::Geometry* geometry = geode.getDrawable(i)->asGeometry();
		if (!geometry)
			continue;
		osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray());
		int vertexlNum = vertices->size();
		for (int j = 0; j<vertexlNum; j++) {
			std::cout << "v X: " << vertices->at(j).x() << " Y: " << vertices->at(j).y() << " Z: " << vertices->at(j).z() << std::endl;
		}
	}
}

void FindNodeVisitor::apply(osg::Transform &searchNode) {
	apply((osg::Node&)searchNode);
	traverse((osg::Node&)searchNode);
}

void FindNodeVisitor::setNameToFind(const std::string &searchName) {
	searchForName = searchName;
	foundNodeList.clear();
}

osg::Node* FindNodeVisitor::getFirst() {
	return *(foundNodeList.begin());
}