#include "gsspp/gssbuffer.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <gssapi.h>
#include <algorithm>
using std::string;
using std::ostream;
using std::istream;


GSSBuffer::GSSBuffer( const GSSBuffer& other )
{
	_buff.length = other._buff.length;
	_buff.value = malloc( _buff.length );
	memcpy( _buff.value, other._buff.value, _buff.length );
}

GSSBuffer::GSSBuffer( const gss_buffer_desc& buff )
{
	_buff.length = buff.length;
	_buff.value = malloc( _buff.length );
	memcpy( _buff.value, buff.value, _buff.length );
}

GSSBuffer::GSSBuffer( size_t len )
{
	_buff.length = len;
	_buff.value = malloc( _buff.length );
}

GSSBuffer::GSSBuffer( const char * s )
{
	_buff.length = strlen( s );
	_buff.value = malloc( _buff.length );
	memcpy( (char *)_buff.value, s, _buff.length );
}

GSSBuffer::GSSBuffer( const void * s, size_t len )
{
	_buff.length = len;
	_buff.value = malloc( _buff.length );
	memcpy( _buff.value, s, _buff.length );
}

GSSBuffer::GSSBuffer( const string& str )
{
	_buff.length = str.size();
	_buff.value = malloc( _buff.length );
	memcpy( _buff.value, str.c_str(), _buff.length );
}

GSSBuffer::GSSBuffer( istream& is, size_t len )
{
	_buff.length = len;
	_buff.value = malloc( _buff.length );
	is.read( (char *)_buff.value, len );
}


void GSSBuffer::assign( const GSSBuffer& other )
{
	GSSBuffer tmp( other );
	swap( tmp );
}

void GSSBuffer::assign( const gss_buffer_desc& buff )
{
	GSSBuffer tmp( buff );
	swap( tmp );
}

void GSSBuffer::assign( const char * s )
{
	GSSBuffer tmp( s );
	swap( tmp );
}

void GSSBuffer::assign( void * s, size_t len )
{
	GSSBuffer tmp( s, len );
	swap( tmp );
}

void GSSBuffer::assign( const string& str )
{
	GSSBuffer tmp( str );
	swap( tmp );
}

void GSSBuffer::assign( istream& is, size_t len )
{
	GSSBuffer tmp( is, len );
	swap( tmp );
}


void GSSBuffer::swap( GSSBuffer& other )
{
	std::swap( _buff, other._buff );
}

void GSSBuffer::swap( gss_buffer_desc& other )
{
	std::swap( _buff, other );
}

void GSSBuffer::clear()
{
	OM_uint32 x;
	// this function checks if the buffer is null
	gss_release_buffer( &x, &_buff );
}

void GSSBuffer::resize( size_t len )
{
	GSSBuffer tmp( len );
	memcpy( tmp._buff.value, _buff.value, std::min( len, _buff.length ) );
	
	swap( tmp );
}

GSSBuffer& GSSBuffer::operator += ( const GSSBuffer& other )
{
	GSSBuffer tmp( *this );
	tmp.resize( tmp.size() + other.size() );
	
	memcpy( (char *)tmp._buff.value + size(), other._buff.value, other.size() );
	swap( tmp );

	return *this;
}

bool operator ==( const GSSBuffer& a,       const GSSBuffer& b       ) { return ( a.size() == b.size() && !memcmp( a.bytes(), b.bytes(), a.size() ) ); }
bool operator ==( const GSSBuffer& a,       const gss_buffer_desc& b ) { return ( a.size() == b.length && !memcmp( a.bytes(), b.value, a.size() ) ); }
bool operator ==( const gss_buffer_desc& a, const GSSBuffer& b       ) { return b == a; }

bool operator < ( const GSSBuffer& a,       const GSSBuffer& b       ) { return ( a.size() < b.size() || ( a.size() == b.size() && 0 > memcmp( a.bytes(), b.bytes(), a.size() ) ) ); }
bool operator < ( const GSSBuffer& a,       const gss_buffer_desc& b ) { return ( a.size() < b.length || ( a.size() == b.length && 0 > memcmp( a.bytes(), b.value, a.size() ) ) ); }
bool operator < ( const gss_buffer_desc& a, const GSSBuffer& b       ) { return b > a; }

bool operator !=( const GSSBuffer& a,       const GSSBuffer& b       ) { return !( a == b ); }
bool operator !=( const GSSBuffer& a,       const gss_buffer_desc& b ) { return !( a == b ); }
bool operator !=( const gss_buffer_desc& a, const GSSBuffer& b       ) { return b != a; }

bool operator > ( const GSSBuffer& a,       const GSSBuffer& b       ) { return !( a < b ) && !( a == b ); }
bool operator > ( const GSSBuffer& a,       const gss_buffer_desc& b ) { return !( a < b ) && !( a == b ); }
bool operator > ( const gss_buffer_desc& a, const GSSBuffer& b       ) { return b < a; }

bool operator <=( const GSSBuffer& a,       const GSSBuffer& b       ) { return ( a < b ) || ( a == b ); }
bool operator <=( const GSSBuffer& a,       const gss_buffer_desc& b ) { return ( a < b ) || ( a == b ); }
bool operator <=( const gss_buffer_desc& a, const GSSBuffer& b       ) { return b >= a; }

bool operator >=( const GSSBuffer& a,       const GSSBuffer& b       ) { return ( a > b ) || ( a == b ); }
bool operator >=( const GSSBuffer& a,       const gss_buffer_desc& b ) { return ( a > b ) || ( a == b ); }
bool operator >=( const gss_buffer_desc& a, const GSSBuffer& b       ) { return b <= a; }

ostream& operator<< ( ostream& os, const GSSBuffer& buff )
{
	os.write( buff.bytes(), buff.size() );
	return os;
}

std::istream& operator>> ( std::istream& is, GSSBuffer& buff )
{
	string tmp;
	is >> tmp;
	buff.assign( tmp );
	return is;
}
