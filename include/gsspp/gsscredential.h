#ifndef __GSSCREDENTIAL_H__
#define __GSSCREDENTIAL_H__


#include <gssapi.h>

class GSSName;

class GSSCredential
{
 public:
	GSSCredential() : _credential( GSS_C_NO_CREDENTIAL ) {}
	GSSCredential( gss_cred_id_t cred ) : _credential( cred ) {}
	GSSCredential( const GSSName& name );

	~GSSCredential() { clear(); }

	void clear();
	
	
	operator gss_cred_id_t  () { return _credential;  }
	operator gss_cred_id_t *() { return &_credential; }
	
 private:
	// no copy function for credential
	GSSCredential( const GSSCredential& );
	GSSCredential& operator = ( const GSSCredential& );

	gss_cred_id_t _credential;
};

#endif // __GSSCREDENTIAL_H__
