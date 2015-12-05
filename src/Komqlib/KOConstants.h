/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/11	@(#) KOConstants.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     KOConstants.h
//  Author:   David Tamkun
//  Written:  December 1998
//  Desc:     KO Error Values and Other Constants. 
//
//  History: 
//  11/10/98 DMT Created original version in CError.h.
//  12/09/98 DMT Renumbered KO-Specific Error Numbers.  Used to start
//               at 3000.  Now starting at 65536.  Also, added 
//               65556 through 65557. 
//  12/10/98 DMT Moved constants from CError.h to KOConstants.h
//  01/10/99 DMT Added additional Constants for KOMQDLQ
//  09/28/99 DRC Added additional Constants
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#define _RunTime_

#ifndef __KOCONSTANTS_H__
#define __KOCONSTANTS_H__

// ATHOME switch suppreses MQ Series calls in DLL and prevents them from
// linking in.  Handy for running a version at home, where you don't 
// have the whole MQ Series client and want to run basic checks or ensure
// a clean compile.
//
//#define ATHOME
#define _ver2_

//      123456789012345678901234567890 
#define	KORC_UNSPECIFIED               65536L
#define	KORC_INVALID_OPEN_OPT		   65537L
#define	KORC_INVALID_Q_NAME			   65538L
#define	KORC_INVALID_BUFFER			   65539L
#define	KORC_BUFFER_TOO_BIG			   65540L
#define	KORC_INVALID_MSG_ID			   65541L
#define	KORC_INVALID_CORREL_ID		   65542L
#define	KORC_INVALID_REPLY_Q_NAME	   65543L
#define	KORC_INVALID_MSG_SIZE		   65544L
#define	KORC_INVALID_EXPIRE			   65545L
#define	KORC_UNUSABLE_QMGR_CONN		   65546L
#define	KORC_NEW_CQUEUE_FAIL		   65547L
#define	KORC_MAX_QUEUES_EXCEEDED	   65548L
#define	KORC_INVALID_QID			   65549L
#define	KORC_INVALID_QID_NOT_OPEN	   65550L
#define	KORC_INVALID_QMGR_NAME		   65551L
#define	KORC_QMGR_NOT_CONNECTED		   65552L
#define	KORC_INVALID_STRING_BUFF	   65553L
#define	KORC_INVALID_STRING_BUFF_SIZE  65554L
#define KORC_QMGR_ALREADY_CONNECTED	   65555L
#define KORC_INVALID_GET_MODE          65556L						//DMT120998
#define KORC_INVALID_WAIT_INTERVAL     65557L						//DMT120998

#define KORC_BUFFER_TOO_SMALL          65558L						//DMT121498
#define KORC_INVALID_MSG_ID_LEN        65559L						//DMT121498
#define KORC_INVALID_CORREL_ID_LEN     65560L						//DMT121498
#define KORC_INVALID_REPLYTOQ_LEN      65561L						//DMT121498
#define KORC_INVALID_FORMAT_LEN        65562L						//DMT121498
#define KORC_RESERVED1                 65563L // Why Gap??          //DMT040299
#define KORC_INVALID_CHARSET           65564L                       //DMT031199
#define KORC_INVALID_PERSIST           65565L                       //DMT040299


// DMT 01/10/99 - #'s starting at 66000 and above are for KOMQDLQ
#define KORC_OPEN_INPUT_FAIL           66000L
#define KORC_NOT_IMPLEMENTED           66001L
#define KORC_LINE_WIDTH_EXCEEDS_BUFFER 66002L
#define KORC_INVALID_BUFFER_SIZE       66003L
#define KORC_MEMORY_ALLOC_FAIL         66004L
#define KORC_PARM_COLL_GETVALUE_FAILED 66005L
#define KORC_UNEXPECTED_MSG_TYPE       66006L
#define KORC_ROLLBACK_DUE_TO_PREV_ERR  66007L
#define KORC_OPEN_OUTPUT_FAIL          66008L
#define KORC_WRITE_TO_OUTPUT_FAIL      66009L
#define KORC_MISSING_COMMAND_PARMS     66010L
#define KORC_NONNUMERIC_CMD_PARM       66011L
#define KORC_INTERNAL_ERR_CMD_PARM_GAP 66012L
#define KORC_NONNUMERIC_PARM           66013L
#define KORC_INTERNAL_ERR_CMD_FLAG_BAD 66014L
#define KORC_MISSING_PARMS             66015L
#define KORC_INTERNAL_ERR_MAX_PARMS    66016L
#define KORC_INTERNAL_ERR_PARM_NOT_FND 66017L
#define KORC_INTERNAL_ERR_POSIT_PRM_NF 66018L
#define KORC_PARM_FLAG_NOT_FOUND       66019L
#define KORC_INTERNAL_ERR_BAD_FLAG     66020L
#define KORC_INTERNAL_ERR_MQAPI_NOTREG 66021L
#define KORC_PARM_SYNTAX_ERROR         66022L
// DMT 01/13/99 Added Below
#define KORC_INPUT_READ_FAIL_NOT_OPEN  66023L
#define KORC_INVALID_PARM_FILE_WIDTH   66024L

//---------------------------------------------------------------------+
// DMT 03/26/99
// Starting Error #'s at 66100 and above for restructured code.
//---------------------------------------------------------------------+
#define KORC_INVALID_USER_MODE         66100L
#define KORC_INVALID_APP_MODE          66101L
#define KORC_INVALID_APP_USER_MODE     66102L
#define KORC_INVALID_BATCH_INTERVAL    66103L
#define KORC_INVALID_ARGUMENT          66104L
#define KORC_INVALID_MSG_COUNT         66105L
#define KORC_INVALID_MSG_TYPE          66106L
#define KORC_FILE_READ_ERROR           66107L
#define KORC_LINE_ENDS_AT_END_OF_FILE  66108L
#define KORC_FILE_WRITE_ERROR          66109L
#define KORC_RESTART_REPOSITION_FAILED 66110L
#define KORC_FILE_OPEN_ERROR           66111L
#define KORC_FILE_CLOSE_ERROR          66112L
#define KORC_FILE_NAME_NOT_PROVIDED    66113L
#define KORC_NO_CONTROL_MSG            66114L
#define KORC_COMMIT_FAILED             66115L
#define KORC_BATCH_OUT_OF_BALANCE      66116L
#define KORC_INTERNAL_ERROR            66117L
#define KORC_ERROR_FORMATTING_TIMESTMP 66118L
#define KORC_NULL_LOG_POINTER          66119L
#define KORC_NULL_QM_POINTER           66120L
#define KORC_QUEUE_ERROR               66121L
#define KORC_NULL_BUFFER_POINTER       66122L
#define KORC_INVALID_FILE_NAME         66123L
#define KORC_INVALID_MSG_FORMAT        66124L
#define KORC_INVLD_BATCH_INTVL_RESTART 66125L
#define KORC_NO_DATA                   66126L
#define KORC_ADMIN_ACCESS_DENIED       66127L
#define KORC_INVALID_PARM_COMBINATION  66128L
#define KORC_INVALID_OPERATION		   66129L
#define KORC_PROCESSOR_ERROR		   66130L
#define KORC_RESOURCE_ERROR			   66131L
#define KORC_EMPTY_QUEUE                 100L
#define KORC_MYSTERY_VALUE             25600L


//---------------------------------------------------------------------+
// Various KO Constants
//---------------------------------------------------------------------+
#define KO_MAX_WAIT_INTERVAL		   15000			//DMT121498
#define KO_MAX_EXPIRY                  25920000L		//DMT031299
#define KO_SEVEN_DAY_EXPIRATION        6048000L             //DRC011000

// Max Message size based on ISO site standard 
//     2MB less 4,000 bytes IBM reserves for Msg Header.
#define KO_MAX_MSG_SIZE                2093152L	
#define KO_MAX_BUFFER_SIZE             KO_MAX_MSG_SIZE + 1 	//DMT032199
#define KO_RR_BUFFER_SIZE              100                  //DRC011000

// Valid Message Formats.  MQ Series field is 8 bytes, but we'll let
// user leave off the trailing spaces.
#define KOFMT_STRING                   "MQSTR"
#define KOFMT8_STRING                  "MQSTR   "

#define KOFMT_SAP                      "MQHSAP"
#define KOFMT8_SAP                     "MQHSAP  "
#define KORRR_STRING                   "None"


#define KODEFAULT_STRING_SIZE		   160
#define KO_DEFAULT_EXPIRY              KO_MAX_EXPIRY // 30 days
#define KO_DEFAULT_WAIT_INTERVAL       5000L	     // 5 seconds
#define KO_DEFAULT_PRIORITY            5l            // middle of range
#define KO_RR_PRIORITY                 2l            // Higher priority
#define KO_DEFAULT_PERSISTENCE         1l		     //MQPER_PERSISTENT
#define KOFMT_DEFAULT                  KOFMT8_STRING //MQFMT_STRING
#define KO_DEFAULT_MSG_TYPE            8		     //MQMT_DATAGRAM
#define KO_DEFAULT_RETURN_RECEIPT      KORRR_STRING  // DRC 09/28/99

#define KO_USE_DEFAULT_EXPIRY          -2L
#define KO_USE_DEFAULT_WAIT_INTERVAL   -2L
#define KO_USE_DEFAULT_PRIORITY        -2L
#define KO_USE_DEFAULT_PERSISTENCE     -2L
#define KO_USE_DEFAULT_FORMAT          "        "	// 8 spaces
#define KO_USE_DEFAULT_MSG_TYPE        -2L



// KO Completion Codes - which happen to coincide with 
// MQ Series Completion Codes.
//
#define KOCC_GOOD                      0
#define KOCC_WARNING                   1
#define KOCC_FAIL                      2


//
//  Parameter Id's for the Get/Set Attribute Functions
//
enum {
   NoAttrib,            // No Attribute specified
   RestartKey,          // Restart Key
   CCSID,               // Coded Character Set Id
   Encoding,            // Numeric Encoding Scheme
   LogReceipt,          // Log Receipt Requested
   DataReceipt          // Data Receipt Requested
};

//
//  Response Data Id's
//
enum {
   OS,                 // Env:Operation System
   Host,               // Env:Host Name
   Job,                // Env:Job Name
   CCVersion,          // Env:Common Code Version
   Operation,          // ExecPrm:Processing Operation
   Mode,               // ExecPrm:Processing Mode 
   Option,             // ExecPrm:Processing Option
   ParmFileName,       // ExecPrm:Parameter File Name
   BatchIntvl,         // FilePrm:Batch Interval
   ExpireIntvl,        // FilePrm:Expiration Interval
   MsgId,              // FilePrm:Message Id Header
   BufSize,            // FilePrm:IO Buffer Size
   QueueMgr,           // FilePrm:Queue Manager Name
   DataQueue,          // FilePrm:Data Queue Name
   CntlQueue,          // FilePrm:Control Queue Name
   ReplyTo,            // FilePrm:ReplyTo Queue Name
   FailOnQuiesce,      // FilePrm:FailOnQuiesce Indicator
   AutoBalance,        // FilePrm:Auto Balancing Indicator
   CommitmentControl,  // FilePrm:Commitment Control Indicator
   Id,                 // Result:Batch (Message) Id
   DateTime,           // Result:Run Date and Time
   CntlTrgt,           // Result:Target count for Balancing
   SkipMsgs,           // Attribute Definition
   DataMsgs,           // Attribute Definition
   CntlMsgs,           // Attribute Definition
   SynchPts,           // Attribute Definition
   Error,              // Attribute Definition
   FeedBack,           // Attribute Definition
   CompCode            // Attribute Definition
};

//
//  Extended Services Utility Operations
//
enum {
	NoOpr,
	PutFiles,          
	GetFiles,             
	UpdateFile
};
#endif
