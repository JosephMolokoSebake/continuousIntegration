//---------------------------------------------------------------------------
#include "jpch.h"

#pragma hdrstop

#include "RASErrorDef.h"
//#include <raserror.h>
#include <ras.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

String sGetRASError(const DWORD dwError)
{

   char szError[256];
   ::RasGetErrorString(dwError,szError,sizeof(szError));
   return String(szError);
/*

   switch(dwError)
      {
		case PENDING:
			return "An operation is pending.";
		case ERROR_INVALID_PORT_HANDLE:
			return "An invalid port handle was detected.";
		case ERROR_PORT_ALREADY_OPEN:
			return "The specified port is already open.";
		case ERROR_BUFFER_TOO_SMALL:
			return "The caller's buffer is too small.";
		case ERROR_WRONG_INFO_SPECIFIED:
			return "Incorrect information was specified.";
		case ERROR_CANNOT_SET_PORT_INFO:
			return "The port information cannot be set.";
		case ERROR_PORT_NOT_CONNECTED:
			return "The specified port is not connected.";
		case ERROR_EVENT_INVALID:
			return "An invalid event was detected.";
		case ERROR_DEVICE_DOES_NOT_EXIST:
			return "A device was specified that does not exist.";
		case ERROR_DEVICETYPE_DOES_NOT_EXIST:
			return "A device type was specified that does not exist.";
		case ERROR_BUFFER_INVALID:
			return "An invalid buffer was specified.";
		case ERROR_ROUTE_NOT_AVAILABLE:
			return "A route was specified that is not available.";
		case ERROR_ROUTE_NOT_ALLOCATED:
			return "A route was specified that is not allocated.";
		case ERROR_INVALID_COMPRESSION_SPECIFIED:
			return "An invalid compression was specified.";
		case ERROR_OUT_OF_BUFFERS:
			return "There were insufficient buffers available.";
		case ERROR_PORT_NOT_FOUND:
			return "The specified port was not found.";
		case ERROR_ASYNC_REQUEST_PENDING:
			return "An asynchronous request is pending.";
		case ERROR_ALREADY_DISCONNECTING:
			return "The modem (or other connecting device) is already disconnecting.";
		case ERROR_PORT_NOT_OPEN:
			return "The specified port is not open.";
		case ERROR_PORT_DISCONNECTED:
			return "A connection to the remote computer could not be established, so the port used for this connection was closed.";
		case ERROR_NO_ENDPOINTS:
			return "No endpoints could be determined.";
		case ERROR_CANNOT_OPEN_PHONEBOOK:
			return "The system could not open the phone book file.";
		case ERROR_CANNOT_LOAD_PHONEBOOK:
			return "The system could not load the phone book file.";
		case ERROR_CANNOT_FIND_PHONEBOOK_ENTRY:
			return "The system could not find the phone book entry for this connection.";
		case ERROR_CANNOT_WRITE_PHONEBOOK:
			return "The system could not update the phone book file.";
		case ERROR_CORRUPT_PHONEBOOK:
			return "The system found invalid information in the phone book file.";
		case ERROR_CANNOT_LOAD_STRING:
			return "A string could not be loaded.";
		case ERROR_KEY_NOT_FOUND:
			return "A key could not be found.";
		case ERROR_DISCONNECTION:
			return "The connection was terminated by the remote computer before it could be completed.";
		case ERROR_REMOTE_DISCONNECTION:
			return "The connection was closed by the remote computer.";
		case ERROR_HARDWARE_FAILURE:
			return "The modem (or other connecting device) was disconnected due to hardware failure.";
		case ERROR_USER_DISCONNECTION:
			return "The user disconnected the modem (or other connecting device).";
		case ERROR_INVALID_SIZE:
			return "An incorrect structure size was detected.";
		case ERROR_PORT_NOT_AVAILABLE:
			return "The modem (or other connecting device) is already in use or is not configured properly.";
		case ERROR_CANNOT_PROJECT_CLIENT:
			return "Your computer could not be registered on the remote network.";
		case ERROR_UNKNOWN:
			return "There was an unknown error.";
		case ERROR_WRONG_DEVICE_ATTACHED:
			return "The device attached to the port is not the one expected.";
		case ERROR_BAD_STRING:
			return "A string was detected that could not be converted.";
		case ERROR_REQUEST_TIMEOUT:
			return "The request has timed out.";
		case ERROR_CANNOT_GET_LANA:
			return "No asynchronous net is available.";
		case ERROR_NETBIOS_ERROR:
			return "An error has occurred involving NetBIOS.";
		case ERROR_SERVER_OUT_OF_RESOURCES:
			return "The server cannot allocate NetBIOS resources needed to support the client.";
		case ERROR_NAME_EXISTS_ON_NET:
			return "One of your computer's NetBIOS names is already registered on the remote network.";
		case ERROR_SERVER_GENERAL_NET_FAILURE:
			return "A network adapter at the server failed.";
		case WARNING_MSG_ALIAS_NOT_ADDED:
			return "You will not receive network message popups.";
		case ERROR_AUTH_INTERNAL:
			return "There was an internal authentication error.";
		case ERROR_RESTRICTED_LOGON_HOURS:
			return "The account is not permitted to log on at this time of day.";
		case ERROR_ACCT_DISABLED:
			return "The account is disabled.";
		case ERROR_PASSWD_EXPIRED:
			return "The password for this account has expired.";
		case ERROR_NO_DIALIN_PERMISSION:
			return "The account does not have permission to dial in.";
		case ERROR_SERVER_NOT_RESPONDING:
			return "The remote access server is not responding.";
		case ERROR_FROM_DEVICE:
			return "The modem (or other connecting device) has reported an error.";
		case ERROR_UNRECOGNIZED_RESPONSE:
			return "There was an unrecognized response from the modem (or other connecting device).";
		case ERROR_MACRO_NOT_FOUND:
			return "A macro required by the modem (or other connecting device) was not found.";
		case ERROR_MACRO_NOT_DEFINED:
			return "A command or response in the device.INF file section refers to an undefined macro.";
		case ERROR_MESSAGE_MACRO_NOT_FOUND:
			return "The <message> macro was not found in the device.INF file section.";
		case ERROR_DEFAULTOFF_MACRO_NOT_FOUND:
			return "The <defaultoff> macro in the device.INF file section contains an undefined macro.";
		case ERROR_FILE_COULD_NOT_BE_OPENED:
			return "The device.INF file could not be opened.";
		case ERROR_DEVICENAME_TOO_LONG:
			return "The device name in the device.INF or media.INI file is too long.";
		case ERROR_DEVICENAME_NOT_FOUND:
			return "The media.INI file refers to an unknown device name.";
		case ERROR_NO_RESPONSES:
			return "The device.INF file contains no responses for the command.";
		case ERROR_NO_COMMAND_FOUND:
			return "The device.INF file is missing a command.";
		case ERROR_WRONG_KEY_SPECIFIED:
			return "There was an attempt to set a macro not listed in device.INF file section.";
		case ERROR_UNKNOWN_DEVICE_TYPE:
			return "The media.INI file refers to an unknown device type.";
		case ERROR_ALLOCATING_MEMORY:
			return "The system has run out of memory.";
		case ERROR_PORT_NOT_CONFIGURED:
			return "The modem (or other connecting device) is not properly configured.";
		case ERROR_DEVICE_NOT_READY:
			return "The modem (or other connecting device) is not functioning.";
		case ERROR_READING_INI_FILE:
			return "The system was unable to read the media.INI file.";
		case ERROR_NO_CONNECTION:
			return "The connection was terminated.";
		case ERROR_BAD_USAGE_IN_INI_FILE:
			return "The usage parameter in the media.INI file is invalid.";
		case ERROR_READING_SECTIONNAME:
			return "The system was unable to read the section name from the media.INI file.";
		case ERROR_READING_DEVICETYPE:
			return "The system was unable to read the device type from the media.INI file.";
		case ERROR_READING_DEVICENAME:
			return "The system was unable to read the device name from the media.INI file.";
		case ERROR_READING_USAGE:
			return "The system was unable to read the usage from the media.INI file.";
		case ERROR_READING_MAXCONNECTBPS:
			return "The system was unable to read the maximum connection BPS rate from the media.INI file.";
		case ERROR_READING_MAXCARRIERBPS:
			return "The system was unable to read the maximum carrier connection speed from the media.INI file.";
		case ERROR_LINE_BUSY:
			return "The phone line is busy.";
		case ERROR_VOICE_ANSWER:
			return "A person answered instead of a modem (or other connecting device).";
		case ERROR_NO_ANSWER:
			return "The remote computer did not respond.";
		case ERROR_NO_CARRIER:
			return "The system could not detect the carrier.";
		case ERROR_NO_DIALTONE:
			return "There was no dial tone.";
		case ERROR_IN_COMMAND:
			return "The modem (or other connecting device) reported a general error.";
		case ERROR_WRITING_SECTIONNAME:
			return "There was an error in writing the section name.";
		case ERROR_WRITING_DEVICETYPE:
			return "There was an error in writing the device type.";
		case ERROR_WRITING_DEVICENAME:
			return "There was an error in writing the device name.";
		case ERROR_WRITING_MAXCONNECTBPS:
			return "There was an error in writing the maximum connection speed.";
		case ERROR_WRITING_MAXCARRIERBPS:
			return "There was an error in writing the maximum carrier speed.";
		case ERROR_WRITING_USAGE:
			return "There was an error in writing the usage.";
		case ERROR_WRITING_DEFAULTOFF:
			return " There was an error in writing the default-off.";
		case ERROR_READING_DEFAULTOFF:
			return " There was an error in reading the default-off.";
		case ERROR_EMPTY_INI_FILE:
         return "Empty error file.";
		case ERROR_AUTHENTICATION_FAILURE:
			return "Access was denied because the username and/or password was invalid on the domain.";
		case ERROR_PORT_OR_DEVICE:
			return "There was a hardware failure in the modem (or other connecting device).";
		case ERROR_NOT_BINARY_MACRO:
			return "Not binary macro.";
		case ERROR_DCB_NOT_FOUND:
			return "EDCB not found.";
		case ERROR_STATE_MACHINES_NOT_STARTED:
			return "The state machines are not started.";
		case ERROR_STATE_MACHINES_ALREADY_STARTED:
			return "The state machines are already started.";
		case ERROR_PARTIAL_RESPONSE_LOOPING:
			return "The response looping did not complete.";
		case ERROR_UNKNOWN_RESPONSE_KEY:
			return "A response keyname in the device.INF file is not in the expected format.";
		case ERROR_RECV_BUF_FULL:
			return "The modem (or other connecting device) response caused a buffer overflow.";
		case ERROR_CMD_TOO_LONG:
			return "The expanded command in the device.INF file is too long.";
		case ERROR_UNSUPPORTED_BPS:
			return "The modem moved to a connection speed not supported by the COM driver.";
		case ERROR_UNEXPECTED_RESPONSE:
			return "Device response received when none expected.";
		case ERROR_INTERACTIVE_MODE:
			return "The connection needs information from you, but the application does not allow user interaction.";
		case ERROR_BAD_CALLBACK_NUMBER:
			return "The callback number is invalid.";
		case ERROR_INVALID_AUTH_STATE:
			return "The authorization state is invalid.";
		case ERROR_WRITING_INITBPS:
			return "ERROR_WRITING_INITBPS";
		case ERROR_X25_DIAGNOSTIC:
			return "There was an error related to the X.25 protocol.";
		case ERROR_ACCT_EXPIRED:
			return "The account has expired.";
		case ERROR_CHANGING_PASSWORD:
			return "There was an error changing the password on the domain.";
		case ERROR_OVERRUN:
			return "Serial overrun errors were detected while communicating with the modem.";
		case ERROR_RASMAN_CANNOT_INITIALIZE	:
			return "A configuration error on this computer is preventing this connection.";
		case ERROR_BIPLEX_PORT_NOT_AVAILABLE:
			return "The two-way port is initializing.  Wait a few seconds and redial.";
		case ERROR_NO_ACTIVE_ISDN_LINES:
			return "No active ISDN lines are available.";
		case ERROR_NO_ISDN_CHANNELS_AVAILABLE:
			return "No ISDN channels are available to make the call.";
		case ERROR_TOO_MANY_LINE_ERRORS:
			return "Too many errors occurred because of poor phone line quality.";
		case ERROR_IP_CONFIGURATION:
			return "The Remote Access Service IP configuration is unusable.";
		case ERROR_NO_IP_ADDRESSES:
			return "No IP addresses are available in the static pool of Remote Access Service IP addresses.";
		case ERROR_PPP_TIMEOUT:
			return "The connection was terminated because the remote computer did not respond in a timely manner.";
		case ERROR_PPP_REMOTE_TERMINATED:
			return "The connection was terminated by the remote computer.";
		case ERROR_PPP_NO_PROTOCOLS_CONFIGURED:
			return "A connection to the remote computer could not be established.";
		case ERROR_PPP_NO_RESPONSE:
			return "The remote computer did not respond.";
		case ERROR_PPP_INVALID_PACKET:
			return "Invalid data was received from the remote computer. This data was ignored.";
		case ERROR_PHONE_NUMBER_TOO_LONG:
			return "The phone number, including prefix and suffix, is too long.";
		case ERROR_IPXCP_NO_DIALOUT_CONFIGURED:
			return "The IPX protocol cannot dial out on the modem (or other connecting device) because this computer is not configured for dialing out (it is an IPX router).";
		case ERROR_IPXCP_NO_DIALIN_CONFIGURED:
			return "The IPX protocol cannot dial in on the modem (or other connecting device) because this computer is not configured for dialing in (the IPX router is not installed).";
		case ERROR_IPXCP_DIALOUT_ALREADY_ACTIVE:
			return "The IPX protocol cannot be used for dialing out on more than one modem (or other connecting device) at a time.";
		case ERROR_ACCESSING_TCPCFGDLL:
			return "Cannot access TCPCFG.DLL.";
		case ERROR_NO_IP_RAS_ADAPTER:
			return "The system cannot find an IP adapter.";
		case ERROR_SLIP_REQUIRES_IP:
			return "SLIP cannot be used unless the IP protocol is installed.";
		case ERROR_PROJECTION_NOT_COMPLETE:
			return "Computer registration is not complete.";
		case ERROR_PROTOCOL_NOT_CONFIGURED:
			return "The protocol is not configured.";
		case ERROR_PPP_NOT_CONVERGING:
			return "Your computer and the remote computer could not agree on PPP control protocols.";
		case ERROR_PPP_CP_REJECTED:
			return "A connection to the remote computer could not be completed. You might need to adjust the protocols on this computer. For further assistance, click More Info or search Help and Support Center for this error number.";
		case ERROR_PPP_LCP_TERMINATED:
			return "The PPP link control protocol was terminated.";
		case ERROR_PPP_REQUIRED_ADDRESS_REJECTED:
			return "The requested address was rejected by the server.";
		case ERROR_PPP_NCP_TERMINATED:
			return "The remote computer terminated the control protocol.";
		case ERROR_PPP_LOOPBACK_DETECTED:
			return "Loopback was detected.";
		case ERROR_PPP_NO_ADDRESS_ASSIGNED:
			return "The server did not assign an address.";
		case ERROR_CANNOT_USE_LOGON_CREDENTIALS:
			return "The authentication protocol required by the remote server cannot use the stored password.";
		case ERROR_TAPI_CONFIGURATION:
			return "An invalid dialing rule was detected.";
		case ERROR_NO_LOCAL_ENCRYPTION:
			return "The local computer does not support the required data encryption type.";
		case ERROR_NO_REMOTE_ENCRYPTION:
			return "The remote computer does not support the required data encryption type.";
		case ERROR_REMOTE_REQUIRES_ENCRYPTION:
			return "The remote computer requires data encryption.";
		case ERROR_IPXCP_NET_NUMBER_CONFLICT:
			return "The system cannot use the IPX network number assigned by the remote computer.";
		case ERROR_INVALID_SMM:
			return "ERROR_INVALID_SMM";
		case ERROR_SMM_UNINITIALIZED:
			return "ERROR_SMM_UNINITIALIZED";
		case ERROR_NO_MAC_FOR_PORT:
			return "ERROR_NO_MAC_FOR_PORT";
		case ERROR_SMM_TIMEOUT:
			return "ERROR_SMM_TIMEOUT";
		case ERROR_BAD_PHONE_NUMBER:
			return "ERROR_BAD_PHONE_NUMBER";
		case ERROR_WRONG_MODULE:
			return "ERROR_WRONG_MODULE";
		case ERROR_INVALID_CALLBACK_NUMBER:
			return "The callback number contains an invalid character.";
		case ERROR_SCRIPT_SYNTAX:
			return "A syntax error was encountered while processing a script.";
		case ERROR_HANGUP_FAILED:
			return "The connection could not be disconnected because it was created by the multi-protocol router.";
		case ERROR_BUNDLE_NOT_FOUND:
			return "The system could not find the multi-link bundle.";
		case ERROR_CANNOT_DO_CUSTOMDIAL:
			return "The system cannot perform automated dial because this connection  has a custom dialer specified.";
 		case ERROR_DIAL_ALREADY_IN_PROGRESS:
			return "This connection is already being dialed.";
		case ERROR_RASAUTO_CANNOT_INITIALIZE	:
			return "Remote Access Services could not be started automatically.";
		case ERROR_CONNECTION_ALREADY_SHARED:
			return "Internet Connection Sharing is already enabled on the connection.";
		case ERROR_SHARING_CHANGE_FAILED:
			return "An error occurred while the existing Internet Connection Sharing settings were being changed.";
		case ERROR_SHARING_ROUTER_INSTALL:
			return "An error occurred while routing capabilities were being enabled.";
		case ERROR_SHARE_CONNECTION_FAILED:
			return "An error occurred while Internet Connection Sharing was being enabled for the connection.";
		case ERROR_SHARING_PRIVATE_INSTALL:
			return "An error occurred while the local network was being configured for sharing.";
		case ERROR_CANNOT_SHARE_CONNECTION:
			return "Internet Connection Sharing cannot be enabled. There is more than one LAN connection other than the connection to be shared.";
		case ERROR_NO_SMART_CARD_READER:
			return "No smart card reader is installed.";
		case ERROR_SHARING_ADDRESS_EXISTS:
			return "Internet Connection Sharing cannot be enabled. A LAN connection is already configured with the IP address that is required for automatic IP addressing.";
		case ERROR_NO_CERTIFICATE:
			return "A certificate could not be found. Connections that use the L2TP protocol over IPSec require the installation of a machine certificate.";
		case ERROR_SHARING_MULTIPLE_ADDRESSES:
			return "Internet Connection Sharing cannot be enabled. The LAN connection selected as the private network has more than one IP address configured.";
		case ERROR_FAILED_TO_ENCRYPT:
			return "The connection attempt failed because of failure to encrypt data.";
		case ERROR_BAD_ADDRESS_SPECIFIED:
			return "The specified destination is not reachable.";
		case ERROR_CONNECTION_REJECT:
			return "The remote computer rejected the connection attempt.";
		case ERROR_CONGESTION:
			return "The connection attempt failed because the network is busy.";
		case ERROR_INCOMPATIBLE:
			return "The remote computer's network hardware is incompatible with the type of call requested.";
		case ERROR_NUMBERCHANGED:
			return "The connection attempt failed because the destination number has changed.";
		case ERROR_TEMPFAILURE:
			return "The connection attempt failed because of a temporary failure.  Try connecting again.";
		case ERROR_BLOCKED:
			return "The call was blocked by the remote computer.";
		case ERROR_DONOTDISTURB:
			return "The call could not be connected because the remote computer has invoked the Do Not Disturb feature.";
		case ERROR_OUTOFORDER:
			return "The connection attempt failed because the modem (or other connecting device) on the remote computer is out of order.";
		case ERROR_UNABLE_TO_AUTHENTICATE_SERVER:
			return "It was not possible to verify the identity of the server.";
		case ERROR_SMART_CARD_REQUIRED:
			return "To dial out using this connection you must use a smart card.";
		case ERROR_INVALID_FUNCTION_FOR_ENTRY:
			return "An attempted function is not valid for this connection.";
		case ERROR_CERT_FOR_ENCRYPTION_NOT_FOUND:
			return "The connection requires a certificate, and no valid certificate was found.";
		case ERROR_SHARING_RRAS_CONFLICT:
			return "Network Address Translation (NAT) is currently installed as a routing protocol, and must be removed before enabling Internet Connection Sharing and/or Internet Connection Firewall.";
		case ERROR_SHARING_NO_PRIVATE_LAN:
			return "Internet Connection Sharing cannot be enabled. The LAN connection selected as the private network is either not present, or is disconnected from the network.";
		case ERROR_NO_DIFF_USER_AT_LOGON:
			return "You cannot dial using this connection at logon time, because it is configured to use a user name different than the one on the smart card.";
		case ERROR_NO_REG_CERT_AT_LOGON:
			return "You cannot dial using this connection at logon time, because it is not configured to use a smart card.";
		case ERROR_OAKLEY_NO_CERT:
			return "The L2TP connection attempt failed because there is no valid machine certificate on your computer for security authentication.";
		case ERROR_OAKLEY_AUTH_FAIL:
			return "The L2TP connection attempt failed because the security layer could not authenticate the remote computer.";
		case ERROR_OAKLEY_ATTRIB_FAIL:
			return "The L2TP connection attempt failed because the security layer could not negotiate compatible parameters with the remote computer.";
		case ERROR_OAKLEY_GENERAL_PROCESSING:
			return "The L2TP connection attempt failed because the security layer encountered a processing error during initial negotiations with the remote computer.";
		case ERROR_OAKLEY_NO_PEER_CERT:
			return "The L2TP connection attempt failed because certificate validation on the remote computer failed.";
		case ERROR_OAKLEY_NO_POLICY:
			return "The L2TP connection attempt failed because security policy for the connection was not found.";
		case ERROR_OAKLEY_TIMED_OUT:
			return "The L2TP connection attempt failed because security negotiation timed out.";
		case ERROR_OAKLEY_ERROR:
			return "The L2TP connection attempt failed because an error occurred while negotiating security.";
		case ERROR_UNKNOWN_FRAMED_PROTOCOL:
			return "The Framed Protocol RADIUS attribute for this user is not PPP.";
		case ERROR_WRONG_TUNNEL_TYPE:
			return "The Tunnel Type RADIUS attribute for this user is not correct.";
		case ERROR_UNKNOWN_SERVICE_TYPE:
			return "The Service Type RADIUS attribute for this user is neither Framed nor Callback Framed.";
		case ERROR_CONNECTING_DEVICE_NOT_FOUND:
			return "A connection to the remote computer could not be established because the modem was not found or was busy.";
		case ERROR_NO_EAPTLS_CERTIFICATE:
			return "A certificate could not be found that can be used with this Extensible Authentication Protocol.";
		case ERROR_SHARING_HOST_ADDRESS_CONFLICT:
			return "Internet Connection Sharing (ICS) cannot be enabled due to an IP address conflict on the network.";
		case ERROR_AUTOMATIC_VPN_FAILED:
			return "Unable to establish the VPN connection.  The VPN server may be unreachable, or security parameters may not be configured properly for this connection.";
		case ERROR_VALIDATING_SERVER_CERT:
         return "This connection is configured to validate the identity of the access server, but Windows cannot verify the digital certificate sent by the server.";
      default:
			return "Unknown Error";
      }
*/
}
