#ifndef __GSSMECH_H__
#define __GSSMECH_H__

#include <gssapi.h>
#include <string>

class GSSMech
{
 public:
	GSSMech() : _oid( GSS_C_NO_OID ) {}
	GSSMech( gss_OID_desc_struct * oid ) : _oid( oid ) {}
	GSSMech( const std::string& mech_name );

	operator gss_OID  () { return _oid; }
	operator gss_OID *() { return &_oid;  }

 private:
	gss_OID _oid;
};

#endif // __GSSMECH_H__

