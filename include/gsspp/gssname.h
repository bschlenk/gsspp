#ifndef __GSSNAME_H__
#define __GSSNAME_H__

#include <gssapi.h>
#include <string>
#include <iosfwd>

class GSSBuffer;
class GSSMech;

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
		// KRB5_PRINCIPAL_NAME,
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
	bool empty() const { return _name == GSS_C_NO_NAME; }
	bool valid() const { return !empty(); }

	std::string str( NameType * nt = 0 ) const;
	GSSName canonicalize( const GSSMech& mech ) const;
	GSSBuffer export_name( const GSSMech& mech ) const;
	void import_name( const GSSBuffer& buff, NameType nt );

	bool operator== ( const GSSName& name    ) const;
	bool operator== ( const std::string& str ) const;
	bool operator!= ( const GSSName& name    ) const;
	bool operator!= ( const std::string& str ) const;

 private:
	gss_name_t _name;

	static const gss_OID _oid_types[];
};

bool operator== ( const std::string& str, const GSSName& name );
bool operator!= ( const std::string& str, const GSSName& name );

std::ostream& operator<< ( std::ostream& os, const GSSName& name );


#endif // __GSSNAME_H__
