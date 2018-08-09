#if defined(_MSC_VER) && (_MSC_VER >= 1600)  
# pragma execution_character_set("utf-8")  
#endif

#include <QtCore/QCoreApplication>
#include "TextureVisitor.h"
#include "VertexVisitor.h"
#include "OSGSelectFiles.h"

#include <osgDB/readfile>
#include <osgDB/writefile>
#include <osgViewer/Viewer>
#include <osgUtil/Optimizer>

#include <fstream>
#include<iostream>
#include <io.h>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	setlocale(LC_CTYPE, "chs");

	//设置工作空间
	QTextStream qin(stdin);
	QTextStream qout(stdout);
	QString path; QString savePath; QString LOD; QString saveName; QString savePathAuto;


	std::wcout << QString("请输入倾斜数据目录").toStdWString() << std::endl;
	qin >> path;
	path = path + "\\";
	savePathAuto = path + "\\output" + "\\";

	// 检查目录是否存在，若不存在则新建
	QDir dir;
	if (!dir.exists(savePathAuto))
	{
		bool res = dir.mkpath(savePathAuto);
		qDebug() << "新建目录是否成功" << res;
	}

	std::wcout << QString("请输入LOD级别").toStdWString() << std::endl;
	qin >> LOD;


	//获取所需文件
	OSGSelectFiles osgSelect;
	//获取目录文件信息
	QFileInfoList osgbFileInfo = osgSelect.GetFileList(path);
	QStringList selectedOSGBFiles = osgSelect.SelectFileList(osgbFileInfo, LOD);
	QFileInfoList selectedOSGBFileInfo = osgSelect._selectedOSGBFileInfo;
	//实例化Viewer
	osg::ref_ptr<osgViewer::Viewer> view = new osgViewer::Viewer();
	//创建根节点
	osg::ref_ptr<osg::Group> root = new osg::Group();


	//载入模型
	for each (QString filename in selectedOSGBFiles)
	{
		std::string qxfilename = filename.toStdString();
		/*std::cout << qxfilename << endl;*/
		root->addChild(osgDB::readNodeFile(qxfilename));
	}

	//创建纹理、顶点访问器并启动访问器
	TextureVisitor tv; VertexVisitor vv;
	root->accept(tv); root->accept(vv);
	//申请输出流
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

	//贴图映射表
	std::map<std::string, osg::Image*>imageList = tv.getImages();
	//贴图映射表迭代器
	std::map<std::string, osg::Image*>::iterator iter = imageList.begin();

	/*unsigned int cnt = 0; char* buffer = new char[2000];*/

	std::vector<std::string> nameList;

	for (; iter != imageList.end(); iter++)
	{
		nameList.push_back(iter->first);
		osgDB::writeImageFile(*(iter->second), savePathAuto.toStdString() + iter->first);

		//sprintf(buffer, "Tile_+122_-045_L19%d.jpg", cnt++);
		tvfout << iter->first<<"  "<<iter->second << std::endl;		
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
	return a.exec();
}
