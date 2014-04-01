#ifndef __GSSMECH_H__
#define __GSSMECH_H__

#include <gssapi.h>
#include <string>
#include <vector>

class GSSMech
{
 public:
	GSSMech() : _oid( GSS_C_NO_OID ) {}
	GSSMech( gss_OID_desc_struct * oid ) : _oid( oid ) {}
	GSSMech( const std::string& mech_name );

	operator gss_OID  () { return _oid;   }
	operator gss_OID *() { return &_oid;  }

 private:
	gss_OID _oid;
};

class GSSMechList
{
 public:
	GSSMechList()  { inquire(); }
	~GSSMechList() { clear();   }

	void     inquire();
	void     clear();
	size_t   size() { return _oid_set->count; }
	gss_OID operator[]( size_t index );
	gss_OID at( size_t index );

	operator gss_OID_set  () { return _oid_set;  }
	operator gss_OID_set *() { return &_oid_set; }

 private:
	gss_OID_set _oid_set;
};

#endif // __GSSMECH_H__

