#include "GetSimplifySTLDataVisitor.h"
#include <windows.h>
#include <osg/Geode>

GetSimplifySTLDataVisitor::GetSimplifySTLDataVisitor(Subset& s) :dstSubset(s), osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
	
};

void GetSimplifySTLDataVisitor::apply(osg::Geode& geode)
{
	unsigned int count = geode.getNumDrawables();
	for (unsigned int i = 0; i < count; i++)
	{
		osg::Geometry* geometry = geode.getDrawable(i)->asGeometry();
		if (!geometry)
			continue;
		// 顶点数据
		osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray());
		int vertexlNum = vertices->size();
		for (int i = 0; i<vertexlNum; i++) {
			dstSubset.vertexs.push_back(vertices->at(i).x());
			dstSubset.vertexs.push_back(vertices->at(i).y());
			dstSubset.vertexs.push_back(vertices->at(i).z());
		}
		// 法向量
		osg::Vec3Array* normals = dynamic_cast<osg::Vec3Array*>(geometry->getNormalArray());
		int normalNum = normals->size();
		for (int i = 0; i<normalNum; i++) {
			dstSubset.normals.push_back(normals->at(i).x());
			dstSubset.normals.push_back(normals->at(i).y());
			dstSubset.normals.push_back(normals->at(i).z());
		}
		normalBindKinds.insert(geometry->getNormalBinding());
		if (osg::Geometry::BIND_PER_VERTEX != geometry->getNormalBinding())
		{
			std::string msg = "未处理的法向量绑定方式";
			MessageBoxA(0, msg.c_str(), "警告", MB_OK);
		}
		// 索引数组
		for (unsigned int j = 0; j < geometry->getNumPrimitiveSets(); ++j)
		{
			osg::PrimitiveSet* ps = geometry->getPrimitiveSet(j);
			if (!ps) continue;
			pts.insert(ps->getType());
			modes.insert(ps->getMode());
			switch (ps->getType())
			{
			case osg::PrimitiveSet::DrawElementsUIntPrimitiveType:
			{
				osg::DrawElementsUInt* deui = dynamic_cast<osg::DrawElementsUInt*>(ps);
				const unsigned indexNum = deui->getNumIndices();
				switch (deui->getMode())
				{
				case osg::PrimitiveSet::TRIANGLES:
				{
					for (unsigned int k = 0; k<indexNum; k++)
					{
						dstSubset.indices.push_back(deui->at(k));
					}
					break;
				}
				default:
				{
					std::string msg = "未处理的绘制模式";
					MessageBoxA(0, msg.c_str(), "警告", MB_OK);
				}
				}
				break;
			}
			case osg::PrimitiveSet::DrawElementsUShortPrimitiveType:
			{
				osg::DrawElementsUShort* de = dynamic_cast<osg::DrawElementsUShort*>(ps);
				const unsigned indexNum = de->getNumIndices();
				switch (de->getMode())
				{
				case osg::PrimitiveSet::TRIANGLES:
				{
					for (unsigned int k = 0; k<indexNum; k++)
					{
						dstSubset.indices.push_back(de->at(k));
					}
					break;
				}
				case osg::PrimitiveSet::TRIANGLE_STRIP:
				{
					for (unsigned int k = 0; k<indexNum - 2; k++)
					{
						//此处索引为何与基数偶数有关，可百度GL_TRIANGLE_STRIP
						if (k % 2 == 0)
						{
							dstSubset.indices.push_back(de->at(k));
							dstSubset.indices.push_back(de->at(k + 1));
							dstSubset.indices.push_back(de->at(k + 2));
						}
						else
						{
							dstSubset.indices.push_back(de->at(k));
							dstSubset.indices.push_back(de->at(k + 2));
							dstSubset.indices.push_back(de->at(k + 1));
						}
					}
					break;
				}
				default:
				{
					std::string msg = "未处理的绘制模式";
					MessageBoxA(0, msg.c_str(), "警告", MB_OK);
				}
				}
				break;
			}
			case osg::PrimitiveSet::DrawArraysPrimitiveType:
			{
				osg::DrawArrays* da = dynamic_cast<osg::DrawArrays*>(ps);
				int first = da->getFirst();
				int count = da->getCount();
				switch (da->getMode())
				{
				case osg::PrimitiveSet::TRIANGLES:
				{
					for (int k = first; k<first + count; k++) {
						dstSubset.indices.push_back(k);
					}
					break;
				}
				case osg::PrimitiveSet::TRIANGLE_STRIP:
				{
					for (int k= first; k<first + count - 2; k++) {
						if (k % 2 == 0)
						{
							dstSubset.indices.push_back(k);
							dstSubset.indices.push_back(k + 1);
							dstSubset.indices.push_back(k + 2);
						}												 
						else										  
						{												 
							dstSubset.indices.push_back(k);
							dstSubset.indices.push_back(k + 2);
							dstSubset.indices.push_back(k + 1);
						}
					}
					break;
				}
				default:
				{
					std::string msg = "未处理的绘制模式";
					MessageBoxA(0, msg.c_str(), "警告", MB_OK);
				}
				}
				break;
			}
			default:
			{
				std::string msg = "未处理的图元类型";
				//MessageBoxA(0, msg.c_str(), "警告", MB_OK);
			}
			}
		}
	}
}

osg::Node* GetSimplifySTLDataVisitor::GetFirst()
{
	return *foundNodeList.begin();
}

