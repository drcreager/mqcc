/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/11	@(#) CError.C 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     CError.cpp
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     This file contains the implementation of the CError
//            Class.
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//  01/14/99 CAH Added Copy Constructor and Assignment Operator
//---------------------------------------------------------------------+
#include "CError.h"



//---------------------------------------------------------------------+
// Static Error Table used to resolve Error Codes to Names for
// Error Reporting.  As new Constants are added to KOConstants.h, they
// should be added here as well if the error an occur in the API.
//
// The table includes MQ Series and KO error codes.
//
// Note that MQ Series repeated code 2120.
//
// *** The table's last Error must have an Error # of -1.  This is how
// *** the code knows it's at the end (kind of like NULL for strings).
//---------------------------------------------------------------------+
CError::SErrorTable CError::sErrorTable[] = 
{
	// Error Number                   String Description (30 bytes)
	{MQRC_NONE,                      "MQRC_NONE"                     }, // 0L
	{MQRC_ALIAS_BASE_Q_TYPE_ERROR,   "MQRC_ALIAS_BASE_Q_TYPE_ERROR"  }, // 2001L
    {MQRC_ALREADY_CONNECTED,         "MQRC_ALREADY_CONNECTED"        }, // 2002L
    {MQRC_BACKED_OUT,                "MQRC_BACKED_OUT"               }, // 2003L
     {MQRC_BUFFER_ERROR,              "MQRC_BUFFER_ERROR"             }, // 2004L
     {MQRC_BUFFER_LENGTH_ERROR,       "MQRC_BUFFER_LENGTH_ERROR"      }, // 2005L
     {MQRC_CHAR_ATTR_LENGTH_ERROR,    "MQRC_CHAR_ATTR_LENGTH_ERROR"   }, // 2006L
     {MQRC_CHAR_ATTRS_ERROR,          "MQRC_CHAR_ATTRS_ERROR"         }, // 2007L
     {MQRC_CHAR_ATTRS_TOO_SHORT,      "MQRC_CHAR_ATTRS_TOO_SHORT"     }, // 2008L
     {MQRC_CONNECTION_BROKEN,         "MQRC_CONNECTION_BROKEN"        }, // 2009L
     {MQRC_DATA_LENGTH_ERROR,         "MQRC_DATA_LENGTH_ERROR"        }, // 2010L
     {MQRC_DYNAMIC_Q_NAME_ERROR,      "MQRC_DYNAMIC_Q_NAME_ERROR"     }, // 2011L
     {MQRC_ENVIRONMENT_ERROR,         "MQRC_ENVIRONMENT_ERROR"        }, // 2012L
     {MQRC_EXPIRY_ERROR,              "MQRC_EXPIRY_ERROR"             }, // 2013L
     {MQRC_FEEDBACK_ERROR,            "MQRC_FEEDBACK_ERROR"           }, // 2014L
     {MQRC_GET_INHIBITED,             "MQRC_GET_INHIBITED"            }, // 2016L
     {MQRC_HANDLE_NOT_AVAILABLE,      "MQRC_HANDLE_NOT_AVAILABLE"     }, // 2017L
     {MQRC_HCONN_ERROR,               "MQRC_HCONN_ERROR"              }, // 2018L
     {MQRC_HOBJ_ERROR,                "MQRC_HOBJ_ERROR"               }, // 2019L
     {MQRC_INHIBIT_VALUE_ERROR,       "MQRC_INHIBIT_VALUE_ERROR"      }, // 2020L
     {MQRC_INT_ATTR_COUNT_ERROR,      "MQRC_INT_ATTR_COUNT_ERROR"     }, // 2021L
     {MQRC_INT_ATTR_COUNT_TOO_SMALL,  "MQRC_INT_ATTR_COUNT_TOO_SMALL" }, // 2022L
     {MQRC_INT_ATTRS_ARRAY_ERROR,     "MQRC_INT_ATTRS_ARRAY_ERROR"    }, // 2023L
     {MQRC_SYNCPOINT_LIMIT_REACHED,   "MQRC_SYNCPOINT_LIMIT_REACHED"  }, // 2024L
     {MQRC_MAX_CONNS_LIMIT_REACHED,   "MQRC_MAX_CONNS_LIMIT_REACHED"  }, // 2025L
     {MQRC_MD_ERROR,                  "MQRC_MD_ERROR"                 }, // 2026L
     {MQRC_MISSING_REPLY_TO_Q,        "MQRC_MISSING_REPLY_TO_Q"       }, // 2027L
     {MQRC_MSG_TYPE_ERROR,            "MQRC_MSG_TYPE_ERROR"           }, // 2029L
     {MQRC_MSG_TOO_BIG_FOR_Q,         "MQRC_MSG_TOO_BIG_FOR_Q"        }, // 2030L
     {MQRC_MSG_TOO_BIG_FOR_Q_MGR,     "MQRC_MSG_TOO_BIG_FOR_Q_MGR"    }, // 2031L
     {MQRC_NO_MSG_AVAILABLE,          "MQRC_NO_MSG_AVAILABLE"         }, // 2033L
     {MQRC_NO_MSG_UNDER_CURSOR,       "MQRC_NO_MSG_UNDER_CURSOR"      }, // 2034L
     {MQRC_NOT_AUTHORIZED,            "MQRC_NOT_AUTHORIZED"           }, // 2035L
     {MQRC_NOT_OPEN_FOR_BROWSE,       "MQRC_NOT_OPEN_FOR_BROWSE"      }, // 2036L
     {MQRC_NOT_OPEN_FOR_INPUT,        "MQRC_NOT_OPEN_FOR_INPUT"       }, // 2037L
     {MQRC_NOT_OPEN_FOR_INQUIRE,      "MQRC_NOT_OPEN_FOR_INQUIRE"     }, // 2038L
     {MQRC_NOT_OPEN_FOR_OUTPUT,       "MQRC_NOT_OPEN_FOR_OUTPUT"      }, // 2039L
     {MQRC_NOT_OPEN_FOR_SET,          "MQRC_NOT_OPEN_FOR_SET"         }, // 2040L
     {MQRC_OBJECT_CHANGED,            "MQRC_OBJECT_CHANGED"           }, // 2041L
     {MQRC_OBJECT_IN_USE,             "MQRC_OBJECT_IN_USE"            }, // 2042L
     {MQRC_OBJECT_TYPE_ERROR,         "MQRC_OBJECT_TYPE_ERROR"        }, // 2043L
     {MQRC_OD_ERROR,                  "MQRC_OD_ERROR"                 }, // 2044L
     {MQRC_OPTION_NOT_VALID_FOR_TYPE, "MQRC_OPTION_NOT_VALID_FOR_TYPE"}, // 2045L
     {MQRC_OPTIONS_ERROR,             "MQRC_OPTIONS_ERROR"            }, // 2046L
     {MQRC_PERSISTENCE_ERROR,         "MQRC_PERSISTENCE_ERROR"        }, // 2047L
     {MQRC_PERSISTENT_NOT_ALLOWED,    "MQRC_PERSISTENT_NOT_ALLOWED"   }, // 2048L
     {MQRC_PRIORITY_EXCEEDS_MAXIMUM,  "MQRC_PRIORITY_EXCEEDS_MAXIMUM" }, // 2049L
     {MQRC_PRIORITY_ERROR,            "MQRC_PRIORITY_ERROR"           }, // 2050L
     {MQRC_PUT_INHIBITED,             "MQRC_PUT_INHIBITED"            }, // 2051L
     {MQRC_Q_DELETED,                 "MQRC_Q_DELETED"                }, // 2052L
     {MQRC_Q_FULL,                    "MQRC_Q_FULL"                   }, // 2053L
     {MQRC_Q_NOT_EMPTY,               "MQRC_Q_NOT_EMPTY"              }, // 2055L
     {MQRC_Q_SPACE_NOT_AVAILABLE,     "MQRC_Q_SPACE_NOT_AVAILABLE"    }, // 2056L
     {MQRC_Q_TYPE_ERROR,              "MQRC_Q_TYPE_ERROR"             }, // 2057L
     {MQRC_Q_MGR_NAME_ERROR,          "MQRC_Q_MGR_NAME_ERROR"         }, // 2058L
     {MQRC_Q_MGR_NOT_AVAILABLE,       "MQRC_Q_MGR_NOT_AVAILABLE"      }, // 2059L
     {MQRC_REPORT_OPTIONS_ERROR,      "MQRC_REPORT_OPTIONS_ERROR"     }, // 2061L
     {MQRC_SECOND_MARK_NOT_ALLOWED,   "MQRC_SECOND_MARK_NOT_ALLOWED"  }, // 2062L
     {MQRC_SECURITY_ERROR,            "MQRC_SECURITY_ERROR"           }, // 2063L
     {MQRC_SELECTOR_COUNT_ERROR,      "MQRC_SELECTOR_COUNT_ERROR"     }, // 2065L
     {MQRC_SELECTOR_LIMIT_EXCEEDED,   "MQRC_SELECTOR_LIMIT_EXCEEDED"  }, // 2066L
     {MQRC_SELECTOR_ERROR,            "MQRC_SELECTOR_ERROR"           }, // 2067L
     {MQRC_SELECTOR_NOT_FOR_TYPE,     "MQRC_SELECTOR_NOT_FOR_TYPE"    }, // 2068L
     {MQRC_SIGNAL_OUTSTANDING,        "MQRC_SIGNAL_OUTSTANDING"       }, // 2069L
     {MQRC_SIGNAL_REQUEST_ACCEPTED,   "MQRC_SIGNAL_REQUEST_ACCEPTED"  }, // 2070L
     {MQRC_STORAGE_NOT_AVAILABLE,     "MQRC_STORAGE_NOT_AVAILABLE"    }, // 2071L
     {MQRC_SYNCPOINT_NOT_AVAILABLE,   "MQRC_SYNCPOINT_NOT_AVAILABLE"  }, // 2072L
     {MQRC_TRIGGER_CONTROL_ERROR,     "MQRC_TRIGGER_CONTROL_ERROR"    }, // 2075L
     {MQRC_TRIGGER_DEPTH_ERROR,       "MQRC_TRIGGER_DEPTH_ERROR"      }, // 2076L
     {MQRC_TRIGGER_MSG_PRIORITY_ERR,  "MQRC_TRIGGER_MSG_PRIORITY_ERR" }, // 2077L
     {MQRC_TRIGGER_TYPE_ERROR,        "MQRC_TRIGGER_TYPE_ERROR"       }, // 2078L
     {MQRC_TRUNCATED_MSG_ACCEPTED,    "MQRC_TRUNCATED_MSG_ACCEPTED"   }, // 2079L
     {MQRC_TRUNCATED_MSG_FAILED,      "MQRC_TRUNCATED_MSG_FAILED"     }, // 2080L
     {MQRC_UNKNOWN_ALIAS_BASE_Q,      "MQRC_UNKNOWN_ALIAS_BASE_Q"     }, // 2082L
     {MQRC_UNKNOWN_OBJECT_NAME,       "MQRC_UNKNOWN_OBJECT_NAME"      }, // 2085L
     {MQRC_UNKNOWN_OBJECT_Q_MGR,      "MQRC_UNKNOWN_OBJECT_Q_MGR"     }, // 2086L
     {MQRC_UNKNOWN_REMOTE_Q_MGR,      "MQRC_UNKNOWN_REMOTE_Q_MGR"     }, // 2087L
     {MQRC_WAIT_INTERVAL_ERROR,       "MQRC_WAIT_INTERVAL_ERROR"      }, // 2090L
     {MQRC_XMIT_Q_TYPE_ERROR,         "MQRC_XMIT_Q_TYPE_ERROR"        }, // 2091L
     {MQRC_XMIT_Q_USAGE_ERROR,        "MQRC_XMIT_Q_USAGE_ERROR"       }, // 2092L
     {MQRC_NOT_OPEN_FOR_PASS_ALL,     "MQRC_NOT_OPEN_FOR_PASS_ALL"    }, // 2093L
     {MQRC_NOT_OPEN_FOR_PASS_IDENT,   "MQRC_NOT_OPEN_FOR_PASS_IDENT"  }, // 2094L
     {MQRC_NOT_OPEN_FOR_SET_ALL,      "MQRC_NOT_OPEN_FOR_SET_ALL"     }, // 2095L
     {MQRC_NOT_OPEN_FOR_SET_IDENT,    "MQRC_NOT_OPEN_FOR_SET_IDENT"   }, // 2096L
     {MQRC_CONTEXT_HANDLE_ERROR,      "MQRC_CONTEXT_HANDLE_ERROR"     }, // 2097L
     {MQRC_CONTEXT_NOT_AVAILABLE,     "MQRC_CONTEXT_NOT_AVAILABLE"    }, // 2098L
     {MQRC_SIGNAL1_ERROR,             "MQRC_SIGNAL1_ERROR"            }, // 2099L
     {MQRC_OBJECT_ALREADY_EXISTS,     "MQRC_OBJECT_ALREADY_EXISTS"    }, // 2100L
     {MQRC_OBJECT_DAMAGED,            "MQRC_OBJECT_DAMAGED"           }, // 2101L
     {MQRC_RESOURCE_PROBLEM,          "MQRC_RESOURCE_PROBLEM"         }, // 2102L
     {MQRC_ANOTHER_Q_MGR_CONNECTED,   "MQRC_ANOTHER_Q_MGR_CONNECTED"  }, // 2103L
     {MQRC_UNKNOWN_REPORT_OPTION,     "MQRC_UNKNOWN_REPORT_OPTION"    }, // 2104L
     {MQRC_STORAGE_CLASS_ERROR,       "MQRC_STORAGE_CLASS_ERROR"      }, // 2105L
     {MQRC_COD_NOT_VALID_FOR_XCF_Q,   "MQRC_COD_NOT_VALID_FOR_XCF_Q"  }, // 2106L
     {MQRC_XWAIT_CANCELED,            "MQRC_XWAIT_CANCELED"           }, // 2107L
     {MQRC_XWAIT_ERROR,               "MQRC_XWAIT_ERROR"              }, // 2108L
     {MQRC_SUPPRESSED_BY_EXIT,        "MQRC_SUPPRESSED_BY_EXIT"       }, // 2109L
     {MQRC_FORMAT_ERROR,              "MQRC_FORMAT_ERROR"             }, // 2110L
     {MQRC_SOURCE_CCSID_ERROR,        "MQRC_SOURCE_CCSID_ERROR"       }, // 2111L
     {MQRC_SOURCE_INTEGER_ENC_ERROR,  "MQRC_SOURCE_INTEGER_ENC_ERROR" }, // 2112L
     {MQRC_SOURCE_DECIMAL_ENC_ERROR,  "MQRC_SOURCE_DECIMAL_ENC_ERROR" }, // 2113L
     {MQRC_SOURCE_FLOAT_ENC_ERROR,    "MQRC_SOURCE_FLOAT_ENC_ERROR"   }, // 2114L
     {MQRC_TARGET_CCSID_ERROR,        "MQRC_TARGET_CCSID_ERROR"       }, // 2115L
     {MQRC_TARGET_INTEGER_ENC_ERROR,  "MQRC_TARGET_INTEGER_ENC_ERROR" }, // 2116L
     {MQRC_TARGET_DECIMAL_ENC_ERROR,  "MQRC_TARGET_DECIMAL_ENC_ERROR" }, // 2117L
     {MQRC_TARGET_FLOAT_ENC_ERROR,    "MQRC_TARGET_FLOAT_ENC_ERROR"   }, // 2118L
     {MQRC_NOT_CONVERTED,             "MQRC_NOT_CONVERTED"            }, // 2119L
//   {MQRC_CONVERTED_MSG_TOO_BIG,     "MQRC_CONVERTED_MSG_TOO_BIG"    }, // 2120L
     {MQRC_TRUNCATED,                 "MQRC_TRUNCATED"                }, // 2120L
     {MQRC_NO_EXTERNAL_PARTICIPANTS,  "MQRC_NO_EXTERNAL_PARTICIPANTS" }, // 2121L
     {MQRC_PARTICIPANT_NOT_AVAILABLE, "MQRC_PARTICIPANT_NOT_AVAILABLE"}, // 2122L
     {MQRC_OUTCOME_MIXED,             "MQRC_OUTCOME_MIXED"            }, // 2123L
     {MQRC_OUTCOME_PENDING,           "MQRC_OUTCOME_PENDING"          }, // 2124L
     {MQRC_BRIDGE_STARTED,            "MQRC_BRIDGE_STARTED"           }, // 2125L
     {MQRC_BRIDGE_STOPPED,            "MQRC_BRIDGE_STOPPED"           }, // 2126L
     {MQRC_ADAPTER_STORAGE_SHORTAGE,  "MQRC_ADAPTER_STORAGE_SHORTAGE" }, // 2127L
     {MQRC_UOW_IN_PROGRESS,           "MQRC_UOW_IN_PROGRESS"          }, // 2128L
     {MQRC_ADAPTER_CONN_LOAD_ERROR,   "MQRC_ADAPTER_CONN_LOAD_ERROR"  }, // 2129L
     {MQRC_ADAPTER_SERV_LOAD_ERROR,   "MQRC_ADAPTER_SERV_LOAD_ERROR"  }, // 2130L
     {MQRC_ADAPTER_DEFS_ERROR,        "MQRC_ADAPTER_DEFS_ERROR"       }, // 2131L
     {MQRC_ADAPTER_DEFS_LOAD_ERROR,   "MQRC_ADAPTER_DEFS_LOAD_ERROR"  }, // 2132L
     {MQRC_ADAPTER_CONV_LOAD_ERROR,   "MQRC_ADAPTER_CONV_LOAD_ERROR"  }, // 2133L
     {MQRC_BO_ERROR,                  "MQRC_BO_ERROR"                 }, // 2134L
     {MQRC_DH_ERROR,                  "MQRC_DH_ERROR"                 }, // 2135L
     {MQRC_MULTIPLE_REASONS,          "MQRC_MULTIPLE_REASONS"         }, // 2136L
     {MQRC_OPEN_FAILED,               "MQRC_OPEN_FAILED"              }, // 2137L
     {MQRC_ADAPTER_DISC_LOAD_ERROR,   "MQRC_ADAPTER_DISC_LOAD_ERROR"  }, // 2138L
     {MQRC_CNO_ERROR,                 "MQRC_CNO_ERROR"                }, // 2139L
     {MQRC_CICS_WAIT_FAILED,          "MQRC_CICS_WAIT_FAILED"         }, // 2140L
     {MQRC_DLH_ERROR,                 "MQRC_DLH_ERROR"                }, // 2141L
     {MQRC_HEADER_ERROR,              "MQRC_HEADER_ERROR"             }, // 2142L
     {MQRC_SOURCE_LENGTH_ERROR,       "MQRC_SOURCE_LENGTH_ERROR"      }, // 2143L
     {MQRC_TARGET_LENGTH_ERROR,       "MQRC_TARGET_LENGTH_ERROR"      }, // 2144L
     {MQRC_SOURCE_BUFFER_ERROR,       "MQRC_SOURCE_BUFFER_ERROR"      }, // 2145L
     {MQRC_TARGET_BUFFER_ERROR,       "MQRC_TARGET_BUFFER_ERROR"      }, // 2146L
     {MQRC_IIH_ERROR,                 "MQRC_IIH_ERROR"                }, // 2148L
     {MQRC_PCF_ERROR,                 "MQRC_PCF_ERROR"                }, // 2149L
     {MQRC_DBCS_ERROR,                "MQRC_DBCS_ERROR"               }, // 2150L
     {MQRC_OBJECT_NAME_ERROR,         "MQRC_OBJECT_NAME_ERROR"        }, // 2152L
     {MQRC_OBJECT_Q_MGR_NAME_ERROR,   "MQRC_OBJECT_Q_MGR_NAME_ERROR"  }, // 2153L
     {MQRC_RECS_PRESENT_ERROR,        "MQRC_RECS_PRESENT_ERROR"       }, // 2154L
     {MQRC_OBJECT_RECORDS_ERROR,      "MQRC_OBJECT_RECORDS_ERROR"     }, // 2155L
     {MQRC_RESPONSE_RECORDS_ERROR,    "MQRC_RESPONSE_RECORDS_ERROR"   }, // 2156L
     {MQRC_ASID_MISMATCH,             "MQRC_ASID_MISMATCH"            }, // 2157L
     {MQRC_PMO_RECORD_FLAGS_ERROR,    "MQRC_PMO_RECORD_FLAGS_ERROR"   }, // 2158L
     {MQRC_PUT_MSG_RECORDS_ERROR,     "MQRC_PUT_MSG_RECORDS_ERROR"    }, // 2159L
     {MQRC_CONN_ID_IN_USE,            "MQRC_CONN_ID_IN_USE"           }, // 2160L
     {MQRC_Q_MGR_QUIESCING,           "MQRC_Q_MGR_QUIESCING"          }, // 2161L
     {MQRC_Q_MGR_STOPPING,            "MQRC_Q_MGR_STOPPING"           }, // 2162L
     {MQRC_DUPLICATE_RECOV_COORD,     "MQRC_DUPLICATE_RECOV_COORD"    }, // 2163L
     {MQRC_PMO_ERROR,                 "MQRC_PMO_ERROR"                }, // 2173L
     {MQRC_API_EXIT_NOT_FOUND,        "MQRC_API_EXIT_NOT_FOUND"       }, // 2182L
     {MQRC_API_EXIT_LOAD_ERROR,       "MQRC_API_EXIT_LOAD_ERROR"      }, // 2183L
     {MQRC_REMOTE_Q_NAME_ERROR,       "MQRC_REMOTE_Q_NAME_ERROR"      }, // 2184L
     {MQRC_INCONSISTENT_PERSISTENCE,  "MQRC_INCONSISTENT_PERSISTENCE" }, // 2185L
     {MQRC_GMO_ERROR,                 "MQRC_GMO_ERROR"                }, // 2186L
     {MQRC_TMC_ERROR,                 "MQRC_TMC_ERROR"                }, // 2191L
     {MQRC_PAGESET_FULL,              "MQRC_PAGESET_FULL"             }, // 2192L
     {MQRC_PAGESET_ERROR,             "MQRC_PAGESET_ERROR"            }, // 2193L
     {MQRC_NAME_NOT_VALID_FOR_TYPE,   "MQRC_NAME_NOT_VALID_FOR_TYPE"  }, // 2194L
     {MQRC_UNEXPECTED_ERROR,          "MQRC_UNEXPECTED_ERROR"         }, // 2195L
     {MQRC_UNKNOWN_XMIT_Q,            "MQRC_UNKNOWN_XMIT_Q"           }, // 2196L
     {MQRC_UNKNOWN_DEF_XMIT_Q,        "MQRC_UNKNOWN_DEF_XMIT_Q"       }, // 2197L
     {MQRC_DEF_XMIT_Q_TYPE_ERROR,     "MQRC_DEF_XMIT_Q_TYPE_ERROR"    }, // 2198L
     {MQRC_DEF_XMIT_Q_USAGE_ERROR,    "MQRC_DEF_XMIT_Q_USAGE_ERROR"   }, // 2199L
     {MQRC_NAME_IN_USE,               "MQRC_NAME_IN_USE"              }, // 2201L
     {MQRC_CONNECTION_QUIESCING,      "MQRC_CONNECTION_QUIESCING"     }, // 2202L
     {MQRC_CONNECTION_STOPPING,       "MQRC_CONNECTION_STOPPING"      }, // 2203L
     {MQRC_ADAPTER_NOT_AVAILABLE,     "MQRC_ADAPTER_NOT_AVAILABLE"    }, // 2204L
     {MQRC_MSG_ID_ERROR,              "MQRC_MSG_ID_ERROR"             }, // 2206L
     {MQRC_CORREL_ID_ERROR,           "MQRC_CORREL_ID_ERROR"          }, // 2207L
     {MQRC_FILE_SYSTEM_ERROR,         "MQRC_FILE_SYSTEM_ERROR"        }, // 2208L
     {MQRC_NO_MSG_LOCKED,             "MQRC_NO_MSG_LOCKED"            }, // 2209L
     {MQRC_FILE_NOT_AUDITED,          "MQRC_FILE_NOT_AUDITED"         }, // 2216L
     {MQRC_CONNECTION_NOT_AUTHORIZED, "MQRC_CONNECTION_NOT_AUTHORIZED"}, // 2217L
     {MQRC_MSG_TOO_BIG_FOR_CHANNEL,   "MQRC_MSG_TOO_BIG_FOR_CHANNEL"  }, // 2218L
     {MQRC_CALL_IN_PROGRESS,          "MQRC_CALL_IN_PROGRESS"         }, // 2219L
     {MQRC_RMH_ERROR,                 "MQRC_RMH_ERROR"                }, // 2220L
     {MQRC_Q_MGR_ACTIVE,              "MQRC_Q_MGR_ACTIVE"             }, // 2222L
     {MQRC_Q_MGR_NOT_ACTIVE,          "MQRC_Q_MGR_NOT_ACTIVE"         }, // 2223L
     {MQRC_Q_DEPTH_HIGH,              "MQRC_Q_DEPTH_HIGH"             }, // 2224L
     {MQRC_Q_DEPTH_LOW,               "MQRC_Q_DEPTH_LOW"              }, // 2225L
     {MQRC_Q_SERVICE_INTERVAL_HIGH,   "MQRC_Q_SERVICE_INTERVAL_HIGH"  }, // 2226L
     {MQRC_Q_SERVICE_INTERVAL_OK,     "MQRC_Q_SERVICE_INTERVAL_OK"    }, // 2227L
     {MQRC_CHANNEL_AUTO_DEF_OK,       "MQRC_CHANNEL_AUTO_DEF_OK"      }, // 2233L
     {MQRC_CHANNEL_AUTO_DEF_ERROR,    "MQRC_CHANNEL_AUTO_DEF_ERROR"   }, // 2234L
     {MQRC_CFH_ERROR,                 "MQRC_CFH_ERROR"                }, // 2235L
     {MQRC_CFIL_ERROR,                "MQRC_CFIL_ERROR"               }, // 2236L
     {MQRC_CFIN_ERROR,                "MQRC_CFIN_ERROR"               }, // 2237L
     {MQRC_CFSL_ERROR,                "MQRC_CFSL_ERROR"               }, // 2238L
     {MQRC_CFST_ERROR,                "MQRC_CFST_ERROR"               }, // 2239L
     {MQRC_INCOMPLETE_GROUP,          "MQRC_INCOMPLETE_GROUP"         }, // 2241L
     {MQRC_INCOMPLETE_MSG,            "MQRC_INCOMPLETE_MSG"           }, // 2242L
     {MQRC_INCONSISTENT_CCSIDS,       "MQRC_INCONSISTENT_CCSIDS"      }, // 2243L
     {MQRC_INCONSISTENT_ENCODINGS,    "MQRC_INCONSISTENT_ENCODINGS"   }, // 2244L
     {MQRC_INCONSISTENT_UOW,          "MQRC_INCONSISTENT_UOW"         }, // 2245L
     {MQRC_INVALID_MSG_UNDER_CURSOR,  "MQRC_INVALID_MSG_UNDER_CURSOR" }, // 2246L
     {MQRC_MATCH_OPTIONS_ERROR,       "MQRC_MATCH_OPTIONS_ERROR"      }, // 2247L
     {MQRC_MDE_ERROR,                 "MQRC_MDE_ERROR"                }, // 2248L
     {MQRC_MSG_FLAGS_ERROR,           "MQRC_MSG_FLAGS_ERROR"          }, // 2249L
     {MQRC_MSG_SEQ_NUMBER_ERROR,      "MQRC_MSG_SEQ_NUMBER_ERROR"     }, // 2250L
     {MQRC_OFFSET_ERROR,              "MQRC_OFFSET_ERROR"             }, // 2251L
     {MQRC_ORIGINAL_LENGTH_ERROR,     "MQRC_ORIGINAL_LENGTH_ERROR"    }, // 2252L
     {MQRC_SEGMENT_LENGTH_ZERO,       "MQRC_SEGMENT_LENGTH_ZERO"      }, // 2253L
     {MQRC_UOW_NOT_AVAILABLE,         "MQRC_UOW_NOT_AVAILABLE"        }, // 2255L
     {MQRC_WRONG_GMO_VERSION,         "MQRC_WRONG_GMO_VERSION"        }, // 2256L
     {MQRC_WRONG_MD_VERSION,          "MQRC_WRONG_MD_VERSION"         }, // 2257L
     {MQRC_GROUP_ID_ERROR,            "MQRC_GROUP_ID_ERROR"           }, // 2258L
     {MQRC_INCONSISTENT_BROWSE,       "MQRC_INCONSISTENT_BROWSE"      }, // 2259L
     {MQRC_XQH_ERROR,                 "MQRC_XQH_ERROR"                }, // 2260L
     {MQRC_SRC_ENV_ERROR,             "MQRC_SRC_ENV_ERROR"            }, // 2261L
     {MQRC_SRC_NAME_ERROR,            "MQRC_SRC_NAME_ERROR"           }, // 2262L
     {MQRC_DEST_ENV_ERROR,            "MQRC_DEST_ENV_ERROR"           }, // 2263L
     {MQRC_DEST_NAME_ERROR,           "MQRC_DEST_NAME_ERROR"          }, // 2264L
     {MQRC_TM_ERROR,                  "MQRC_TM_ERROR"                 }, // 2265L
     {MQRC_HCONFIG_ERROR,             "MQRC_HCONFIG_ERROR"            }, // 2280L
     {MQRC_FUNCTION_ERROR,            "MQRC_FUNCTION_ERROR"           }, // 2281L
     {MQRC_CHANNEL_STARTED,           "MQRC_CHANNEL_STARTED"          }, // 2282L
     {MQRC_CHANNEL_STOPPED,           "MQRC_CHANNEL_STOPPED"          }, // 2283L
     {MQRC_CHANNEL_CONV_ERROR,        "MQRC_CHANNEL_CONV_ERROR"       }, // 2284L
     {MQRC_SERVICE_NOT_AVAILABLE,     "MQRC_SERVICE_NOT_AVAILABLE"    }, // 2285L
     {MQRC_INITIALIZATION_FAILED,     "MQRC_INITIALIZATION_FAILED"    }, // 2286L
     {MQRC_TERMINATION_FAILED,        "MQRC_TERMINATION_FAILED"       }, // 2287L
     {MQRC_UNKNOWN_Q_NAME,            "MQRC_UNKNOWN_Q_NAME"           }, // 2288L
     {MQRC_SERVICE_ERROR,             "MQRC_SERVICE_ERROR"            }, // 2289L
     {MQRC_Q_ALREADY_EXISTS,          "MQRC_Q_ALREADY_EXISTS"         }, // 2290L
     {MQRC_USER_ID_NOT_AVAILABLE,     "MQRC_USER_ID_NOT_AVAILABLE"    }, // 2291L
     {MQRC_UNKNOWN_ENTITY,            "MQRC_UNKNOWN_ENTITY"           }, // 2292L
     {MQRC_UNKNOWN_AUTH_ENTITY,       "MQRC_UNKNOWN_AUTH_ENTITY"      }, // 2293L
     {MQRC_UNKNOWN_REF_OBJECT,        "MQRC_UNKNOWN_REF_OBJECT"       }, // 2294L
     {MQRC_CHANNEL_ACTIVATED,         "MQRC_CHANNEL_ACTIVATED"        }, // 2295L
     {MQRC_CHANNEL_NOT_ACTIVATED,     "MQRC_CHANNEL_NOT_ACTIVATED"    }, // 2296L
//   {,                               ""                              }, // 
//   {KO MQ API Codes,                "KO MQ API Codes"               }, // 
     {KORC_UNSPECIFIED,               "KORC_UNSPECIFIED"              }, // 65536L
     {KORC_INVALID_OPEN_OPT,          "KORC_INVALID_OPEN_OPT"         }, // 65537L
     {KORC_INVALID_Q_NAME,            "KORC_INVALID_Q_NAME"           }, // 65538L
     {KORC_INVALID_BUFFER,            "KORC_INVALID_BUFFER"           }, // 65539L
     {KORC_BUFFER_TOO_BIG,            "KORC_BUFFER_TOO_BIG"           }, // 65540L
     {KORC_INVALID_MSG_ID,            "KORC_INVALID_MSG_ID"           }, // 65541L
     {KORC_INVALID_CORREL_ID,         "KORC_INVALID_CORREL_ID"        }, // 65542L
     {KORC_INVALID_REPLY_Q_NAME,      "KORC_INVALID_REPLY_Q_NAME"     }, // 65543L
     {KORC_INVALID_MSG_SIZE,          "KORC_INVALID_MSG_SIZE"         }, // 65544L
     {KORC_INVALID_EXPIRE,            "KORC_INVALID_EXPIRE"           }, // 65545L
     {KORC_UNUSABLE_QMGR_CONN,        "KORC_UNUSABLE_QMGR_CONN"       }, // 65546L
     {KORC_NEW_CQUEUE_FAIL,           "KORC_NEW_CQUEUE_FAIL"          }, // 65547L
     {KORC_MAX_QUEUES_EXCEEDED,       "KORC_MAX_QUEUES_EXCEEDED"      }, // 65548L
     {KORC_INVALID_QID,               "KORC_INVALID_QID"              }, // 65549L
     {KORC_INVALID_QID_NOT_OPEN,      "KORC_INVALID_QID_NOT_OPEN"     }, // 65550L
     {KORC_INVALID_QMGR_NAME,         "KORC_INVALID_QMGR_NAME"        }, // 65551L
     {KORC_QMGR_NOT_CONNECTED,        "KORC_QMGR_NOT_CONNECTED"       }, // 65552L
     {KORC_INVALID_STRING_BUFF,       "KORC_INVALID_STRING_BUFF"      }, // 65553L
     {KORC_INVALID_STRING_BUFF_SIZE,  "KORC_INVALID_STRING_BUFF_SIZE" }, // 65554L
     {KORC_QMGR_ALREADY_CONNECTED,    "KORC_QMGR_ALREADY_CONNECTED"   }, // 65555L
     {KORC_INVALID_GET_MODE,          "KORC_INVALID_GET_MODE"         }, // 65556L
     {KORC_INVALID_WAIT_INTERVAL,     "KORC_INVALID_WAIT_INTERVAL"    }, // 65557L
     {KORC_BUFFER_TOO_SMALL,          "KORC_BUFFER_TOO_SMALL"         }, // 65558L
     {KORC_INVALID_MSG_ID_LEN,        "KORC_INVALID_MSG_ID_LEN"       }, // 65559L
     {KORC_INVALID_CORREL_ID_LEN,     "KORC_INVALID_CORREL_ID_LEN"    }, // 65560L
     {KORC_INVALID_REPLYTOQ_LEN,      "KORC_INVALID_REPLYTOQ_LEN"     }, // 65561L
     {KORC_INVALID_FORMAT_LEN,        "KORC_INVALID_FORMAT_LEN"       }, // 65562L
     {KORC_RESERVED1,                 "KORC_RESERVED1"                }, // 65563L
     {KORC_INVALID_CHARSET,           "KORC_INVALID_CHARSET"          }, // 65564L
     {KORC_INVALID_PERSIST,           "KORC_INVALID_PERSIST"          }, // 65565L
//   {,                               ""                              }, // 
//   {Old Dead Letter Queue Codes,    "Old Dead Letter Queue Codes"   }, // 
     {KORC_OPEN_INPUT_FAIL,           "KORC_OPEN_INPUT_FAIL"          }, // 66000L
     {KORC_NOT_IMPLEMENTED,           "KORC_NOT_IMPLEMENTED"          }, // 66001L
     {KORC_LINE_WIDTH_EXCEEDS_BUFFER, "KORC_LINE_WIDTH_EXCEEDS_BUFFER"}, // 66002L
     {KORC_INVALID_BUFFER_SIZE,       "KORC_INVALID_BUFFER_SIZE"      }, // 66003L
     {KORC_MEMORY_ALLOC_FAIL,         "KORC_MEMORY_ALLOC_FAIL"        }, // 66004L
     {KORC_PARM_COLL_GETVALUE_FAILED, "KORC_PARM_COLL_GETVALUE_FAILED"}, // 66005L
     {KORC_UNEXPECTED_MSG_TYPE,       "KORC_UNEXPECTED_MSG_TYPE"      }, // 66006L
     {KORC_ROLLBACK_DUE_TO_PREV_ERR,  "KORC_ROLLBACK_DUE_TO_PREV_ERR" }, // 66007L
     {KORC_OPEN_OUTPUT_FAIL,          "KORC_OPEN_OUTPUT_FAIL"         }, // 66008L
     {KORC_WRITE_TO_OUTPUT_FAIL,      "KORC_WRITE_TO_OUTPUT_FAIL"     }, // 66009L
     {KORC_MISSING_COMMAND_PARMS,     "KORC_MISSING_COMMAND_PARMS"    }, // 66010L
     {KORC_NONNUMERIC_CMD_PARM,       "KORC_NONNUMERIC_CMD_PARM"      }, // 66011L
     {KORC_INTERNAL_ERR_CMD_PARM_GAP, "KORC_INTERNAL_ERR_CMD_PARM_GAP"}, // 66012L
     {KORC_NONNUMERIC_PARM,           "KORC_NONNUMERIC_PARM"          }, // 66013L
     {KORC_INTERNAL_ERR_CMD_FLAG_BAD, "KORC_INTERNAL_ERR_CMD_FLAG_BAD"}, // 66014L
     {KORC_MISSING_PARMS,             "KORC_MISSING_PARMS"            }, // 66015L
     {KORC_INTERNAL_ERR_MAX_PARMS,    "KORC_INTERNAL_ERR_MAX_PARMS"   }, // 66016L
     {KORC_INTERNAL_ERR_PARM_NOT_FND, "KORC_INTERNAL_ERR_PARM_NOT_FND"}, // 66017L
     {KORC_INTERNAL_ERR_POSIT_PRM_NF, "KORC_INTERNAL_ERR_POSIT_PRM_NF"}, // 66018L
     {KORC_PARM_FLAG_NOT_FOUND,       "KORC_PARM_FLAG_NOT_FOUND"      }, // 66019L
     {KORC_INTERNAL_ERR_BAD_FLAG,     "KORC_INTERNAL_ERR_BAD_FLAG"    }, // 66020L
     {KORC_INTERNAL_ERR_MQAPI_NOTREG, "KORC_INTERNAL_ERR_MQAPI_NOTREG"}, // 66021L
     {KORC_PARM_SYNTAX_ERROR,         "KORC_PARM_SYNTAX_ERROR"        }, // 66022L
     {KORC_INPUT_READ_FAIL_NOT_OPEN,  "KORC_INPUT_READ_FAIL_NOT_OPEN" }, // 66023L
     {KORC_INVALID_PARM_FILE_WIDTH,   "KORC_INVALID_PARM_FILE_WIDTH"  }, // 66024L
//   {,                               ""                              }, // 
//   {Codes for Restructured Code,    "Codes for Restructured Code"   }, // 
     {KORC_INVALID_USER_MODE,         "KORC_INVALID_USER_MODE"        }, // 66100L
     {KORC_INVALID_APP_MODE,          "KORC_INVALID_APP_MODE"         }, // 66101L
     {KORC_INVALID_APP_USER_MODE,     "KORC_INVALID_APP_USER_MODE"    }, // 66102L
     {KORC_INVALID_BATCH_INTERVAL,    "KORC_INVALID_BATCH_INTERVAL"   }, // 66103L
     {KORC_INVALID_ARGUMENT,          "KORC_INVALID_ARGUMENT"         }, // 66104L
     {KORC_INVALID_MSG_COUNT,         "KORC_INVALID_MSG_COUNT"        }, // 66105L
     {KORC_INVALID_MSG_TYPE,          "KORC_INVALID_MSG_TYPE"         }, // 66106L
     {KORC_FILE_READ_ERROR,           "KORC_FILE_READ_ERROR"          }, // 66107L
     {KORC_LINE_ENDS_AT_END_OF_FILE,  "KORC_LINE_ENDS_AT_END_OF_FILE" }, // 66108L
     {KORC_FILE_WRITE_ERROR,          "KORC_FILE_WRITE_ERROR"         }, // 66109L
     {KORC_RESTART_REPOSITION_FAILED, "KORC_RESTART_REPOSITION_FAILED"}, // 66110L
     {KORC_FILE_OPEN_ERROR,           "KORC_FILE_OPEN_ERROR"          }, // 66111L
     {KORC_FILE_CLOSE_ERROR,          "KORC_FILE_CLOSE_ERROR"         }, // 66112L
     {KORC_FILE_NAME_NOT_PROVIDED,    "KORC_FILE_NAME_NOT_PROVIDED"   }, // 66113L
     {KORC_NO_CONTROL_MSG,            "KORC_NO_CONTROL_MSG"           }, // 66114L
     {KORC_COMMIT_FAILED,             "KORC_COMMIT_FAILED"            }, // 66115L
     {KORC_BATCH_OUT_OF_BALANCE,      "KORC_BATCH_OUT_OF_BALANCE"     }, // 66116L
     {KORC_INTERNAL_ERROR,            "KORC_INTERNAL_ERROR"           }, // 66117L
     {KORC_ERROR_FORMATTING_TIMESTMP, "KORC_ERROR_FORMATTING_TIMESTMP"}, // 66118L
     {KORC_NULL_LOG_POINTER,          "KORC_NULL_LOG_POINTER"         }, // 66119L
     {KORC_NULL_QM_POINTER,           "KORC_NULL_QM_POINTER"          }, // 66120L
     {KORC_QUEUE_ERROR,               "KORC_QUEUE_ERROR"              }, // 66121L
     {KORC_NULL_BUFFER_POINTER,       "KORC_NULL_BUFFER_POINTER"      }, // 66122L
     {KORC_INVALID_FILE_NAME,         "KORC_INVALID_FILE_NAME"        }, // 66123L
     {KORC_INVALID_MSG_FORMAT,        "KORC_INVALID_MSG_FORMAT"       }, // 66124L
     {KORC_INVLD_BATCH_INTVL_RESTART, "KORC_INVLD_BATCH_INTVL_RESTART"}, // 66125L
     {KORC_NO_DATA,                   "KORC_NO_DATA"                  }, // 66126L
     {KORC_ADMIN_ACCESS_DENIED,       "KORC_ADMIN_ACCESS_DENIED"      }, // 66127L

	// *** Last Error Must be -1 to indicate end of table.  ***

	{-1L,	"End of Table"}
};





//---------------------------------------------------------------------+
//  Method:   CError::CError(CError&) - Copy constructor
//  Author:   Chris Hamilton
//  Written:  November 1998
//  Desc:     Copy Constructor
//  Args:     CError&
//  
//  Dev Log:  
//  History: 
//  01/14/99  CAH Created original version.
//  03/31/99  DMT Fixed to allocate new CString class for error
//                context.
//---------------------------------------------------------------------+
CError::CError(CError& d)
{
	is_error_context = new CString(KODEFAULT_STRING_SIZE);

	il_reason_code    = d.il_reason_code ;
	il_comp_code      = d.il_comp_code;
	*is_error_context = d.is_error_context->getPointer();
}



//---------------------------------------------------------------------+
//  Method:   operator =
//  Author:   Chris Hamilton
//  Written:  November 1998
//  Desc:     Assigns values in source to target.
//  Args:     CError&
//  Returns:  CError&
//  
//  Dev Log: 
//  History: 
//  01/14/99  CAH Created original version.
//---------------------------------------------------------------------+
CError& CError::operator=(CError& d)
{
	il_reason_code    = d.il_reason_code ;
	il_comp_code      = d.il_comp_code;
	*is_error_context = d.is_error_context->getPointer();

	return d;
}

//---------------------------------------------------------------------+
//  Method:   Init
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     Does Nothing
//  Args:     None
//  Returns:  0.
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//---------------------------------------------------------------------+
long CError::Init ( void ){

	return 0;

}




//---------------------------------------------------------------------+
//  Method:   GetErrorCodes
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Returns Completion Code and Reason Code
//
//  Args:     ref long - Completion Code
//            ref long - Reason Code
//
//  Returns:  Long, Always 0
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//---------------------------------------------------------------------+
long CError::GetErrorCodes( long& rl_completion_code, long& rl_reason_code ) {

	rl_completion_code = il_comp_code;
	rl_reason_code     = il_reason_code;

	return 0;

}




//---------------------------------------------------------------------+
//  Method:   GetErrorContext
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Returns Error Context String, typically a Class::Method
//            location code.  Usually has some message with it as 
//            well.
//
//  Args:     ref CString, to receive msg.
//
//  Returns:  long - always returns 0
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//---------------------------------------------------------------------+
long CError::GetErrorContext( CString& rs_context ) {

	rs_context = is_error_context;

	return 0;
}



//---------------------------------------------------------------------+
//  Method:   Clear Error Codes
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Resets all error codes to 0 and sets Error Context to
//            an empty string ""
//
//  Args:     None
//
//  Returns:  None
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//---------------------------------------------------------------------+
void CError::ClearErrorCodes( void ) {

	il_reason_code = 0;
	il_comp_code = 0;
	*is_error_context = "";

}



//---------------------------------------------------------------------+
//  Method:   GetReasonCodeName
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Uses a Static Table to return Error Text for the 
//            specified error code.  Used to return descriptive name
//            of MQ Series and KO Reason Codes.
//
//  Args:     long - Reason Code to Look Up
//
//            CString Ptr - Receives Text
//
//  Returns:  Always Returns 0
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//---------------------------------------------------------------------+


long CError::GetReasonCodeName(long vl_reason_code, CString* rs_name ) {

	long	ll_counter = 0;
	char	lpc_string[50];


	while (sErrorTable[ll_counter].lErrorNumber != vl_reason_code &&
		   sErrorTable[ll_counter].lErrorNumber != -1L) {

		ll_counter++;
	}

	if (sErrorTable[ll_counter].lErrorNumber != -1L) {

		// found error

		*rs_name = (char*) sErrorTable[ll_counter].szErrorText;
	}
	else {
		// not found

		sprintf(lpc_string, "Unknown Reason Code (%d)", vl_reason_code);

		*rs_name = lpc_string;
	}

	return 0;
}






//---------------------------------------------------------------------+
//  Method:   CheckError
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     If boolean parameter is NOT TRUE, completion code,
//            reason code, and context string are set.
//
//            NOTE!! A better name for this method would've been
//                   "assert" because the condition we pass is the
//                   we want to be true.  We want an error set if
//                   the condition is NOT true.
//
//  Args:     Check Condition - Condition you want to be TRUE.
//
//            long	Completion Code - value to set if Condition is
//                                    in Error (FALSE).
//
//            long  Reason Code     - value to set if Condition is
//                                    in Error (FALSE).
//
//            char* Error Context   - value to set if Condition is
//                                    in Error (FALSE).
//
//  Returns:  None
//  
//  Dev Log: 
//  History: 
//  11/12/98 DMT Created original version.
//---------------------------------------------------------------------+
void CError::CheckError( BOOL vb_assert, long vl_comp, long vl_reason, 
						char* vs_context) {

	if(!vb_assert) {

		il_comp_code      = vl_comp;
		il_reason_code    = vl_reason;
		*is_error_context = vs_context;

	}

}






//---------------------------------------------------------------------+
//  Method:   TermSelf
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     TBD
//  Args:     TBD
//  Returns:  TBD
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//---------------------------------------------------------------------+
long CError::TermSelf( void ) {

return 0;
}

	


//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//---------------------------------------------------------------------+
CError::CError( void ){

	il_comp_code = 0;
	il_reason_code = 0;
	is_error_context = new CString(KODEFAULT_STRING_SIZE);

}




//---------------------------------------------------------------------+
//  Method:   Destructor 
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
CError::~CError( void ){

	delete is_error_context;
	is_error_context = NULL;

}
