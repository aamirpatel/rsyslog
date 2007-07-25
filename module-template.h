/* module-template.h
 * This header contains macros that can be used to implement the 
 * plumbing of modules. 
 *
 * File begun on 2007-07-25 by RGerhards
 *
 * Copyright 2007 Rainer Gerhards and Adiscon GmbH.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * A copy of the GPL can be found in the file "COPYING" in this distribution.
 */
#ifndef	MODULE_TEMPLATE_H_INCLUDED
#define	MODULE_TEMPLATE_H_INCLUDED 1

/* to following macros are used to generate function headers and standard
 * functionality. It works as follows (described on the sample case of
 * createInstance()):
 *
 * BEGINcreateInstance
 * ... custom variable definitions (on stack) ... (if any)
 * CODESTARTcreateInstance
 * ... custom code ... (if any)
 * ENDcreateInstance
 */

/* createInstance()
 */
#define BEGINcreateInstance \
static rsRetVal createInstance(instanceData **ppData)\
	{\
	rsRetVal iRet = RS_RET_OK; /* store error code here */\
	instanceData *pData; /* use this to point to data elements */

#define CODESTARTcreateInstance \
	if((pData = calloc(1, sizeof(instanceData))) == NULL) {\
		*ppData = NULL;\
		return RS_RET_OUT_OF_MEMORY;\
	}

#define ENDcreateInstance \
	*ppData = pData;\
	return iRet;\
}

/* freeInstance()
 */
#define BEGINfreeInstance \
static rsRetVal freeInstance(selector_t *f, void* pModData)\
{\
	rsRetVal iRet = RS_RET_OK;\
	instanceData *pData;

#define CODESTARTfreeInstance \
	assert(f != NULL);\
	pData = (instanceData*) pModData;

#define ENDfreeInstance \
	if(pData != NULL)\
		free(pData); /* we need to free this in any case */\
	return iRet;\
}

/* isCompatibleWithFeature()
 */
#define BEGINisCompatibleWithFeature \
static rsRetVal isCompatibleWithFeature(syslogFeature eFeat)\
{\
	rsRetVal iRet = RS_RET_INCOMPATIBLE;

#define CODESTARTisCompatibleWithFeature

#define ENDisCompatibleWithFeature \
	return iRet;\
}

/* doAction()
 */
#define BEGINdoAction \
static rsRetVal doAction(selector_t *f)\
{\
	rsRetVal iRet = RS_RET_OK;

#define CODESTARTdoAction \
	assert(f != NULL);

#define ENDdoAction \
	return iRet;\
}

/* parseSelectorAct()
 * Extra comments:
 * try to process a selector action line. Checks if the action
 * applies to this module and, if so, processed it. If not, it
 * is left untouched. The driver will then call another module
 */
#define BEGINparseSelectorAct \
static rsRetVal parseSelectorAct(uchar **pp, selector_t *f, void **ppModData)\
{\
	rsRetVal iRet = RS_RET_OK;\
	instanceData *pModData = NULL;

#define CODESTARTparseSelectorAct \
	assert(pp != NULL);\
	assert(ppModData != NULL);\
	assert(f != NULL);

#define ENDparseSelectorAct \
	return iRet;\
}

/* queryEtryPt()
 */
#define BEGINqueryEtryPt \
static rsRetVal queryEtryPt(uchar *name, rsRetVal (**pEtryPoint)())\
{\
	rsRetVal iRet = RS_RET_OK;

#define CODESTARTqueryEtryPt \
	if((name == NULL) || (pEtryPoint == NULL))\
		return RS_RET_PARAM_ERROR;\
	*pEtryPoint = NULL;

#define ENDqueryEtryPt \
	if(iRet == RS_RET_OK)\
		iRet = (*pEtryPoint == NULL) ? RS_RET_NOT_FOUND : RS_RET_OK;\
	return iRet;\
}

/* the following defintion is the standard block for queryEtryPt for output
 * modules. This can be used if no specific handling (e.g. to cover version
 * differences) is needed.
 */
#define CODEqueryEtryPt_STD_OMOD_QUERIES\
	if(!strcmp((char*) name, "doAction")) {\
		*pEtryPoint = doAction;\
	} else if(!strcmp((char*) name, "parseSelectorAct")) {\
		*pEtryPoint = parseSelectorAct;\
	} else if(!strcmp((char*) name, "isCompatibleWithFeature")) {\
		*pEtryPoint = isCompatibleWithFeature;\
	} else if(!strcmp((char*) name, "freeInstance")) {\
		*pEtryPoint = freeInstance;\
	}

/* modInit()
 * This has an extra parameter, which is the specific name of the modInit
 * function. That is needed for built-in modules, which must have unique
 * names in order to link statically.
 *
 * Extra Comments:
 * initialize the module
 *
 * Later, much more must be done. So far, we only return a pointer
 * to the queryEtryPt() function
 * TODO: do interface version checking & handshaking
 * iIfVersRequeted is the version of the interface specification that the
 * caller would like to see being used. ipIFVersProvided is what we
 * decide to provide.
 */
#define BEGINmodInit(uniqName) \
rsRetVal modInit##uniqName(int iIFVersRequested __attribute__((unused)), int *ipIFVersProvided, rsRetVal (**pQueryEtryPt)())\
{\
	rsRetVal iRet = RS_RET_OK;

#define CODESTARTmodInit \
	if((pQueryEtryPt == NULL) || (ipIFVersProvided == NULL))\
		return RS_RET_PARAM_ERROR;

#define ENDmodInit \
	*pQueryEtryPt = queryEtryPt;\
	return iRet;\
}

/*
 */
#define BEGIN \

#define CODESTART \

#define END \


#endif /* #ifndef MODULE_TEMPLATE_H_INCLUDED */
/*
 * vi:set ai:
 */
