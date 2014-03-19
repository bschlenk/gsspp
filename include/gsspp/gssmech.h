#ifndef __GSSMECH_H__
#define __GSSMECH_H__


#include <gssapi.h>

class GSSMech
{
 public:
	GSSMech() : _oid( GSS_C_NO_OID ) {}
	GSSMech( gss_OID_desc_struct * oid ) : _oid( oid ) {}

	operator gss_OID_desc_struct  () { return *_oid; }
	operator gss_OID_desc_struct *() { return _oid;  }

 private:
	gss_OID_desc_struct * _oid;
};

#endif // __GSSMECH_H__

