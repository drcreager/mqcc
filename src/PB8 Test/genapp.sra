$PBExportHeader$genapp.sra
$PBExportComments$Generated SDI Application Object
forward
global type genapp from application
end type
global transaction sqlca
global dynamicdescriptionarea sqlda
global dynamicstagingarea sqlsa
global error error
global message message
end forward

global type genapp from application
string appname = "genapp"
end type
global genapp genapp

on genapp.create
appname = "genapp"
message = create message
sqlca = create transaction
sqlda = create dynamicdescriptionarea
sqlsa = create dynamicstagingarea
error = create error
end on

on genapp.destroy
destroy( sqlca )
destroy( sqlda )
destroy( sqlsa )
destroy( error )
destroy( message )
end on

event open;//*-----------------------------------------------------------------*/
//*    open:  Application Open Script
//*          1) Instantiate connection service
//*           2) Populate SQLCA and Connect to the database
//*           3) Open main window
//*-----------------------------------------------------------------*/
n_genapp_connectservice lnv_connectserv

lnv_connectserv = Create using "n_genapp_connectservice"

If lnv_connectserv.of_ConnectDB ( ) = 0 Then
	/*  Open Main window  */
	Open ( w_genapp_main )
End If

Destroy lnv_connectserv
end event

event close;//*-----------------------------------------------------------------*/
//*    close:  Application Close Script
//            1) Instantiate connection service
//            2) Disconnect from the database
//*-----------------------------------------------------------------*/
n_genapp_connectservice lnv_connectserv

lnv_connectserv = Create using "n_genapp_connectservice"

lnv_connectserv.of_DisconnectDB ( )

Destroy lnv_connectserv
end event

