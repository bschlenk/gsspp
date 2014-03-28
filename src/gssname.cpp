#include "gsspp/gssname.h"
#include "gsspp/gssbuffer.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>
#include <algorithm>

const gss_OID GSSName::_oid_types[] = {
	GSS_C_NO_OID,
	GSS_C_NT_USER_NAME,
	GSS_C_NT_MACHINE_UID_NAME,
	GSS_C_NT_STRING_UID_NAME,
	// GSS_C_NT_HOSTBASED_SERVICE_X, // deprecated, use GSS_C_NT_HOSTBASED_SERVICE
	GSS_C_NT_HOSTBASED_SERVICE,
	GSS_C_NT_ANONYMOUS,
	GSS_C_NT_EXPORT_NAME
};
	

GSSName::GSSName( const GSSName& other )
{
	OM_uint32 maj, min;
	maj = gss_duplicate_name( &min, other._name, &_name );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_duplicate_name" );
}

GSSName::GSSName( const gss_name_t& other )
{
	OM_uint32 maj, min;
	maj = gss_duplicate_name( &min, other, &_name );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_duplicate_name" );
}

GSSName::GSSName( const GSSBuffer& buff, NameType nt )
{
	OM_uint32 maj, min;

	maj = gss_import_name(
		&min,
		const_cast<GSSBuffer&>( buff ),
		_oid_types[nt],
		&_name
	);

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_import_name" );
}

GSSName::GSSName( const char * name, NameType nt ) : _name( GSS_C_NO_NAME )
{
	GSSBuffer buff( name );
	GSSName tmp( buff, nt );
	swap( tmp );
}

GSSName::GSSName( const std::string& name, NameType nt ) : _name( GSS_C_NO_NAME )
{
	GSSBuffer buff( name );
	GSSName tmp( buff, nt );
	swap( tmp );
}

void GSSName::assign( const GSSName& other )
{
	GSSName tmp( other );
	swap( tmp );
}

void GSSName::assign( const gss_name_t& other )
{
	GSSName tmp( other );
	swap( tmp );
}

void GSSName::assign( const GSSBuffer& buff, NameType nt )
{
	GSSName tmp( buff, nt );
	swap( tmp );
}

void GSSName::assign( const char * name, NameType nt )
{
	GSSName tmp( name, nt );
	swap( tmp );
}

void GSSName::assign( const std::string& name, NameType nt )
{
	GSSName tmp( name, nt );
	swap( tmp );
}

void GSSName::swap( GSSName& other )
{
	std::swap( _name, other._name );
}

void GSSName::swap( gss_name_t& other )
{
	std::swap( _name, other );
}

void GSSName::clear()
{
	OM_uint32 x;
	gss_release_name( &x, &_name );
}
