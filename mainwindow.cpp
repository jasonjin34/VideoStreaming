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
    typedef int (*av_read_play)(AVFormatContext*);
    typedef attribute_deprecated int (*avpicture_get_size)(enum AVPixelFormat pix_fmt, int width, int height);

    //avcodec functions
    typedef const char* (*avcodec_configuration)(void);
    typedef void (*av_init_packet)(AVPacket *pkt);
    typedef AVCodecContext (*avcodec_alloc_context3)(const AVCodec* codec);
    typedef AVCodec* (*avcodec_find_decoder)(enum AVCodecID id);
    typedef int (*avcodec_get_context_default3)(AVCodecContext*s, const AVCodec* codec);
    typedef attribute_deprecated int (*avcodec_copy_context)(AVCodecContext*dest, const AVCodecContext*src);
    typedef int (*avcodec_open2)(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);

    //swscale function
    typedef struct SwsContext* (*sws_getContext)(int srcW, int srcH, enum AVPixelFormat srcFormat, int dstW, int dstH, enum AVPixelFormat dstFormat, int flags, SwsFilter *srcFilter, SwsFilter *dstFilter, const double *param);
    typedef int (*sws_scale)(struct SwsContext *c, const uint8_t *const srcSlice[], const int srcStride[], int srcSliceY, int srcSliceH, uint8_t *const dst[], const int dstStride[]);

    //avutil function
    typedef AVFrame (*av_frame_alloc)(void);

    //set my functions'name
    static av_register_all registerAll;
    static av_read_frame readFrame;
    static avformat_open_input openInput;
    static avformat_close_input closeInput;
    static avformat_find_stream_info findStreamInfo;
    static avformat_network_init initStreamNetwork;
    static avpicture_get_size avpictureGetSize;

    avformat_alloc_context contextAlloc;
    static avcodec_configuration Configuration;
    static avformat_version avformatVersion;
    static av_init_packet initavPacket;
    static av_read_play avReadplay;
    static avcodec_alloc_context3 avcodecAlloccontext;
    static avcodec_find_decoder avCodecfindDecoder;
    static avcodec_get_context_default3 avcodeGetContextDefault3;
    static avcodec_copy_context avcodecCopyContext;
    static avcodec_open2 avcodecOpen;

    static sws_getContext swsGetContext;
    static sws_scale swsScale;

    static av_frame_alloc avFrameAlloc;

    SetDllDirectory(L"C:/HIWI/VideoStreaming/Videostreaming");

    QLibrary avformat("avformat-58.dll");
    QLibrary avcodec("avcodec-58.dll");
    QLibrary swscale("swscale-5.dll");
    QLibrary avutil("avutil-56.dll");

    bool status_avformat, status_avcodec, status_swscale, status_avutil;
    status_avformat = avformat.load();
    status_avcodec = avcodec.load();
    status_swscale = swscale.load();
    status_avutil = avutil.load();

    if(status_avformat && status_avcodec && status_swscale && status_avutil)
    {
        //assign avformat functions
        registerAll = static_cast<av_register_all>(avformat.resolve("av_register_all"));
        readFrame = (av_read_frame)avformat.resolve("av_read_frame");
        openInput = (avformat_open_input)avformat.resolve("avformat_open_input");
        closeInput = (avformat_close_input)avformat.resolve("avformat_close_input");
        findStreamInfo = (avformat_find_stream_info)avformat.resolve("avformat_find_stream_info");
        initStreamNetwork = (avformat_network_init)avformat.resolve("avformat_network_init");
        contextAlloc =(avformat_alloc_context)avformat.resolve("avformat_alloc_context");
        avformatVersion = (avformat_version)avformat.resolve("avformat_version");
        avReadplay = (av_read_play)avformat.resolve("av_read_play");
        avpictureGetSize = (avpicture_get_size)avformat.resolve("avpicture_get_size");

        //assign avcodec functions
        Configuration = (avcodec_configuration)avcodec.resolve("avcodec_configuration");
        initavPacket = (av_init_packet)avcodec.resolve("av_init_packet");
        avcodecAlloccontext = (avcodec_alloc_context3)avcodec.resolve("avcodec_alloc_context3");
        avCodecfindDecoder = (avcodec_find_decoder)avcodec.resolve("avcodec_find_decoder");
        avcodeGetContextDefault3 = (avcodec_get_context_default3)avcodec.resolve("avcodec_get_context_default3");
        avcodecCopyContext = (avcodec_copy_context)avcodec.resolve("avcodec_copy_context");
        avcodecOpen = (avcodec_open2)avcodec.resolve("avcodec_open2");

        //assign swscale function
        swsGetContext = (sws_getContext)swscale.resolve("sws_getContext");
        swsScale = (sws_scale)swscale.resolve("sws_scale");

        //assign avutil functions
        avFrameAlloc = (av_frame_alloc)avutil.resolve("av_frame_alloc");

        registerAll();
        initStreamNetwork();

        AVPacket packet;
        initavPacket(&packet);
        AVCodecContext* ccontext;
        SwsContext *img_convert_ctx;
        // Open the initial context variables that are needed
        AVFormatContext* format_ctx = contextAlloc();

        AVStream* stream = nullptr;
        int cnt = 0;

        //set the url link
        int option = 1; // 1, local video, 2, rtsp protocal
        const char* url;
        if(option == 1) url = "C:/HIWI/VideoStreaming/Videostreaming/testvideo.mp4";
        else url = "rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov";

        //test result
        if (openInput(&format_ctx, url,nullptr, nullptr) != 0) qDebug() << "did not open the input source";
        else  qDebug() << "library and video load successfully";

        //start reading packets from stream and write them to file
        avReadplay(format_ctx);

        QString formatS = QString("the version of avformat: %1").arg(avformatVersion());
        qDebug() << formatS;
    }
    else {
        qDebug() << "fail load library";
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}
