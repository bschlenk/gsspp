#ifndef __GSSMECH_H__
#define __GSSMECH_H__

#include <gssapi.h>
#include <string>
#include <iosfwd>

class GSSMech
{
 public:
	GSSMech() : _oid( GSS_C_NO_OID ) {}
	GSSMech( gss_OID oid ) : _oid( oid ) {}
	GSSMech( const std::string& mech_name );

	size_t size() const { return _oid->length; }
	std::string str() const;

	operator gss_OID  () { return _oid;   }
	operator gss_OID *() { return &_oid;  }

 private:
	gss_OID _oid;
};

std::ostream& operator<< ( std::ostream& os, const GSSMech& mech );

class GSSMechList
{
 public:
	GSSMechList();
	~GSSMechList() { clear(); }

	void   inquire();
	void   clear();
	size_t size()  const { return _oid_set->count; }
	bool   empty() const { return !size(); }

	void add( const GSSMech& mech );
	bool contains( const GSSMech& mech ) const;
	const GSSMech operator[]( size_t index ) const;
	const GSSMech at( size_t index ) const;

	GSSMechList& operator+= ( const GSSMech& mech );

	operator gss_OID_set  () { return _oid_set;  }
	operator gss_OID_set *() { return &_oid_set; }

 private:
	gss_OID_set _oid_set;
};

#endif // __GSSMECH_H__

