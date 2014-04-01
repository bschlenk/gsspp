#include "gsspp/gssmech.h"
#include "gsspp/gssbuffer.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>
#include <string>
#include <algorithm>
#include <stdexcept>
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

gss_OID GSSMechList::operator[]( size_t index )
{
	return ( _oid_set->elements + index );
}

gss_OID GSSMechList::at( size_t index )
{
	if ( index >= _oid_set->count )
		throw std::out_of_range( "index out of range" );

	return ( *this )[ index ];
}

void GSSMechList::inquire()
{
	OM_uint32 maj, min;
	maj = gss_indicate_mechs( &min, &_oid_set );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_indicate_mechs" );
}

void GSSMechList::clear()
{
	OM_uint32 maj, min;
	gss_release_oid_set( &min, &_oid_set );
}
