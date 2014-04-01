#include "gsspp/gssmech.h"
#include "gsspp/gssbuffer.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstdio>
using std::string;
using std::transform;
using std::ostream;

using namespace std;

GSSMech::GSSMech( const string& mech_name )
{
	/*
	OM_uint32 maj, min;
	GSSBuffer buff( mech_name );
	maj = gss_str_to_oid( &min, buff, &_oid );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_str_to_oid" );
	*/
}

string GSSMech::str() const
{
	int max_len = size() * 3 + size() + 1;
	char * buff = new char[max_len];
	int pos = 0;
	for ( size_t i = 0; i < _oid->length; ++i )
		pos += sprintf( buff + pos, "%d.", ( ( unsigned char * )( _oid->elements ) )[i] );
	string s( buff, pos - 1 );
	delete [] buff;
	return s;
}

ostream& operator<< ( ostream& os, const GSSMech& mech )
{
	os << mech.str();
	return os;
}

GSSMechList::GSSMechList() : _oid_set( GSS_C_NO_OID_SET )
{
	OM_uint32 min;
	gss_create_empty_oid_set( &min, &_oid_set );
}

void GSSMechList::add( const GSSMech& mech )
{
	OM_uint32 maj, min;
	maj = gss_add_oid_set_member( &min, const_cast<GSSMech&>( mech ), &_oid_set );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_add_oid_set_member" );
}

bool GSSMechList::contains( const GSSMech& mech ) const
{
	OM_uint32 maj, min;
	int present;
	maj = gss_test_oid_set_member( &min, const_cast<GSSMech&>( mech ), _oid_set, &present );

	return ( maj == GSS_S_COMPLETE ) && present;
}

const GSSMech GSSMechList::operator[]( size_t index ) const
{
	return ( _oid_set->elements + index );
}

const GSSMech GSSMechList::at( size_t index ) const
{
	if ( index >= _oid_set->count )
		throw std::out_of_range( "index out of range" );

	return ( *this )[ index ];
}

void GSSMechList::inquire()
{
	clear();

	OM_uint32 maj, min;
	maj = gss_indicate_mechs( &min, &_oid_set );

	if ( maj != GSS_S_COMPLETE )
		throw GSSException( maj, min, "gss_indicate_mechs" );
}

void GSSMechList::clear()
{
	OM_uint32 min;
	gss_release_oid_set( &min, &_oid_set );
}

GSSMechList& GSSMechList::operator+= ( const GSSMech& mech )
{
	add( mech );
	return *this;
}
