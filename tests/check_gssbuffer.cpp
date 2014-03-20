#include "gsspp/gssbuffer.h"
#include <check.h>
#include <cassert>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
using namespace std;

START_TEST( test_gssbuffer )
{
	GSSBuffer buff;
	assert( buff.size() == 0 );

	const char * cTestStr = "hello test";
	buff.assign( cTestStr );

	assert( buff.size() == strlen( cTestStr ) );
	assert( 0 == memcmp( buff.bytes(), cTestStr, buff.size() ) );
	// assert( strcmp( buff.get<char *>(), testStr ) );
	
	string testStr( cTestStr );

	GSSBuffer testStrBuff( testStr );
	assert( buff == testStrBuff );

	testStr += " number 2";
	testStrBuff = testStr;
	assert( buff < testStrBuff );

	stringstream ss( testStr );
	GSSBuffer ssBuff( ss, testStr.size() / 2 );
	assert( ssBuff == testStr.substr( 0, testStr.size() / 2 ) );

	cout << ssBuff << endl;
}
END_TEST
