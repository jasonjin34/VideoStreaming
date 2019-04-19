#include "mainwindow.h"
#include "ui_mainwindow.h"
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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    static av_register_all registerAll;
    static av_read_frame readFrame;
    static avformat_open_input openInput;
    static avformat_close_input closeInput;
    static avformat_find_stream_info findStreamInfo;
    static avformat_network_init initStreamNetwork;
    avformat_alloc_context contextAlloc;
    static avcodec_configuration Configuration;
    static avformat_version avformatVersion;


    SetDllDirectory(L"C:/HIWI/VideoStreaming/Videostreaming");

    QLibrary avformat("avformat-58.dll");
    QLibrary avcodec("avcodec-58.dll");
    bool status;
    status = avformat.load();


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

        //set the url link
        int option = 1; // 1, local video, 2, rtsp protocal
        const char* url;
        if(option == 1) url = "C:/HIWI/VideoStreaming/Videostreaming/testvideo.mp4";
        else url = "rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov";

        //test result
        if (openInput(&format_ctx, url,nullptr, nullptr) != 0) qDebug() << "did not open the input source";
        else  qDebug() << "successful";

        QString formatS = QString("the version of avformat: %1").arg(avformatVersion());
        qDebug() << formatS;
        qDebug() << Configuration();

    }
    else {
        qDebug() << "fail load library";
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}
