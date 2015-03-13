#include "StdAfx.h"

void rewriteFloats(char*& buffer, int count, int remBytes)
{
  for (; count>0; count--)
  {
    int i = 0;
    for(; i<remBytes; i++)
      buffer[i]=NULL;
    buffer+=4;
  }
}

int copyDataChunk(char*& buffer, int bytesPerData)
{
  buffer+=4;
  int data_count = (int&)*buffer;
  buffer+=4 + data_count*bytesPerData;
  return 8 + data_count*bytesPerData;
}

void rewriteLcdChunk(char*& buffer, int floatsPerData, int remBytes)
{
  buffer+=4;
  int data_count = (int&)*buffer;
  buffer+=4;
  rewriteFloats(buffer, data_count*floatsPerData, remBytes);
}

void processGEOS(char*& buffer) 
{
  char* endMark = buffer+(int&)*buffer+4;
  buffer+=4;
  int read_geos_bytes = 0;
  while (buffer < endMark)
  {
    buffer+=4;
    rewriteLcdChunk(buffer, 3, 2); // VRTX
    rewriteLcdChunk(buffer, 3, 2); // NRMS
    copyDataChunk(buffer, 4); // PTYP
    copyDataChunk(buffer, 4); // PCNT
    copyDataChunk(buffer, 2); // PVTX
    copyDataChunk(buffer, 1); // GNDX
    copyDataChunk(buffer, 4); // MTGC
    copyDataChunk(buffer, 4); // MATS
    buffer+=40;

    int cnt = (int&)*buffer;
    buffer+=4;
    rewriteFloats(buffer, 7*cnt, 2);

    buffer+=4;		// UVAS
    int uvas_count = (int&)*buffer;
    buffer+=4;

    for (; uvas_count>0; uvas_count--)
    {
      buffer+=4;		// UVBS
      int uvbs_count = (int&)*buffer;
      buffer+=4;
      rewriteFloats(buffer, uvbs_count*2, 2);
    }
  }
}

void processKXXX(char*& buffer, int dimensions, int remBytes)
{
  int data_count = (int&)*buffer;
  buffer+=4;
  int lineType = (int&)*buffer;
  int floatsPerData = dimensions*( (lineType>1)?3:1 );
  buffer+=8;
  for (; data_count>0; data_count--)
  {
    buffer+=4;
    rewriteFloats(buffer, floatsPerData, remBytes);
  }
}

void processKXXXList(char*& buffer, int bytesToUse, int remBytes)
{
  char* endMark = buffer+bytesToUse;
  while(buffer<endMark)
  {
    int tag = (int&)*buffer;
    buffer+=4;
    switch(tag)
    {
    case('CSGK'):
    case('RTGK'):
      processKXXX(buffer, 3, 2);
      break;
    case('TRGK'):
      processKXXX(buffer, 4, 2);
      break;
    default:
      return;
    }
  }
}

void processBONE(char*& buffer) 
{
  char* endMark = buffer+(int&)*buffer+4;
  buffer+=4;
  while(buffer < endMark)
  {
    int n = (int&)*buffer;
    buffer+=96;
    processKXXXList(buffer, n-96, 2);
    buffer+=8;
  }
}

void processHELP(char*& buffer)
{
  char* endMark = buffer+(int&)*buffer+4;
  buffer+=4;
  while(buffer < endMark)
  {
    int n = (int&)*buffer;
    buffer+=96;
    processKXXXList(buffer, n-96, 2);
  }
}

void MdxOpt(char* buffer, int size)
{
  char* endMark = buffer+size;
  int tag = 0;
  // Make sure to starts with MDLX
  tag = (int&)*buffer;
  if (tag != 'XLDM')
    return;
  else
    buffer+=4;

  for(;buffer<endMark;)
  {
    tag = (int&)*buffer;
    buffer+=4;
    switch(tag)
    {
    case 'SOEG':
      processGEOS(buffer);
      break;
    case 'ENOB':
      processBONE(buffer);
      break;
    case 'PLEH':
      processHELP(buffer);
      break;
    default:
      buffer += 4+(int&)*buffer;
      break;
    }
  }
}
