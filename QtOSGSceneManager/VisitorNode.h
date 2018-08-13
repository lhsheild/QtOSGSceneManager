#pragma once
#include <osg/NodeVisitor>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Switch>
#include <iostream>

class VisitorNode:public osg::NodeVisitor
{
public:
	VisitorNode();
	virtual void apply(osg::Node &node);
	virtual void apply(osg::Group &node);
	virtual void apply(osg::MatrixTransform &node);
	virtual void apply(osg::Switch &node);
	virtual void apply(osg::Geode &node);
};

