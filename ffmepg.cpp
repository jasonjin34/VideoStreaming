#include "ffmepg.h"
#include <QDebug>
#include <QLibrary>
#include <Windows.h>


namespace iaw {

FFmepg::FFmepg()
{

}

void FFmepg::initialize()
{
    SetDllDirectory(L"/HIWI/VideoStreamingVideostreaming");

    QLibrary ffmepg("avformat-58.dll");
    bool status;
    status = ffmepg.load();

    if(status)
    {
        qDebug() << "successful";
    }
    else {
        qDebug() << "fail";
    }

}


}
