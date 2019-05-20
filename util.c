//
//  util.c
//  diff
//
//  Created by William McCarthy on 5/9/19.
//  Copyright © 2019 William McCarthy. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include "util.h"

#define BUFLEN 256


char* yesorno(int condition) { return condition == 0 ? "no" : "YES"; }

FILE* openfile(const char* filename, const char* openflags) {
  FILE* f;
  if ((f = fopen(filename, openflags)) == NULL) {  printf("can't open '%s'\n", filename);  exit(1); }
  return f;
}

void printline(void) {
  for (int i = 0; i < 10; ++i) { printf("=========="); }
  printf("\n");
}


void printleft(const char* left, const char* left2) {
  if(left == NULL){left = left2;}
  char buf[BUFLEN];
  
  strcpy(buf, left);
  int j = 0, len = (int)strlen(buf) - 1;
  for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' '; }
  buf[len + j++] = '<';
  buf[len + j++] = '\0';
  printf("%s\n", buf);
}

void printleftcol(const char* left, const char* right){
  if(left == NULL || right == NULL) {return;}
  char buf[BUFLEN], buf2[BUFLEN];
  
  strcpy(buf, left);
  strcpy(buf2, left);

  if(strcmp(left, right) == 0){  
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 48 - len ; ++j) { buf[len + j] = ' ';}
    buf[len + j++] = '(';
    buf[len + j++] = '\0';

    for(int i = 0; i < (int)strlen(buf2) - 1; ++i){
      buf2[i] = ' ';
    }
    printf("%-50s %s", buf, buf2);
  }
  else{
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 47 - len ; ++j) { buf[len + j] = ' ';}
    buf[len + j++] = '\0';
    printf("%-47s | %s", buf, buf2);
  }
  
}

void suppresscommonline(const char* left, const char* right){
  if(left == NULL || right == NULL) {return;}
  char buf[BUFLEN], buf2[BUFLEN];
  
  strcpy(buf, left);
  strcpy(buf2, left);

  if(strcmp(left, right) == 0){
    return;
  }
  else{
    int j = 0, len = (int)strlen(buf) - 1;
    for (j = 0; j <= 47 - len ; ++j) { buf[len + j] = ' ';}
    buf[len + j++] = '\0';
    printf("%-47s | %s", buf, buf2);
  }
  
}

void printright(const char* right, const char* right2) {
  if (right == NULL && right2 == NULL) { return; }
  else if(right == NULL){right = right2;}
  printf("%50s %s", ">", right);
}

void printra(const char* right, const char* right2) {
  if (right == NULL && right2 == NULL) { return; }
  else if(right == NULL){right = right2;}
  printf("%s %s", ">", right);
}

void printla(const char* right, const char* right2) {
  if (right == NULL && right2 == NULL) { return; }
  else if(right == NULL){right = right2;}
  printf("%s %s", "<", right);
}

void printboth(const char* left, const char *right) {
  char buf[BUFLEN];
  size_t len = strlen(left);
  if (len > 0) { strncpy(buf, left, len); }
  buf[len - 1] = '\0';
  if(strcmp(left, right) == 0){
    printf("%-50s %s", buf, right);
  }
  else{
    printf("%-48s | %s", buf, right);
  }
}

void printdifs(const char* p, const char* q) {
  char buf[BUFLEN];
  size_t len = strlen(p);
  if (len > 0) { strncpy(buf, p, len); }
  buf[len - 1] = '\0';
  printf("%-50s %s", buf, q);
}

void printdif(const char* left, const char* right){
  if(left == NULL || right == NULL) {return;}
  char buf[BUFLEN], buf2[BUFLEN];
  
  strcpy(buf, left);
  strcpy(buf2, left);

  if(strcmp(left, right) == 0){
    return;
  }
  else{
    printf("< %s\n> %s>\n", left, right);
  }
  
}

