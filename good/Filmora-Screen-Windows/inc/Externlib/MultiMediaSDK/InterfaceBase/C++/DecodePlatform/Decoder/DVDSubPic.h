#pragma once

#include "CodecFrame.h"

#include "DemuxParam.h"



typedef void* PDVDSUBPIC;

extern "C"  PDVDSUBPIC __stdcall DVDSubPicCreate(int nMaxWidth, int nMaxHeight, EXTRA_DATA* pExData);
extern "C"  int __stdcall DVDSubPicPutData(PDVDSUBPIC, const MEDIA_BLOCK*);
extern "C"  const SubPicFrame* __stdcall DVDSubPicGetNextFrame(PDVDSUBPIC);
extern "C"  void __stdcall DVDSubPicReset(PDVDSUBPIC);
extern "C"  void __stdcall DVDSubPicClose(PDVDSUBPIC);