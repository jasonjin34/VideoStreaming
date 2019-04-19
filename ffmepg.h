#ifndef FFMEPG_H
#define FFMEPG_H

#include <QString>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
}

namespace iaw {

typedef  void (*av_register_all)(void);

class FFmepg
{
public:
    FFmepg();
    static void initialize();

};

};

#endif // FFMEPG_H
