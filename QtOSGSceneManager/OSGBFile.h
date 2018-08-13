#pragma once
#include <QString>
#include <qdir.h>
class OSGBFile
{
public:
	OSGBFile();
	~OSGBFile();
	QFileInfoList _OSGBFileInfo_list;
	QString name;
};

