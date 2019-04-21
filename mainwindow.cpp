#include "mainwindow.h"
#include <sstream>
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLibrary>
#include <Windows.h>
#include <iostream>


extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
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
    typedef int (*av_find_best_stream) (AVFormatContext *ic, enum AVMediaType type, int wanted_stream_nb, int related_stream, AVCodec **decoder_ret, int flags);

    //avcodec functions
    typedef const char* (*avcodec_configuration)(void);
    typedef void (*av_init_packet)(AVPacket *pkt);
    typedef AVCodecContext* (*avcodec_alloc_context3)(const AVCodec* codec);
    typedef AVCodec* (*avcodec_find_decoder)(enum AVCodecID id);
    typedef int (*avcodec_get_context_default3)(AVCodecContext*s, const AVCodec* codec);
    typedef attribute_deprecated int (*avcodec_copy_context)(AVCodecContext*dest, const AVCodecContext*src);
    typedef int (*avcodec_open2)(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);

    //swscale function
    typedef struct SwsContext* (*sws_getContext)(int srcW, int srcH, enum AVPixelFormat srcFormat, int dstW, int dstH, enum AVPixelFormat dstFormat, int flags, SwsFilter *srcFilter, SwsFilter *dstFilter, const double *param);
    typedef int (*sws_scale)(struct SwsContext *c, const uint8_t *const srcSlice[], const int srcStride[], int srcSliceY, int srcSliceH, uint8_t *const dst[], const int dstStride[]);
    typedef struct SwsContext* (*sws_getCachedContext)(	struct SwsContext * 	context,
                                                        int 	srcW,
                                                        int 	srcH,
                                                        enum AVPixelFormat 	srcFormat,
                                                        int 	dstW,
                                                        int 	dstH,
                                                        enum AVPixelFormat 	dstFormat,
                                                        int 	flags,
                                                        SwsFilter * 	srcFilter,
                                                        SwsFilter * 	dstFilter,
                                                        const double * 	param
                                                        );

    //avutil function
    typedef AVFrame (*av_frame_alloc)(void);
    typedef double (*av_q2d)(AVRational a);
    typedef int64_t (*av_rescale_q) (int64_t a, AVRational bq, AVRational cq) av_const;
    typedef const char * (*av_get_pix_fmt_name) (enum AVPixelFormat pix_fmt);

    //set my functions'name
    static av_register_all registerAll;
    static av_read_frame readFrame;
    static avformat_open_input openInput;
    static avformat_close_input closeInput;
    static avformat_find_stream_info findStreamInfo;
    static avformat_network_init initStreamNetwork;
    static avpicture_get_size avpictureGetSize;
    static av_find_best_stream avFindBestStream;

    avformat_alloc_context contextAlloc;
    static avcodec_configuration Configuration;
    static avformat_version avformatVersion;
    static av_init_packet initavPacket;
    static av_read_play avReadplay;
    avcodec_alloc_context3 avcodecAlloccontext;
    static avcodec_find_decoder avCodecfindDecoder;
    static avcodec_get_context_default3 avcodeGetContextDefault3;
    static avcodec_copy_context avcodecCopyContext;
    static avcodec_open2 avcodecOpen;

    static sws_getContext swsGetContext;
    static sws_scale swsScale;
    static sws_getCachedContext swsGetCachedContext;

    static av_frame_alloc avFrameAlloc;
    static av_q2d avQ2d;
    static av_rescale_q avRescaleq;
    static av_get_pix_fmt_name avGet_pix_fmt_name;

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
        avFindBestStream = (av_find_best_stream)avformat.resolve("av_find_best_stream");

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
        swsGetCachedContext= (sws_getCachedContext)swscale.resolve("sws_getCachedContext");

        //assign avutil functions
        avFrameAlloc = (av_frame_alloc)avutil.resolve("av_frame_alloc");
        avQ2d = (av_q2d)avutil.resolve("av_q2d");
        avRescaleq = (av_rescale_q)avutil.resolve("av_rescale_q");
        avGet_pix_fmt_name = (av_get_pix_fmt_name)avutil.resolve("av_get_pix_fmt_name");
        //check avformat version
        QString formatS = QString("the version of avformat: %1").arg(avformatVersion());
        qDebug() << formatS;

        /*********************************get frame image code**********************************/
        registerAll();
        initStreamNetwork();
        int ret;

        //set the url link
        int option = 1; // 1, local video, 2, rtsp protocal
        const char* url;
        if(option == 1) url = "C:/HIWI/VideoStreaming/Videostreaming/testvideo.mp4";
        else url = "rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov";

        // open input file context
        AVFormatContext* inctx = nullptr;
        ret = openInput(&inctx, url, nullptr, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avforamt_open_input(\"" << url << "\"): ret=" << ret;
            qDebug() << "failed";
        }
        // retrive input stream information
        ret = findStreamInfo(inctx, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avformat_find_stream_info: ret=" << ret;
             qDebug() << "failed";
        }

        // find primary video stream
        AVCodec* vcodec = nullptr;
        ret = avFindBestStream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
        if (ret < 0) {
            std::cerr << "fail to av_find_best_stream: ret=" << ret;
             qDebug() << "failed";
        }
        const int vstrm_idx = ret;
        AVStream* vstrm = inctx->streams[vstrm_idx];

        // open video decoder context
        ret = avcodecOpen(vstrm->codec, vcodec, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avcodec_open2: ret=" << ret;
           qDebug() << "failed";
        }

        // print input video stream informataion
        std::cout
            << "infile: " << url << "\n"
            << "format: " << inctx->iformat->name << "\n"
            << "vcodec: " << vcodec->name << "\n"
            << "size:   " << vstrm->codec->width << 'x' << vstrm->codec->height << "\n"
            << "frame:  " << vstrm->nb_frames << "\n"
            << std::flush;

        // initialize sample scaler
        const int dst_width = vstrm->codec->width;
        const int dst_height = vstrm->codec->height;
        const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_BGR24;
        SwsContext* swsctx = swsGetCachedContext(
            nullptr, vstrm->codec->width, vstrm->codec->height, vstrm->codec->pix_fmt,
            dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
        if (!swsctx) {
            std::cerr << "fail to sws_getCachedContext";
            qDebug() << "failed";
        }

        std::cout << "output: " << dst_width << 'x' << dst_height << ',' << avGet_pix_fmt_name(dst_pix_fmt) << std::endl;
        /*

        AVCodecContext* ccontext = nullptr;
        SwsContext *img_convert_ctx;
        // Open the initial context variables that are needed
        AVFormatContext* format_ctx = contextAlloc();
        int video_stream_index;

        AVStream* stream = nullptr;
        int cnt = 0;

        //set the url link
        int option = 2; // 1, local video, 2, rtsp protocal
        const char* url;
        if(option == 1) url = "C:/HIWI/VideoStreaming/Videostreaming/testvideo.mp4";
        else url = "rtsp://184.72.239.149/vod/mp4:BigBuckBunny_115k.mov";

        //check validity of the url
        if (openInput(&format_ctx, url,nullptr, nullptr) != 0) qDebug() << "did not open the input source";
        else {
            qDebug() << "library and video load successfully";
            cnt = openInput(&format_ctx, url,nullptr, nullptr);
        }

        if (findStreamInfo(format_ctx,nullptr) < 0) qDebug() << "No stream info";

        //search video stream
        for (int i = 0; i < format_ctx->nb_streams; i++) {
            if (format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
                video_stream_index = i;
        }

        AVPacket packet;
        initavPacket(&packet);

        //start reading packets from stream and write them to file
        avReadplay(format_ctx);

        //get the codec
        const AVCodec *codec = nullptr;
        codec = avCodecfindDecoder(AV_CODEC_ID_H264);
        if(!codec) qDebug() << "No valid codec";

        ccontext = avcodecAlloccontext(codec);

        //avcodeGetContextDefault3(ccontext,codec);
        avcodecCopyContext(ccontext, format_ctx->streams[video_stream_index]->codec);

        if (avcodecOpen(ccontext, codec, nullptr) < 0) exit(1);
        img_convert_ctx = swsGetContext(ccontext->width, ccontext->height,ccontext->pix_fmt, ccontext->width, ccontext->height, AV_PIX_FMT_RGB24,SWS_BICUBIC, nullptr, nullptr, nullptr);

        //int a = avpictureGetSize(AV_PIX_FMT_BGR24, ccontext->width, ccontext->height);
        //qDebug() << a;

        */
    }
    else {
        qDebug() << "fail load library";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
