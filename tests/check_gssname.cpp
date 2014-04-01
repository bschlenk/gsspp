#include "gsspp/gssname.h"
#include "gsspp/gssbuffer.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
	GSSName name;
	assert( name.empty() );
	assert( !name.valid() );

	name = "host";
	assert( !name.empty() );
	assert( name.valid() );

	GSSName name2( "host" );
	assert( !name2.empty() );
	assert( name2.valid() );

	assert( name == name2 );
	name2 = "brians";

	assert( name != name2 );

	GSSName::NameType nt;
	string n = name.str( &nt );

	//assert( nt == GSSName::HOSTBASED_SERVICE );
	cout << nt << endl;
	
	cout << n << endl;

	return 0;
}
