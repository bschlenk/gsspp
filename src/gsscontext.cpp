#include "gsspp/gsscontext.h"
#include "gsspp/gssbuffer.h"
#include "gsspp/gssname.h"
#include "gsspp/gsscredential.h"
#include "gsspp/gssmech.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>


GSSContext::GSSContext( const GSSBuffer& buff )
{
	import_context( buff );
}

void GSSContext::clear()
{
	OM_uint32 x;
	gss_delete_sec_context( &x, &_context, GSS_C_NO_BUFFER );
}

bool GSSContext::initialize( GSSBuffer& input, const GSSName& target, const Flags& flags, const GSSCredential& cred, GSSMech mech, OM_uint32 time, gss_channel_bindings_t bindings )
{
	GSSBuffer recv_tok, send_tok;
	OM_uint32 maj_stat, min_stat, ret_flags;

	maj_stat = gss_init_sec_context(
			&min_stat,
			const_cast<GSSCredential&>( cred ),
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

bool GSSContext::accept( GSSBuffer& input, const GSSCredential& cred, gss_channel_bindings_t bindings )
{
	GSSBuffer output;
	OM_uint32 maj_stat, min_stat, ret_flags;
	
	maj_stat = gss_accept_sec_context(
			&min_stat,
			&_context,
			const_cast<GSSCredential&>( cred ),
			input,
			bindings,
			0,        // gss_name_t* - name of initiating principle
			0,        // gss_OID*    - security mechanism used
			output,
			0,        // OM_uint32*  - ret_flags
			0,        // OM_uint32*  - time_req
			0         // gss_cred_id_t* - delegated_cred_handle - only valid if ret_flags contains GSS_C_DELEG_FLAG
	);

	if ( !valid() || ( maj_stat != GSS_S_COMPLETE && maj_stat != GSS_S_CONTINUE_NEEDED ) )
		throw GSSException( maj_stat, min_stat, "gss_accept_sec_context" );

	input.swap( output );

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

GSSBuffer GSSContext::get_mic( const GSSBuffer& message, gss_qop_t qop ) const
{
	OM_uint32 maj, min;
	GSSBuffer mic;

	maj = gss_get_mic( &min, _context, qop, const_cast<GSSBuffer&>( message ), mic );

	if ( maj != GSS_S_COMPLETE )
	{
		throw GSSException( maj, min, "gss_get_mic" );
	}

	return mic;
}

bool GSSContext::verify_mic( const GSSBuffer& message, const GSSBuffer& mic ) const
{
	OM_uint32 maj, min;
	maj = gss_verify_mic( &min, _context, const_cast<GSSBuffer&>( message ), const_cast<GSSBuffer&>( mic ), 0 );

	if ( maj == GSS_S_COMPLETE )
		return true;
	if ( maj == GSS_S_BAD_SIG )
		return false;
	throw GSSException( maj, min, "gss_verify_mic" );
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

GSSBuffer GSSContext::unwrap( const GSSBuffer& wrapped, bool * conf_state, gss_qop_t * qop_state ) const
{
	return unwrap( *this, wrapped, conf_state, qop_state );
}

GSSBuffer GSSContext::unwrap( const GSSContext& context, const GSSBuffer& message, bool * conf_state, gss_qop_t * qop_state )
{
	OM_uint32 maj, min;
	int iconf_state;
	GSSBuffer unwrapped;
	maj = gss_unwrap( &min, context, const_cast<GSSBuffer&>( message ), unwrapped, &iconf_state, qop_state );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_unwrap" );

	if ( conf_state )
		*conf_state = iconf_state;

	return unwrapped;
}
	
void GSSContext::unwrap_in_place( GSSBuffer& wrapped, bool * conf_state, gss_qop_t * qop_state ) const
{
	unwrap_in_place( *this, wrapped, conf_state, qop_state );
}

void GSSContext::unwrap_in_place( const GSSContext& context, GSSBuffer& message, bool * conf_state, gss_qop_t * qop_state )
{
	GSSBuffer tmp( unwrap( context, message, conf_state, qop_state ) );
	message.swap( tmp );
}

size_t GSSContext::wrap_size_limit( size_t max_size, bool encrypt, gss_qop_t qop ) const
{
	OM_uint32 maj, min, size_limit;
	maj = gss_wrap_size_limit( &min, _context, encrypt, qop, max_size, &size_limit );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_wrap_size_limit" );

	return size_limit;
}
