#ifndef __GSSNAME_H__
#define __GSSNAME_H__

#include <gssapi.h>
#include <string>

class GSSBuffer;

class GSSName
{
 public:

	enum NameType {
		NO_OID,
		USER_NAME,
		MACHINE_UID_NAME,
		STRING_UID_NAME,
		// HOSTBASED_SERVICE_X, // deprecated, use HOSTBASED_SERVICE
		HOSTBASED_SERVICE,
		ANONYMOUS,
		EXPORT_NAME,
	};

	// constructors
	GSSName() : _name( GSS_C_NO_NAME ) {}
	GSSName( const GSSName& );
	GSSName( const gss_name_t& );
	GSSName( const GSSBuffer&,   NameType nt = HOSTBASED_SERVICE );
	GSSName( const char *,       NameType nt = HOSTBASED_SERVICE );
	GSSName( const std::string&, NameType nt = HOSTBASED_SERVICE );

	// assignment operators
	GSSName& operator= ( const GSSName& other     ) { assign( other ); return *this; }
	GSSName& operator= ( const GSSBuffer& other   ) { assign( other ); return *this; }
	GSSName& operator= ( const gss_name_t& other  ) { assign( other ); return *this; }
	GSSName& operator= ( const char * other       ) { assign( other ); return *this; }
	GSSName& operator= ( const std::string& other ) { assign( other ); return *this; }

	~GSSName() { clear(); }

	void assign( const GSSName& );
	void assign( const gss_name_t& );
	void assign( const GSSBuffer&,   NameType nt = HOSTBASED_SERVICE );
	void assign( const char *,       NameType nt = HOSTBASED_SERVICE );
	void assign( const std::string&, NameType nt = HOSTBASED_SERVICE );

	void swap( GSSName& );
	void swap( gss_name_t& );

	operator       gss_name_t *()       { return &_name; }
	operator       gss_name_t  ()       { return _name;  }
	operator const gss_name_t  () const { return _name;  }

	void clear();
	bool empty() { return _name == GSS_C_NO_NAME; }
	bool valid() { return !empty(); }

 private:
	gss_name_t _name;

	static const gss_OID _oid_types[];
};


#endif // __GSSNAME_H__
