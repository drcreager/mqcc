/*******************************************************************************
 * scyexit.c - Source file for MQSeries C DLL scyexit.dll                      *
 *                                                                             *
 * Description                                                                 *
 * -----------                                                                 *
 *    SCYEXIT is used for SVRCONN / CLNTCONN chanel security exits to exchange *
 *    user logon information:                                                  *
 *    1. Client initiates a security message containing user network logon ID  *
 *       and workstation ID;                                                   *
 *    2. Server receives the security message and overwrites the MCAUSER, and  *
 *       records the user and workstation IDs;                                 *
 *    3. Starts message transmission with agreement;                           *
 *    4. Terminates channels.                                                  *
 *                                                                             *
 *    This program will run on Windows NT or 95 MQSeries client, and NT or     *
 *    AIX server.                                                              *
 *                                                                             *
 *******************************************************************************
 * Compile/Link                                                                *
 * ------------                                                                *
 * NT/95: icc -DNT /Ge-m+ scyexit.c scyexit.def -l mpr.lib advapi32.lib        *
 *    
 *  scyexit.def file:                                                          *
 *  LIBRARY scyexit                                                            *
 *  DATA    MULTIPLE                                                           *
 *  EXPORTS                                                                    *
 *     _SecurityExit                                                           *
 *     ?MQStart                                                                *
 *  or:                                                                        *
 *  before compiling be sure that VCVAR32.BAT was run during the visual c insta*
 *  cl /DNT /MT /LD scyexit.c scyexit.def  /link mpr.lib advapi32.lib          *
 *  scyexit.def file:                                                          *
 *  LIBRARY scyexit                                                            *
 *  DATA MULTIPLE                                                              *
 *  EXPORTS SecurityExit                                                       *
 *                                                                             *
 * AIX: cc -c scyexit.c                                                        *
 *  ld -o scyexit scyexit.o -bE:scyexit.exp -H512 -T512 -e MQStart -bM:SRE -lc *
 *                                                                             *cl
 *  scyexit.exp file:                                                          *
 *  #!                                                                         *
 *  SecurityExit                                                               *
 *  MQStart                                                                    *
 *                                                                             *
 *******************************************************************************
 * How to set up the exit                                                      *
 * ----------------------                                                      *
 *    1. Define server-conn and client-conn channels on server:                *
 *                                                                             *
 *       def chl(ABC) chltype(SVRCONN) +                                       *
 *           scyexit('ServerPath\scyexit(SecurityExit)')                       *
 *                                                                             *
 *       def chl(ABC) chltype(CLNTCONN) conname(SvrAddress) +                  *
 *           scyexit('ClientPath\scyexit(SecurityExit)')                       *
 *                                                                             *
 *    2. Copy AMQCLCHL from server \mqm\qmgrs\@ipcc\AMQCLCHL to client         *
 *                                                                             *
 *    3. Set environment variables on client:                                  *
 *                                                                             *
 *       set MQCHLLIB=PathOfAMQCLCHL.TAB                                       *
 *       set MQCHLTAB=AMQCLCHL.TAB                                             *
 *                                                                             *
 *******************************************************************************                                                                                *
 * Modification History                                                        *
 * --------------------                                                        *
 * 03/07/1999       Initial release                           CJ Feng,IBM      *
 * 04/20/1999       modified logging                          bnb              *
 * 05/06/1999       corrected strncpy for qmgrname            bnb              *
 * 12/22/2000       added security exit version display       drc              *
 *                  added second try for WNetGetUser(NULL)    drc              *
 *                                                                             *
 ******************************************************************************/

#include <cmqc.h>
#include <cmqxc.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef   NT
#include <wtypes.h>
#include <winbase.h>
#include <winnetwk.h>
#endif

#define _NoRemoteAccess_
#define _TempCode01_ 

void MQStart() {;}  /* dummy entry point - UNIX for constsency only */

void MQENTRY SecurityExit (PMQCXP  pChannelExitParms,  /* chl exit parameter block: input/output */
                           PMQCD   pChannelDefinition, /* channel definition: input/output */
                           PMQLONG pDataLength,        /* data length of AgentBuffer: input/output */
                           PMQLONG pAgentBufferLength, /* length of AgentBuffer: input */
                           PMQBYTE pAgentBuffer,       /* agent buffer: input/output */
                           PMQLONG pExitBufferLength,  /* length of ExitBuffer: input/output */
                           PMQPTR  pExitBufferAddr)    /* address of ExitBufferAddr: input/output */
{
   FILE   *fp;
   char   szTimestamp[15];
   time_t ltime;
   char   QMgrName[MQ_Q_MGR_NAME_LENGTH];
   char   ChName[MQ_CHANNEL_NAME_LENGTH];
   int    i;
/*   char   *file;   */

#ifdef NT
   CHAR  szUserInfo[256];
   DWORD cchBuff = 256;

   fp = fopen("c:\\mqclient\\scyexit.log", "a");
    
#else
   fp = fopen("/tmp/scyexit.log", "a");
#endif

   switch (pChannelExitParms->ExitReason)
   {
      case MQXR_INIT:
         ltime = time(NULL);
         strftime(szTimestamp, sizeof(szTimestamp), "%Y%m%d%H%M%S", localtime(&ltime));
      #ifdef _RemoteAccess_
         fprintf(fp, "\n%s  TCCC Security Exit (1.00.00.03) Disconnected\n", szTimestamp);
      #else
         fprintf(fp, "\n%s  TCCC Security Exit (1.00.00.03)\n", szTimestamp);
      #endif
         fprintf(fp, "                ExitReason: MCA initialization\n");
         pChannelExitParms->ExitReason = MQXCC_OK;
         break;

      case MQXR_TERM:
         ltime = time(NULL);
         strftime(szTimestamp, sizeof(szTimestamp), "%Y%m%d%H%M%S", localtime(&ltime));
         fprintf(fp, "%s  ExitReason: MCA termination\n", szTimestamp);
         pChannelExitParms->ExitReason = MQXCC_OK;
         break;

      case MQXR_INIT_SEC:
         ltime = time(NULL);
         strftime(szTimestamp, sizeof(szTimestamp), "%Y%m%d%H%M%S", localtime(&ltime));
         fprintf(fp, "%s  ExitReason: Initiate security exchange\n", szTimestamp);
         switch (pChannelDefinition->ChannelType)
         {
#ifdef NT
            case MQCHT_CLNTCONN:
               strcpy(pAgentBuffer, "\0");
            #ifdef _RemoteAccess_
			   if (strlen(pAgentBuffer) <= 0)
			   {
				  strcpy(pAgentBuffer,"C: z8364a ");
			   }
			#else
			  #ifdef _1_0_0_2_
               if (WNetGetUser("C:", (LPSTR) szUserInfo, &cchBuff) == NO_ERROR )
               {
                  strcat(pAgentBuffer, "C: ");
                  strcat(pAgentBuffer, szUserInfo);
                  strcat(pAgentBuffer, " ");
               }
			  #else
               if (WNetGetUser(NULL, (LPSTR) szUserInfo, &cchBuff) == NO_ERROR )
               {
                  #ifdef _TempCode01_ 
                  strcat(pAgentBuffer, "C: ");
                  #else
                  strcat(pAgentBuffer, "[NULL] ");
                  #endif
                  strcat(pAgentBuffer, szUserInfo);
                  strcat(pAgentBuffer, " ");
               }
			  #endif
 			#endif
              else
               {
                  switch ( GetLastError() )
                  {
                     case ERROR_NOT_CONNECTED:
                        fprintf(fp, "                WNetGetUser: ERROR_NOT_CONNECTED\n");
                        break;

                     case ERROR_MORE_DATA:
                        fprintf(fp, "                WNetGetUser: ERROR_MORE_DATA\n");
                        break;

                     case ERROR_NO_NETWORK:
                        fprintf(fp, "                WNetGetUser: ERROR_NO_NETWORK\n");
                        break;

                     case ERROR_EXTENDED_ERROR:
                        fprintf(fp, "                WNetGetUser: ERROR_EXTENDED_ERROR\n");
                        break;

                     case ERROR_NO_NET_OR_BAD_PATH:
                        fprintf(fp, "                WNetGetUser: ERROR_NO_NET_OR_BAD_PATH\n");
                        break;

                     default:
                        fprintf(fp, "                WNetGetUser: unknow network error\n");
                        break;
                  } // endswtch
                  pChannelExitParms->ExitResponse = MQXCC_SUPPRESS_FUNCTION;
                  break;
               } /* endif WNetGetUser() error */

            #ifdef _RemoteAccess_
			   strcpy(szUserInfo,"Z8364a");
               if (strlen(szUserInfo) > 0)
               {
                  strcat(pAgentBuffer, szUserInfo);
               }
			#else
               if (GetComputerName((LPSTR) szUserInfo, &cchBuff))
               {
                  strcat(pAgentBuffer, szUserInfo);
               }
			#endif
               else /* GetComputerName() error, let's terminate the exit */
               {
                  pChannelExitParms->ExitResponse = MQXCC_SUPPRESS_FUNCTION;
                  break;
               }

               fprintf(fp, "                Resource, User and Workstation IDs: %s\n", pAgentBuffer);
               fprintf(fp, "                CHL name: %s\n", pChannelDefinition->ChannelName);

               /* pass the user logon information to server */
               *pDataLength = *pAgentBufferLength;
               pChannelExitParms->ExitResponse = MQXCC_SEND_SEC_MSG;
               break;
#endif
            case MQCHT_SVRCONN:
               fprintf(fp, "                Not client initiated security exit.\n");
               break;

            default:
               fprintf(fp, "                MQXR_INIT_SEC: unsupported channel type\n");
               pChannelExitParms->ExitResponse = MQXCC_SUPPRESS_FUNCTION;
               break;
         }
         break;

      case MQXR_SEC_MSG:
         /* fprintf(fp, "                ExitReason: Security message received\n"); */

         
	 switch (pChannelDefinition->ChannelType)
         {
            case MQCHT_SVRCONN:
               /* CLNTCONN initiated exchange have an additional flow back to */
               /* server to indicate end of security exchange                 */
               if (*pDataLength == 0)
               {
                  /* no data sent, security exchange completed with agreement */
                  /* leave exit with ok                                       */
                  pChannelExitParms->ExitReason = MQXCC_OK;
                  break;
               }

               memcpy(QMgrName, pChannelDefinition->QMgrName, MQ_Q_MGR_NAME_LENGTH);
	       /* strip trailing blanks */
	       for (i=MQ_Q_MGR_NAME_LENGTH-1; i>=0; i--)
	       {
	         if (QMgrName[i] != ' ')
	           break;
	       }
	       QMgrName[i+1] = '\0';

               memcpy(ChName, pChannelDefinition->ChannelName, MQ_CHANNEL_NAME_LENGTH);
	       /* strip trailing blanks */
	       for (i=MQ_CHANNEL_NAME_LENGTH-1; i>=0; i--)
	       {
	         if (ChName[i] != ' ')
	           break;
	       }
	       ChName[i+1] = '\0';

               fprintf(fp, "                User and Workstation IDs: %s\n", pAgentBuffer);

               fprintf(fp, "                Qmgrname: %s\n", QMgrName);
               fprintf(fp, "                CHL name: %s\n", ChName);


               strncpy(pChannelDefinition->MCAUserIdentifier, pAgentBuffer, strcspn(pAgentBuffer, " "));

               strcpy(pAgentBuffer, pChannelDefinition->MCAUserIdentifier);
               pChannelExitParms->ExitResponse = MQXCC_SEND_SEC_MSG;

               break;

            case MQCHT_CLNTCONN:
               /* flow user id and password */
               /* fprintf(fp, "                MCAUserIdentifier returned from server: %s\n", pAgentBuffer); */
               #ifdef _TempCode01_ 
               fprintf(fp, "                MCAUserIdentifier returned from server: %s\n", pAgentBuffer); 
               #endif 
               pChannelExitParms->ExitResponse = MQXCC_OK;
               break;

            default:
               fprintf(fp, "                MQXR_SEC_MSG: error - unsupported channel type\n");
               pChannelExitParms->ExitResponse = MQXCC_SUPPRESS_FUNCTION;
               break;
         }
         break;

      default:
         fprintf(fp, "                Unsupported exit reason\n");
         break;
   }  /* end swicth */

   fclose(fp);
   return;
}  /* end SecurityExit() */ 

