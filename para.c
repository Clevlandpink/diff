//
//  para.c
//  diff
//
//  Created by William McCarthy on 5/9/19.
//  Copyright © 2019 William McCarthy. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "para.h"
#include "util.h"

#define BUFLEN 256


para* para_make(char* base[], int filesize, int start, int stop) {
  para* p = (para*) malloc(sizeof(para));
  p->base = base;
  p->filesize = filesize;
  p->start = start;
  p->stop = stop;
  p->firstline = (p == NULL || start < 0) ? NULL : p->base[start];
  p->secondline = (p == NULL || start < 0 || filesize < 2) ? NULL : p->base[start + 1];
  
  return p;
}

para* para_first(char* base[], int size) {
  para* p = para_make(base, size, 0, -1);
  return para_next(p);
}

void para_destroy(para* p) { free(p); }

para* para_next(para* p) {
  if (p == NULL || p->stop == p->filesize) { return NULL; }
  
  int i;
  para* pnew = para_make(p->base, p->filesize, p->stop + 1, p->stop + 1);
  for (i = pnew->start; i < p->filesize && strcmp(p->base[i], "\n") != 0; ++i) { }
  pnew->stop = i;
  
  if (pnew->start >= p->filesize) {
    free(pnew);
    pnew = NULL;
  }
  return pnew;
}
size_t para_filesize(para* p) { return p == NULL ? 0 : p->filesize; }

size_t para_size(para* p) { return p == NULL || p->stop < p->start ? 0 : p->stop - p->start + 1; }

char** para_base(para* p) { return p->base; }

char* para_info(para* p) {
  static char buf[BUFLEN];   // static for a reason
  snprintf(buf, sizeof(buf), "base: %p, filesize: %d, start: %d, stop: %d\n",
           p->base, p->filesize, p->start, p->stop);
  return buf;  // buf MUST be static
}

int para_equal(para* p, para* q) {
  if (p == NULL || q == NULL) { return 0; }
  if (para_size(p) != para_size(q)) { return 0; }
  if (p->start >= p->filesize || q->start >= q->filesize) { return 0; }
  int i = p->start, j = q->start, equal = 0;
  while ((equal = strcmp(p->base[i], q->base[i])) == 0) { ++i; ++j; }
  return 1;
}

void para_print(para* p, para* q, void (*fp)(const char*, const char*)) {
  if (p == NULL && q == NULL) { return; }
  if(q == NULL){
    for (int i = p->start; i <= p->stop && i != p->filesize; ++i) { fp(p->base[i], NULL); }
  }
  else{
    int j = q->start;
    for (int i = p->start; i <= p->stop && i != p->filesize; ++i, ++j) { fp(p->base[i], q->base[j]); }
  }
}

void para_printdiffs(para* p, para* q, void (*fp)(const char*, const char*)) {
  if (p == NULL || q == NULL) { return; }
  int j = q->start;
  for (int i = p->start; i <= p->stop && i != p->filesize; ++i, ++j) {
    fp(p->base[i], q->base[j]);
  }
}

void para_printdiff(const para* p, const para* q, void (*fp)(const char*, const char*)) {
  char *b1[BUFLEN], *b2[BUFLEN];
  int c1 = 0, c2 = 0;
  int i = p->start, j = q->start, equal = 0;
  while ((equal = strcmp(p->base[i], q->base[j])) == 0) { ++i; ++j; }
  while (q->base[i] != NULL && p->base[i] != NULL && (equal = strcmp(p->base[i], q->base[j])) != 0) {
    b1[c1] = p->base[i]; b2[c2] = q->base[j];
    ++i; ++j; ++c1; ++c2;
  }
  fp(*b1, *b2);
  return;
}

void para_printfile(char* base[], int count, void (*fp)(const char*, const char*)) {
  para* p = para_first(base, count);
  while (p != NULL) {
    para_print(p, NULL, fp);
    p = para_next(p);
  }
  printline();
}

