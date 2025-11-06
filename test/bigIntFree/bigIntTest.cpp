#include <iostream>
#include <string>
#include <stdio.h>

#include "ooasn1.h"

using namespace std;

void pause(void)
{
	cerr << "Press any key to continue.";
	getc(stdin);
	cerr << endl;
}


int main(int argc, char **argv)
{
	// RSA modulus as decoded from a certificate
	char * bigint = "0x0dfa165428bf757f5a75c3c46933ffd1"
					  "85c1e8a7707fe7d396dde3be5f3c2eac"
					  "49e75bfe85c3d9bf868d31cfdc9c1946"
					  "99cc3f3e895dfaae5124c34f84c6d4b4"
					  "9c4aff4b779f0ac3e61776a371522c98"
					  "5db7e9a4a938d8e79671c6b8e0d8186b"
					  "1f6be621acc41ba68eb308907a348399"
					  "e4fc61b2ba714f884dfc0708a5fff776"
					  "5d864f69de020c71adf87619fbaa8634"
					  "c94c797af83261edcbe882e8c8a33cff"
					  "09fe36df5b3961da28d7035b27507a70"
					  "e24dd4e25a795916fbdbf5a4bdcf2df0"
					  "3232408e2adb828d8d5ca3efd0430a70"
					  "828d1024e7ca4a8049c00934a6a63e4f"
					  "650fd11f6b7d1dbcabec18db49a0147d"
					  "c81249b2e47c27d493956151e47157b1f";
	OOCTXT keyctx;
	initContext(&keyctx);
	ASN1BigInt key;
	bigIntInit(&key);

	if(0 > bigIntSetStr(&keyctx,&key,bigint,0)) {
		bigIntFree(&keyctx,&key);
		freeContext(&keyctx);
		cerr << "Big integer from hex-encoded public key could not be parsed" << endl;
		return 1;
	}

	size_t keyOctects = key.numocts;
	cout << "RSA modulus is " << keyOctects * 8 << " bits long" << endl;
	// pause();
	bigIntFree(&keyctx,&key); // causes heap corruption
	freeContext(&keyctx);
	return 0;
}



