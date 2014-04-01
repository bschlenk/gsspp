#include "gsspp/gssmech.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>
#include <cstdio>
#include <cassert>

int main()
{
	try
	{
		GSSMechList mechList;
		assert( mechList.size() > 0 );
		printf( "num mechs: %zu\n", mechList.size() );
		for ( size_t i = 0; i < mechList.size(); ++i )
		{
			gss_OID oid = mechList[ i ];
			assert( oid );
		}
	}
	catch ( GSSException& e )
	{
		assert( false );
	}

	return 0;
}
