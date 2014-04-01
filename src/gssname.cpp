#include "gsspp/gssname.h"
#include "gsspp/gssbuffer.h"
#include "gsspp/gssmech.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>
//#include <gssapi/gssapi_krb5.h>
#include <string>
#include <algorithm>

using std::string;

const gss_OID GSSName::_oid_types[] = {
	GSS_C_NO_OID,
	GSS_C_NT_USER_NAME,
	GSS_C_NT_MACHINE_UID_NAME,
	GSS_C_NT_STRING_UID_NAME,
	// GSS_C_NT_HOSTBASED_SERVICE_X, // deprecated, use GSS_C_NT_HOSTBASED_SERVICE
	GSS_C_NT_HOSTBASED_SERVICE,
	GSS_C_NT_ANONYMOUS,
	GSS_C_NT_EXPORT_NAME
	// const_cast<gss_OID>( GSS_KRB5_NT_PRINCIPAL_NAME )
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

string GSSName::str( NameType * nt ) const
{
	OM_uint32 maj, min;
	GSSBuffer buff;
	gss_OID oid;
	maj = gss_display_name( &min, _name, buff, &oid );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_display_name" );

	if ( nt )
	{
		for ( int i = 0, len = ( sizeof( _oid_types ) / sizeof( gss_OID ) ); i < len; ++i )
		{
			if ( _oid_types[i] == oid )
			{
				*nt = (NameType) i;
				break;
			}
		}
	}

	return buff.str();
}

GSSName GSSName::canonicalize( const GSSMech& mech ) const
{
	OM_uint32 maj, min;
	GSSName canon_name;
	maj = gss_canonicalize_name( &min, _name, const_cast<GSSMech&>( mech ), canon_name );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_canonicalize_name" );

	return canon_name;
}

GSSBuffer GSSName::export_name( const GSSMech& mech ) const
{
	GSSName name( canonicalize( mech ) );

	OM_uint32 maj, min;
	GSSBuffer buff;
	maj = gss_export_name( &min, name, buff );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_export_name" );

	return buff;
}

void GSSName::import_name( const GSSBuffer& buff, NameType nt )
{
	OM_uint32 maj, min;
	maj = gss_import_name( &min, const_cast<GSSBuffer&>( buff ), _oid_types[nt], &_name );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_import_name" );
}

bool GSSName::operator== ( const GSSName& name ) const
{
	OM_uint32 maj;
	int equal;
	maj = gss_compare_name( 0, *( const_cast<GSSName*>( this ) ), const_cast<GSSName&>( name ), &equal );

	return ( maj == GSS_S_COMPLETE ) && equal;
}

bool GSSName::operator== ( const std::string& str ) const
{
	// If importing the name given by `str` throws an exception,
	// assume it is not equal.
	try
	{
		GSSName other( str );
		return *this == other;
	}
	catch ( GSSException& e )
	{
		return false;
	}
}

bool GSSName::operator!= ( const GSSName& name    ) const { return !( *this == name ); }
bool GSSName::operator!= ( const std::string& str ) const { return !( *this == str  ); }

bool operator== ( const std::string& str, const GSSName& name ) { return name == str;    }
bool operator!= ( const std::string& str, const GSSName& name ) { return !(name == str); }

std::ostream& operator<< ( std::ostream& os, const GSSName& name )
{
	os << name.str();
	return os;
}
