#pragma once

#include <fcntl.h>
#include <io.h>
#include <stdlib.h>
#include <sys\stat.h>
#include <direct.h>
#include <atlbase.h>

#include <strsafe.h>

#pragma comment(lib, "comdlg32.lib")

#define MAX_SLK_FIELD_NUM	  230
#define MAX_W3O_FIELD_NUM	  1170
#define MAX_TXT_FIELD_NUM  	64

#define OPTIMIZER_FINISH	  100			//≥Ã–Ú∑µªÿ÷µ
#define ERR_FILE_NOT_FOUND	1
#define ERR_FILE_INVALID	  2
#define ERR_JSCRIPT_LOST	  3


#define OPT_UNIT	objectsForOptimizer[0]
#define OPT_ITEM	objectsForOptimizer[1]
#define OPT_DEST	objectsForOptimizer[2]
#define OPT_DOOD	objectsForOptimizer[3]
#define OPT_ABIL	objectsForOptimizer[4]
#define OPT_BUFF	objectsForOptimizer[5]
#define OPT_UPGR	objectsForOptimizer[6]

#define CNV_UNIT	0
#define CNV_ITEM	0
#define CNV_DEST	0
#define CNV_DOOD	1
#define CNV_ABIL	2
#define CNV_BUFF	0
#define CNV_UPGR	3

#ifndef SFMPQ_STATIC
#define SFMPQ_STATIC
#endif

#include "SFmpqapi.h"
#include "MdxBlpOpt.h"
#include "hash.h"
#include "SLKConv.h"
#include "GetInputFile.h"
#include "CWc3MapOptimizerHepler.hpp"

#pragma warning(disable: 4305)
#pragma warning(disable: 4309)
#pragma warning(disable: 4995)
#pragma warning(disable: 4996)
