#include "StdAfx.h"

#define BLP_MAX_MIPMAPS 16

struct BLP_HEADER
{
  UINT32 nBlpFlag;
  UINT32 nType;
  UINT32 nFlag;
  UINT32 nWidth;
  UINT32 nHeight;
  UINT32 nPicType;
  UINT32 nPicSubType;
  UINT32 nOffset[BLP_MAX_MIPMAPS];
  UINT32 nSize[BLP_MAX_MIPMAPS];
};

int BlpOpt(char* buffer, int fileSize, char mipNum)
{
  if ((mipNum < 1) || (mipNum > 16))
    mipNum = 16;
  BLP_HEADER* BlpHeader = (BLP_HEADER*)buffer;

  if (BlpHeader->nBlpFlag != '1PLB')
    return 0;

  if (BlpHeader->nFlag == 7)
  {
      mipNum = 16;
  }

  UINT32 minOffset = 0x7fffffff;
  UINT32 maxOffse = 0;

  for(int i = 0; i < 16; i++)
  {
    if (i < mipNum)
    {
      if (BlpHeader->nOffset[i] < minOffset && BlpHeader->nOffset[i] != 0)
      {
        minOffset = BlpHeader->nOffset[i];
      }

      if ((BlpHeader->nOffset[i]+BlpHeader->nSize[i]) > fileSize)
      {
        BlpHeader->nOffset[i] = 0;
        BlpHeader->nSize[i] = 0;
      }
      else if ((BlpHeader->nOffset[i]+BlpHeader->nSize[i]) > maxOffse)
        maxOffse = BlpHeader->nOffset[i]+BlpHeader->nSize[i];
    }
    else
    {
      BlpHeader->nOffset[i] = 0;
      BlpHeader->nSize[i] = 0;
    }
  }
  char* str = buffer + sizeof(BLP_HEADER);

  if (BlpHeader->nType)
  {
    if (BlpHeader->nFlag == 0)
      str += 0x200;
    else
      str += 0x400;
  }
  else
    str += *(PUINT32)(str)+sizeof(UINT32);

  if (minOffset > sizeof(BLP_HEADER)+0x400)
  {
    minOffset = sizeof(BLP_HEADER)+0x400;
  }

  for(int i = 0; (i < mipNum) && (BlpHeader->nOffset[i] > 0); i++)
  {
    if (minOffset + BlpHeader->nSize[i] <= maxOffse)
    {
      UINT32 nOldOffset = BlpHeader->nOffset[i];
      BlpHeader->nOffset[i] = minOffset;
      
      memcpy(buffer+minOffset, buffer+nOldOffset, BlpHeader->nSize[i]);
      
      minOffset = minOffset + BlpHeader->nSize[i];
    }
    else
    {
      BlpHeader->nOffset[i] = 0;
      BlpHeader->nSize[i]   = 0;
    }
  }

  if (BlpHeader->nFlag == 7)
  {
      for(int i = 1; i <= 8; i++)
      {
          if (BlpHeader->nSize[i] == 0 || BlpHeader->nOffset[i] == 0)
          {
              BlpHeader->nSize[i]   = BlpHeader->nSize[i-1];
              BlpHeader->nOffset[i] = BlpHeader->nOffset[i-1];
          }
      }
  }

  return minOffset;
}
