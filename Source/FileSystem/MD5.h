/*
* This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
* MD5 Message-Digest Algorithm (RFC 1321).
*
* Homepage:
* http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
*
* Author:
* Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
*
* This software was written by Alexander Peslyak in 2001.  No copyright is
* claimed, and the software is hereby placed in the public domain.
* In case this attempt to disclaim copyright and place the software in the
* public domain is deemed null and void, then the software is
* Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
* general public under the following terms:
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted.
*
* There's ABSOLUTELY NO WARRANTY, express or implied.
*
* See md5.c for more information.
*/

#ifndef MD5_H
#define MD5_H

#include <string>

#if defined(WIN32)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#else
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#endif

namespace FileSystem
{
	namespace MD5
	{
		/* Any 32-bit or wider unsigned integer data type will do */
		typedef unsigned int MD5_u32plus;

		typedef struct {
			MD5_u32plus lo, hi;
			MD5_u32plus a, b, c, d;
			unsigned char buffer[64];
			MD5_u32plus block[16];
		} MD5_CTX;

		void EXPORT MD5_Print(unsigned char *md5);
		void EXPORT MD5_Print(std::string md5);
		void EXPORT MD5_Init(MD5_CTX *ctx);
		void EXPORT MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size);
		void EXPORT MD5_Final(unsigned char *result, MD5_CTX *ctx);
		
		EXPORT std::string MD5(const void *data, unsigned long size);
		EXPORT std::string MD5_File(std::string _file);

	}
}
#endif