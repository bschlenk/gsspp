#include "gsspp/gssmech.h"
#include "gsspp/gssexception.h"
#include <gssapi.h>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cassert>
using namespace std;

int main()
{
	GSSMechList mechList;
	assert( mechList.empty() );

	cout << "got here" << endl;

	mechList.inquire();
	assert( mechList.size() > 0 );

	cout << "got here 2" << endl;
	printf( "num mechs: %zu\n", mechList.size() );
	for ( size_t i = 0; i < mechList.size(); ++i )
	{
		GSSMech mech( mechList[i] );
		cout << mech.size() << " bytes: " << mech << endl;
	}

	try
	{
		mechList.at( mechList.size() );
		assert( false );
	}
	catch ( std::out_of_range& e )
	{
		assert( true );
	}

	return 0;
}
