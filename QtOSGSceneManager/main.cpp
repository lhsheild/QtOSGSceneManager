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


	//��֯�ڵ�ṹ
	root->addChild(group);
	root->addChild(switchs);
	group->addChild(mt);
	group->addChild(cow);
	switchs->addChild(group1);
	group1->addChild(glider);
	mt->addChild(cow);
	mt->addChild(glider);

	//���ñ�����
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

	//���ù����ռ�
	QTextStream qin(stdin);
	QTextStream qout(stdout);
	QString path; QString savePath; QString LOD; QString saveName; QString savePathAuto;



	std::wcout << QString("��������б����Ŀ¼").toStdWString() << std::endl;
	qin >> path;
	path = path + "\\";

	std::wcout << QString("������LOD����").toStdWString() << std::endl;
	qin >> LOD;

	savePathAuto = path + "\\"+LOD+"output" + "\\";


	//��ȡ�����ļ�
	OSGSelectFiles osgSelect;
	//��ȡĿ¼�ļ���Ϣ
	QFileInfoList osgbFileInfo = osgSelect.GetFileList(path);
	QStringList selectedOSGBFiles = osgSelect.SelectFileList(osgbFileInfo, LOD);
	QFileInfoList selectedOSGBFileInfo = osgSelect._selectedOSGBFileInfo;

	//��Ŀ¼��ȡ�ļ���Ϣ
	QVector<OSGBFile*> OSGBFileVector = osgSelect.selectFileinfoByFolder;

	//ʵ����Viewer
	osg::ref_ptr<osgViewer::Viewer> view = new osgViewer::Viewer();
	//�������ڵ�
	osg::ref_ptr<osg::Group> root = new osg::Group();


	//����ģ��
	for each (QString filename in selectedOSGBFiles)
	{
		std::string qxfilename = filename.toStdString();
		std::cout << qxfilename + "has been loaded!"<< endl;
		root->addChild(osgDB::readNodeFile(qxfilename));
	}

	//�����������������������������
	TextureVisitor tv; VertexVisitor vv;
	root->accept(tv); root->accept(vv);
	*/

	/*���������
	std::fstream vvfout("vv.txt");
	std::fstream tvfout("tv.txt");
	//��ȡ������
	int size_t = vv.extracted_verts->size();
	//��ʼ�����������
	std::vector<osg::Vec3>::iterator vviter = vv.extracted_verts->begin();
	//д�붥���ļ�
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
		qDebug() << "�½�Ŀ¼�Ƿ�ɹ�" << res;
	}

	//��ͼӳ���
	std::map<std::string, osg::Image*>imageList = tv.getImages();
	//��ͼӳ��������
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

	////�Ż���������
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
