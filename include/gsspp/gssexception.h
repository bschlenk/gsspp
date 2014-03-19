#ifndef __GSSEXCEPTION__
#define __GSSEXCEPTION__


#include <gssapi.h>
#include <exception>

class GSSException : public std::exception
{
 public:
	GSSException() throw() : major_status( GSS_S_COMPLETE ), minor_status( GSS_S_COMPLETE ) {}
	GSSException( OM_uint32 maj, OM_uint32 min, const char * func = "" ) throw();

	virtual ~GSSException() throw() {}

	virtual const char * what() const throw();
	
	OM_uint32 major_status;
	OM_uint32 minor_status;
	char major_message[128];
	char minor_message[128];
	char function[64];
};


#endif // __GSSEXCEPTION__
