#include "NLECommon_Const.h"
#include "NLEGraphics.h"


NLEFrameIndex NLECommon::Const::FRAMEINDEX_EVERYWHERE = (NLEFrameIndex)-1;
NLEFrameIndex NLECommon::Const::FRAMEINDEX_MIN = (NLEFrameIndex)0;
NLEFrameIndex NLECommon::Const::FRAMEINDEX_MAX = (NLEFrameIndex)NLE_INFINITY-6;

NLERange NLECommon::Const::RANGE_INFINITY = NLERange(NLECommon::Const::FRAMEINDEX_MIN, NLECommon::Const::FRAMEINDEX_MAX);

INT NLECommon::Const::TIMELINE_LEVEL_MIN = 0;

INT NLECommon::Const::IMAGE_WIDTH_MAX = 9999;
INT NLECommon::Const::IMAGE_HEIGHT_MAX = 9999;


INT NLECommon::Const::AudioEdit::Samplerate = 44100;
INT NLECommon::Const::AudioEdit::Channels = 2;
INT NLECommon::Const::AudioEdit::BitsPerSample = 16;
INT NLECommon::Const::AudioEdit::BytesPerSecond = 176400; //44100 * 2 * 16/8

NLEPointF NLECommon::Const::POINTF_ORIGNAL = NLEPointF(NLEREAL(0.0), NLEREAL(0.0));
NLEPointF NLECommon::Const::POINTF_CENTER = NLEPointF(NLEREAL(0.5), NLEREAL(0.5) );
NLESizeF NLECommon::Const::NO_RESIZE_SCALE = NLESizeF(NLEREAL(1.0), NLEREAL(1.0) );
NLEPointF NLECommon::Const::NO_TRANSLATE = NLEPointF(NLEREAL(0.0), NLEREAL(0.0));
BYTE	NLECommon::Const::NO_TRANSPARENT = 255;
NLERectF NLECommon::Const::FULL_GRAPHICS_RENDER = NLERectF(NLEREAL(0.0), NLEREAL(0.0), NLEREAL(1.0), NLEREAL(1.0) );
NLERectF NLECommon::Const::EMPTY_GRAPHICS_RENDER = NLERectF(NLEREAL(0.0), NLEREAL(0.0), NLEREAL(0.0), NLEREAL(0.0));
float	NLECommon::Const::NO_ROTATION = 0.0f;
NLERational NLECommon::Const::NORMAL_SPEED = NLERational(1,1);
NLESize		NLECommon::Const::DEFAULT_RENDER_SIZE = NLESize(960,540);
UINT		NLECommon::Const::TASKID_INVALID = 0;

#define MAKECOLOR(a, r, g, b) (((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff))

UINT NLECommon::Const::Color::BLACK_TRANSPARENT = MAKECOLOR(0, 0, 0, 0);
UINT NLECommon::Const::Color::BLACK_OPAQUE = MAKECOLOR(255, 0, 0, 0);

UINT NLECommon::Const::Color::RED_TRANSPARENT = MAKECOLOR(0, 255, 0, 0);
UINT NLECommon::Const::Color::RED_OPAQUE = MAKECOLOR(255, 255, 0, 0);

UINT NLECommon::Const::Color::GREEN_TRANSPARENT = MAKECOLOR(0, 0, 255, 0);
UINT NLECommon::Const::Color::GREEN_OPAQUE = MAKECOLOR(255, 0, 255, 0);

UINT NLECommon::Const::Color::BLUE_TRANSPARENT = MAKECOLOR(0, 0, 0, 255);
UINT NLECommon::Const::Color::BLUE_OPAQUE = MAKECOLOR(255, 0, 0, 255);
