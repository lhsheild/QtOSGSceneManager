#include "VisitorNode.h"


VisitorNode::VisitorNode() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {};
	void VisitorNode::apply(osg::Node &node)
	{
		std::cout << "node: " << node.getName() << std::endl;
		traverse(node);
	}
	void VisitorNode::apply(osg::Group &node)
	{
		std::cout << "apply group: " << node.getName() << std::endl;
		traverse(node);
	}
	void VisitorNode::apply(osg::MatrixTransform &node)
	{
		std::cout << "apply MatrixTransform: " << node.getName() << std::endl;
		traverse(node);
	}
	void VisitorNode::apply(osg::Switch &node)
	{
		std::cout << "apply Switch: " << node.getName() << std::endl;
		traverse(node);
	}
	void VisitorNode::apply(osg::Geode &node)
	{
		std::cout << "apply geode: " << node.getName() << std::endl;
		traverse(node);
	}