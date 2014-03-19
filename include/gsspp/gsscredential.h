#ifndef __GSSCREDENTIAL_H__
#define __GSSCREDENTIAL_H__


#include <gssapi.h>

class GSSCredential
{
 public:
	GSSCredential() : _credential( GSS_C_NO_CREDENTIAL ) {}
	GSSCredential( const GSSCredential& );
	GSSCredential( gss_cred_id_t_desc_struct * cred ) : _credential( cred ) {}
	
	
	operator gss_cred_id_t  () { return _credential;  }
	operator gss_cred_id_t *() { return &_credential; }
	
 private:
	gss_cred_id_t_desc_struct * _credential;
};

#endif // __GSSCREDENTIAL_H__
