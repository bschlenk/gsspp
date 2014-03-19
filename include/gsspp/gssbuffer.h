#ifndef __GSSBUFFER_H__
#define __GSSBUFFER_H__


#include <gssapi.h>
#include <string>
#include <iosfwd>
#include <cassert>

class GSSBuffer
{
 public:
	// constructors
	GSSBuffer() : _buff() {}
	GSSBuffer( const GSSBuffer& other );
	GSSBuffer( const gss_buffer_desc& buff );
	GSSBuffer( size_t len );
	GSSBuffer( const char * s );
	GSSBuffer( const void * s, size_t len );
	GSSBuffer( const std::string& str );
	GSSBuffer( std::istream& is, size_t len );

	// assignment operators
	GSSBuffer& operator= ( const GSSBuffer& other )       { assign( other ); }
	GSSBuffer& operator= ( const gss_buffer_desc& other ) { assign( other ); }
	GSSBuffer& operator= ( const char * other )           { assign( other ); }
	GSSBuffer& operator= ( const std::string& other )     { assign( other ); }

	~GSSBuffer() { clear(); }
	
	void assign( const GSSBuffer& );
	void assign( const gss_buffer_desc& );
	void assign( size_t len );
	void assign( const char * );
	void assign( void *, size_t len );
	void assign( const std::string& );
	void assign( std::istream& is, size_t len );

	void swap( GSSBuffer& other );
	void swap( gss_buffer_desc& other );

	void   clear();
	size_t size()  const { return _buff.length; }
	bool   empty() const { return !size(); }

	std::string    str() const { return std::string( (char *)_buff.value, _buff.length ); }
	const char * bytes() const { return (char *)_buff.value; }
	//const gss_buffer_desc_struct * raw() const { return &_buff; }
	//gss_buffer_desc_struct *       raw()       { return &_buff; }
	const gss_buffer_desc_struct * raw() const { return &_buff; }
	gss_buffer_desc_struct *       raw()       { return &_buff; }

	operator  std::string() const { return str();   }
	operator const char *() const { return bytes(); }
	operator const gss_buffer_desc_struct *() const { return &_buff; }
	operator       gss_buffer_desc_struct *()       { return &_buff; }
	operator void *() { return _buff.value; }

	template <typename T>
	T get() { assert( sizeof( T ) <= _buff.length ); return *(T *)_buff.value; }

	template <typename T>
	T& get( T& val ) { assert( sizeof( T ) <= _buff.length ); val = *(T *)_buff.value; return val; }

 private:
	gss_buffer_desc _buff;
};


bool operator ==( const GSSBuffer& a,       const GSSBuffer& b       );
bool operator ==( const GSSBuffer& a,       const gss_buffer_desc& b );
bool operator ==( const gss_buffer_desc& a, const GSSBuffer& b       );

bool operator !=( const GSSBuffer& a,       const GSSBuffer& b       );
bool operator !=( const GSSBuffer& a,       const gss_buffer_desc& b );
bool operator !=( const gss_buffer_desc& a, const GSSBuffer& b       );

bool operator < ( const GSSBuffer& a,       const GSSBuffer& b       );
bool operator < ( const GSSBuffer& a,       const gss_buffer_desc& b );
bool operator < ( const gss_buffer_desc& a, const GSSBuffer& b       );

bool operator > ( const GSSBuffer& a,       const GSSBuffer& b       );
bool operator > ( const GSSBuffer& a,       const gss_buffer_desc& b );
bool operator > ( const gss_buffer_desc& a, const GSSBuffer& b       );

bool operator <=( const GSSBuffer& a,       const GSSBuffer& b       );
bool operator <=( const GSSBuffer& a,       const gss_buffer_desc& b );
bool operator <=( const gss_buffer_desc& a, const GSSBuffer& b       );

bool operator >=( const GSSBuffer& a,       const GSSBuffer& b       );
bool operator >=( const GSSBuffer& a,       const gss_buffer_desc& b );
bool operator >=( const gss_buffer_desc& a, const GSSBuffer& b       );

std::ostream& operator<< ( std::ostream& os, const GSSBuffer& gss_buff );
std::istream& operator>> ( std::istream& is, GSSBuffer& gss_buff );


#endif // __GSSBUFFER_H__
