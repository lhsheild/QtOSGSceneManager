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

	//���ù����ռ�
	QTextStream qin(stdin);
	QTextStream qout(stdout);
	QString path; QString savePath; QString LOD; QString saveName; QString savePathAuto;


	std::wcout << QString("��������б����Ŀ¼").toStdWString() << std::endl;
	qin >> path;
	path = path + "\\";
	savePathAuto = path + "\\output" + "\\";

	// ���Ŀ¼�Ƿ���ڣ������������½�
	QDir dir;
	if (!dir.exists(savePathAuto))
	{
		bool res = dir.mkpath(savePathAuto);
		qDebug() << "�½�Ŀ¼�Ƿ�ɹ�" << res;
	}

	std::wcout << QString("������LOD����").toStdWString() << std::endl;
	qin >> LOD;


	//��ȡ�����ļ�
	OSGSelectFiles osgSelect;
	//��ȡĿ¼�ļ���Ϣ
	QFileInfoList osgbFileInfo = osgSelect.GetFileList(path);
	QStringList selectedOSGBFiles = osgSelect.SelectFileList(osgbFileInfo, LOD);
	QFileInfoList selectedOSGBFileInfo = osgSelect._selectedOSGBFileInfo;
	//ʵ����Viewer
	osg::ref_ptr<osgViewer::Viewer> view = new osgViewer::Viewer();
	//�������ڵ�
	osg::ref_ptr<osg::Group> root = new osg::Group();


	//����ģ��
	for each (QString filename in selectedOSGBFiles)
	{
		std::string qxfilename = filename.toStdString();
		/*std::cout << qxfilename << endl;*/
		root->addChild(osgDB::readNodeFile(qxfilename));
	}

	//�����������������������������
	TextureVisitor tv; VertexVisitor vv;
	root->accept(tv); root->accept(vv);
	//���������
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

	//��ͼӳ���
	std::map<std::string, osg::Image*>imageList = tv.getImages();
	//��ͼӳ��������
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

	////�Ż���������
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
