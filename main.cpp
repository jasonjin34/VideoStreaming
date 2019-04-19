#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QLibrary>
#include <Windows.h>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SetDllDirectory(L"C:/HIWI/VideoStreaming/Videostreaming");

    QLibrary avformat("avformat-58.dll");
    QLibrary avcodec("avcodec-58.dll");
    bool status;
    status = avformat.load();

    //avformat functions
    typedef void (*av_register_all) (void);
    typedef int (*av_read_frame) (AVFormatContext *s, AVPacket* pkt);
    typedef int (*avformat_open_input) (AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);
    typedef void (*avformat_close_input) (AVFormatContext **s);
    typedef int (*avformat_find_stream_info) (AVFormatContext *ic, AVDictionary **options);
    typedef void (*avformat_network_init)(void);
    typedef AVFormatContext* (*avformat_alloc_context)(void);
    typedef unsigned (*avformat_version)(void);


    //avcodec functions
    typedef const char* (*avcodec_configuration)(void);


    //avformat functions
    static av_register_all registerAll;
    static av_read_frame readFrame;
    static avformat_open_input openInput;
    static avformat_close_input closeInput;
    static avformat_find_stream_info findStreamInfo;
    static avformat_network_init initStreamNetwork;
    avformat_alloc_context contextAlloc;
    static avcodec_configuration Configuration;
    static avformat_version avformatVersion;

    if(status)
    {
        registerAll = static_cast<av_register_all>(avformat.resolve("av_register_all"));
        readFrame = (av_read_frame)avformat.resolve("av_read_frame");
        openInput = (avformat_open_input)avformat.resolve("avformat_open_input");
        closeInput = (avformat_close_input)avformat.resolve("avformat_close_input");
        findStreamInfo = (avformat_find_stream_info)avformat.resolve("avformat_find_stream_info");
        initStreamNetwork = (avformat_network_init)avformat.resolve("avformat_network_init");
        contextAlloc =(avformat_alloc_context)avformat.resolve("avformat_alloc_context");
        avformatVersion = (avformat_version)avformat.resolve("avformat_version");

        Configuration = (avcodec_configuration)avcodec.resolve("avcodec_configuration");

        registerAll();
        initStreamNetwork();

        // Open the initial context variables that are needed
        AVFormatContext* format_ctx = contextAlloc();
        if (openInput(&format_ctx, "https://youtu.be/gYs4xC59kqE",
                      nullptr, nullptr) != 0)
        {
            qDebug() << "did not open the input source";
        }

        qDebug() << "successful";
        QString formatS = QString("the version of avformat: %1").arg(avformatVersion());
        qDebug() << formatS;
        qDebug() << Configuration();

    }
    else {
        qDebug() << "fail";
    }

    return a.exec();
}
