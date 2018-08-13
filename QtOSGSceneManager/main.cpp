#if defined(_MSC_VER) && (_MSC_VER >= 1600)  
# pragma execution_character_set("utf-8")  
#endif

#include <QtCore/QCoreApplication>
#include "TextureVisitor.h"
#include "VertexVisitor.h"
#include "OSGSelectFiles.h"
#include "OSGBFile.h"

#include <osgDB/readfile>
#include <osgDB/writefile>
#include <osgViewer/Viewer>
#include <osgUtil/Optimizer>

#include <fstream>
#include<iostream>
#include <io.h>
#include "GetSimplifySTLDataVisitor.h"
#include "VisitorNode.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	VisitorNode vn;

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->setName("root");
	osg::ref_ptr<osg::Group> group = new osg::Group;
	root->setName("group");
	osg::ref_ptr<osg::Group> group1 = new osg::Group;
	root->setName("group1");
	osg::ref_ptr<osg::Node> glider = osgDB::readNodeFile("glider.osg");
	glider->setName("glider");
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");
	cow->setName("cow");
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->setMatrix(osg::Matrix::translate(osg::Vec3(30, 0, 0)) * osg::Matrix::scale(osg::Vec3(0.2, 0.2, 0.2)));
	mt->setName("mt");
	osg::ref_ptr<osg::Switch> switchs = new osg::Switch;
	switchs->setName("switchs");


	//组织节点结构
	root->addChild(group);
	root->addChild(switchs);
	group->addChild(mt);
	group->addChild(cow);
	switchs->addChild(group1);
	group1->addChild(glider);
	mt->addChild(cow);
	mt->addChild(glider);

	//设置遍历器
	root->accept(vn);

	viewer->setSceneData(root);

	viewer->run();

		//std::vector<>
		//for (int i =0;i<s.vertexs.size();i++)
		//{
		//	fout<<"v"<<" "<<
		//}
		//for (int i = 0; i<s.vertexs.size(); i++)
		//{
		//	//std::cout << s.vertexs.at(i) << std::endl;
		//}

	

	/*2018-8-13
	setlocale(LC_CTYPE, "chs");

	//设置工作空间
	QTextStream qin(stdin);
	QTextStream qout(stdout);
	QString path; QString savePath; QString LOD; QString saveName; QString savePathAuto;



	std::wcout << QString("请输入倾斜数据目录").toStdWString() << std::endl;
	qin >> path;
	path = path + "\\";

	std::wcout << QString("请输入LOD级别").toStdWString() << std::endl;
	qin >> LOD;

	savePathAuto = path + "\\"+LOD+"output" + "\\";


	//获取所需文件
	OSGSelectFiles osgSelect;
	//获取目录文件信息
	QFileInfoList osgbFileInfo = osgSelect.GetFileList(path);
	QStringList selectedOSGBFiles = osgSelect.SelectFileList(osgbFileInfo, LOD);
	QFileInfoList selectedOSGBFileInfo = osgSelect._selectedOSGBFileInfo;

	//分目录获取文件信息
	QVector<OSGBFile*> OSGBFileVector = osgSelect.selectFileinfoByFolder;

	//实例化Viewer
	osg::ref_ptr<osgViewer::Viewer> view = new osgViewer::Viewer();
	//创建根节点
	osg::ref_ptr<osg::Group> root = new osg::Group();


	//载入模型
	for each (QString filename in selectedOSGBFiles)
	{
		std::string qxfilename = filename.toStdString();
		std::cout << qxfilename + "has been loaded!"<< endl;
		root->addChild(osgDB::readNodeFile(qxfilename));
	}

	//创建纹理、顶点访问器并启动访问器
	TextureVisitor tv; VertexVisitor vv;
	root->accept(tv); root->accept(vv);
	*/

	/*申请输出流
	std::fstream vvfout("vv.txt");
	std::fstream tvfout("tv.txt");
	//获取顶点数
	int size_t = vv.extracted_verts->size();
	//初始化顶点迭代器
	std::vector<osg::Vec3>::iterator vviter = vv.extracted_verts->begin();
	//写入顶点文件
	for (int i = 0; i < size_t; i++)
	{
		if (vvfout.is_open())
		{
			vvfout << vviter->x() << "	" << vviter->y() << "	" << vviter->z() << std::endl;
		}
		vviter++;
	}
	*/

	///2018-8-13
	/*
	QDir dir;
	if (!dir.exists(savePathAuto))
	{
		bool res = dir.mkpath(savePathAuto);
		qDebug() << "新建目录是否成功" << res;
	}

	//贴图映射表
	std::map<std::string, osg::Image*>imageList = tv.getImages();
	//贴图映射表迭代器
	std::map<std::string, osg::Image*>::iterator iter = imageList.begin();



	std::vector<std::string> nameList;

	for (; iter != imageList.end(); iter++)
	{
		nameList.push_back(iter->first);
		osgDB::writeImageFile(*(iter->second), savePathAuto.toStdString() + iter->first);

		//sprintf(buffer, "Tile_+122_-045_L19%d.jpg", cnt++);
		//tvfout << iter->first<<"  "<<iter->second << std::endl;		
	}

	QString saveNameFirst; QString saveNameLast;
	saveNameFirst = QString::fromStdString(nameList.front());
	saveNameLast = QString::fromStdString(nameList.back());

	osgDB::writeNodeFile(*root.get(), savePathAuto.toStdString() + saveNameFirst.left(14).toStdString() + "_To_" + saveNameLast.left(14).toStdString() + ".obj");

	////优化场景数据
	//osgUtil::Optimizer optimizer;
	//optimizer.optimize(root.get());

	//view->setSceneData(root.get());
	view->setSceneData(osgDB::readNodeFile(savePathAuto.toStdString() + saveNameFirst.left(14).toStdString() + "_To_" + saveNameLast.left(14).toStdString() + ".obj"));
	view->setUpViewInWindow(300, 300, 1920, 1080);
	view->realize();
	view->run();

	getchar();
	*/




	return a.exec();
}
