#include "gsspp/gssexception.h"
#include "gsspp/gssbuffer.h"
#include <gssapi.h>
#include <cstring>

void display_helper( OM_uint32 code, int type, char * message, int len );

GSSException::GSSException( OM_uint32 maj_stat, OM_uint32 min_stat, const char * func ) throw() : major_status( maj_stat ), minor_status( min_stat )
{
	strncpy( function, func, sizeof( function ) - 1 );
	function[sizeof( function ) - 1] = 0;

	try
	{
		display_helper( maj_stat, GSS_C_GSS_CODE,  major_message, sizeof( major_message ) );
		display_helper( min_stat, GSS_C_MECH_CODE, minor_message, sizeof( minor_message ) );
	}
	catch ( GSSException& e )
	{
		strcpy( major_message, e.major_message );
		strcpy( minor_message, e.minor_message );
		strcpy( function,      e.function      );
	}
}

const char * GSSException::what() const throw()
{
	if ( minor_status )
		return minor_message;
	return major_message;
}

void display_helper( OM_uint32 code, int type, char * message, int len )
{
	OM_uint32 maj, min;
	OM_uint32 msg_ctx = 0;
	int pos = 0;

	do
	{
		GSSBuffer buff;

		maj = gss_display_status( &min, code, type, GSS_C_NO_OID, &msg_ctx, buff );

		if ( maj != GSS_S_COMPLETE )
			throw GSSException( maj, min, "gss_display_status" );

		strncpy( message + pos, buff.bytes(), len - pos - 1 );
		pos += buff.size();
	}
	while ( msg_ctx && pos < len );

	message[len - 1] = 0;
}
