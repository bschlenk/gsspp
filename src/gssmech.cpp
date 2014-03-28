#include "gsspp/gssmech.h"
#include "gsspp/gssbuffer.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>
#include <string>
#include <algorithm>
using std::string;
using std::transform;

GSSMech::GSSMech( const string& mech_name )
{
	OM_uint32 maj, min;
	GSSBuffer buff( mech_name );
	maj = gss_str_to_oid( &min, buff, &_oid );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_str_to_oid" );
}
