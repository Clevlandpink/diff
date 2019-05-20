//
//  util.h
//  diff
//
//  Created by William McCarthy on 5/9/19.
//  Copyright © 2019 William McCarthy. All rights reserved.
//

#ifndef util_h
#define util_h

#include <stdio.h>
#define BUFLEN 256

char* yesorno(int condition);
FILE* openfile(const char* filename, const char* openflags);

void printleft(const char* left, const char* left2);
void printright(const char* right, const char* right2);
void printboth(const char* left, const char* right);
void printdif(const char* p, const char* q);
void printleftcol(const char* left, const char* right);
void printline(void);
void suppresscommonline(const char* left, const char* right);
void printra(const char* right, const char* right2);
void printla(const char* right, const char* right2);

#endif /* util_h */
