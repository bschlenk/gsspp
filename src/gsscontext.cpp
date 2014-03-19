#include "gsspp/gsscontext.h"
#include "gsspp/gssbuffer.h"
#include "gsspp/gssname.h"
#include "gsspp/gsscredential.h"
#include "gsspp/gssmech.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>


#include <iostream>


void GSSContext::clear()
{
	OM_uint32 x;
	gss_delete_sec_context( &x, &_context, GSS_C_NO_BUFFER );
}

bool GSSContext::initialize( GSSBuffer& input, const GSSName& target, const Flags& flags, GSSCredential cred, GSSMech mech, OM_uint32 time, gss_channel_bindings_t bindings )
{
	GSSBuffer recv_tok, send_tok;
	OM_uint32 maj_stat, min_stat, ret_flags;

	maj_stat = gss_init_sec_context(
			&min_stat,
			cred,
			&_context,
			target,
			mech,
			flags,
			time,
			bindings,
			input,
			0,
			send_tok,
			&ret_flags,
			0
	);

	if ( !valid() || ( maj_stat != GSS_S_COMPLETE && maj_stat != GSS_S_CONTINUE_NEEDED ) ) {
		throw GSSException( maj_stat, min_stat, "gss_init_sec_context" );
	}

	input.swap( send_tok );

	return maj_stat == GSS_S_CONTINUE_NEEDED;
}
	
void GSSContext::import_context( const GSSBuffer& buff )
{
	OM_uint32 maj, min;
	maj = gss_import_sec_context( &min, const_cast<GSSBuffer&>( buff ), &_context );

	if ( maj != GSS_S_COMPLETE )
	{
		throw GSSException( maj, min, "gss_import_sec_context" );
	}
}

GSSBuffer GSSContext::export_context()
{
	OM_uint32 maj, min;
	GSSBuffer exported;
	gss_export_sec_context( &min, &_context, exported );

	if ( maj != GSS_S_COMPLETE )
	{
		throw GSSException( maj, min, "gss_export_sec_context" );
	}

	return exported;
}

GSSBuffer GSSContext::wrap( const GSSBuffer& message, bool encrypt, gss_qop_t qop ) const
{ 
	return wrap( *this, message, encrypt, qop );
}

GSSBuffer GSSContext::wrap( const GSSContext& context, const GSSBuffer& message, bool encrypt, gss_qop_t qop )
{
	OM_uint32 maj, min;
	GSSBuffer wrapped;
	maj = gss_wrap( &min, context, encrypt, qop, const_cast<GSSBuffer&>( message ), 0, wrapped );

	if ( maj != GSS_S_COMPLETE )
	{
		throw GSSException( maj, min, "gss_wrap" );
	}

	return wrapped;
}

void GSSContext::wrap_in_place( GSSBuffer& message, bool encrypt, gss_qop_t qop ) const
{
	wrap_in_place( *this, message, encrypt, qop );
}

void GSSContext::wrap_in_place( const GSSContext& context, GSSBuffer& message, bool encrypt, gss_qop_t qop )
{
	GSSBuffer tmp( wrap( context, message, encrypt, qop ) );
	message.swap( tmp );
}

GSSBuffer GSSContext::unwrap( const GSSBuffer& wrapped ) const
{
	return unwrap( *this, wrapped );
}

GSSBuffer GSSContext::unwrap( const GSSContext& context, const GSSBuffer& message )
{
	OM_uint32 maj, min;
	GSSBuffer unwrapped;
	maj = gss_unwrap( &min, context, const_cast<GSSBuffer&>( message ), unwrapped, 0, 0 );

	if ( maj != GSS_S_COMPLETE )
	{
		throw GSSException( maj, min, "gss_unwrap" );
	}

	return unwrapped;
}
	
void GSSContext::unwrap_in_place( GSSBuffer& wrapped ) const
{
	unwrap_in_place( *this, wrapped );
}

void GSSContext::unwrap_in_place( const GSSContext& context, GSSBuffer& message )
{
	GSSBuffer tmp( unwrap( context, message ) );
	message.swap( tmp );
}
