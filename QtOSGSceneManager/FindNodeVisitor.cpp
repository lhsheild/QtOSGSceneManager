#include "FindNodeVisitor.h"

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