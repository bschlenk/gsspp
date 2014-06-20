#include "gsspp/gsscredential.h"
#include "gsspp/gssname.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>

GSSCredential::GSSCredential( const GSSName& name )
{
	// TODO: have a way to return actual time and actual mech

	OM_uint32 maj, min;
	maj = gss_acquire_cred( &min, name, GSS_C_INDEFINITE, 0, 0, &_credential, 0, 0 );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_acquire_cred" );
}

void GSSCredential::clear()
{
	OM_uint32 min;
	gss_release_cred( &min, &_credential );
}
