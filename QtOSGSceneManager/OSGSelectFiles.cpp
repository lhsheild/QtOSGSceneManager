#include "OSGSelectFiles.h"
#include "OSGBFile.h"
//��ȡĿ¼���ļ���Ϣ�б�
QFileInfoList OSGSelectFiles::GetFileList(QString path)
{
	QDir dir(path);
	QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	//������ͬ�ļ��е��ļ��б�
	for (int i = 0; i < folder_list.size(); i++)
	{
		OSGBFile* osgbf = new OSGBFile();
		osgbf->name = folder_list.at(i).fileName();
		QString name = folder_list.at(i).absoluteFilePath();
		osgbf->_OSGBFileInfo_list = GetFileList(name);
		selectFileinfoByFolder.append(osgbf);
	}

	for (int i = 0; i != folder_list.size(); i++)
	{
		QString name = folder_list.at(i).absoluteFilePath();
		QFileInfoList child_file_list = GetFileList(name);
		file_list.append(child_file_list);
	}

	return file_list;
}

QStringList osgbName;
//�����б�ѡȡ�����ļ���
QStringList OSGSelectFiles::SelectFileList(QFileInfoList file_info_list, QString LOD) {
	for each (QFileInfo fileinfo in file_info_list)
	{
		QString delfile = fileinfo.absoluteFilePath();
		QString filter = fileinfo.fileName();   
		QString filter2 = fileinfo.suffix(); //��׺��(!filter.indexOf("L18"))&&
		
		if ((!filter.contains(LOD, Qt::CaseInsensitive)) || (filter2 != "osgb"))//!filter.startsWith(title)) || , QString title
		{
			QFile f;
			//f.remove(delfile);
			//qDebug() << "Delete: " + filter;
			continue;
		}
		_osgbFileList.append(fileinfo.absoluteFilePath());
		_selectedOSGBFileInfo.append(fileinfo);
	}
	return _osgbFileList;
};


