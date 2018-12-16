#include<cstring>
#include<catch.hpp>
#include<array>
#include<iostream>
#include<netinet/in.h>
#include"src/sha1.h"
#include"src/aes.h"
using namespace std;

TEST_CASE("sha1")
{
	SHA1 sha;
	char c[] = "abc";
    union {
		uint32_t k[5];
		array<uint8_t, 20> ar;
	} u;
	u.ar = sha.hash(c, c);
	//da39a3ee 5e6b4b0d 3255bfef 95601890 afd80709
	REQUIRE(u.k[0] == ntohl(0xda39a3ee));
	REQUIRE(u.k[1] == ntohl(0x5e6b4b0d));
	REQUIRE(u.k[2] == ntohl(0x3255bfef));
	REQUIRE(u.k[3] == ntohl(0x95601890));
	REQUIRE(u.k[4] == ntohl(0xafd80709));
}

TEST_CASE("aes128")
{
	AES aes;
	unsigned char key[] = "this is the key hello";
	unsigned char iv[] = "ivivivivviviviviviviviviv";
	unsigned char msg[] = "Test message TSSSSSS";
	aes.key(key);
	aes.iv(iv);
	aes.encrypt(msg, 16);
	REQUIRE(memcmp(msg, "Test message TSSSSSS", 16));
	aes.decrypt(msg, 16);
	REQUIRE(!memcmp(msg, "Test message TSSSSSS", 16));
}
