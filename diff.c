//
//  diff_02.c
//  diff
//
//  Created by William McCarthy on 4/29/19.
//  Copyright © 2019 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGC_ERROR 1
#define TOOMANYFILES_ERROR 2
#define CONFLICTING_OUTPUT_OPTIONS 3

#define MAXSTRINGS 1024
#define MAXPARAS 4096

#define HASHLEN 200

#include "diff.h"
#include "para.h"
#include "util.h"

char fn1[BUFLEN], fn2[BUFLEN];

void version(void) {
  printf("\n\n\ndiff (CSUF diffutils) 1.0.0\n");
  printf("Copyright (C) 2014 CSUF\n");
  printf("This program comes with NO WARRANTY, to the extent permitted by law.\n");
  printf("You may redistribute copies of this program\n");
  printf("under the terms of the GNU General Public License.\n");
  printf("For more information about these matters, see the file named COPYING.\n");
  printf("Written by William McCarthy, Tony Stark, Idean Saghatchi, and Dr. Steven Strange\n");
}

char buf[BUFLEN];
char *strings1[MAXSTRINGS], *strings2[MAXSTRINGS];
int showversion = 0, showbrief = 0, ignorecase = 0, report_identical = 0, showsidebyside = 0;
int showleftcolumn = 0, showunified = 0, showcontext = 0, suppresscommon = 0, diffnormal = 0;

int count1 = 0, count2 = 0;


void loadfiles(const char* filename1, const char* filename2) {
  char *p1;
  const char *p2;
  memset(buf, 0, sizeof(buf));
  memset(strings1, 0, sizeof(strings1));
  memset(strings2, 0, sizeof(strings2));

  
  p1 = fn1; p2 = filename1;
  while((*p1++ = *p2++)){}
  p1 = fn2; p2 = filename2;
  while((*p1++ = *p2++)){}
  
  FILE *fin1 = openfile(filename1, "r");
  FILE *fin2 = openfile(filename2, "r");
  
  while (!feof(fin1) && fgets(buf, BUFLEN, fin1) != NULL) { strings1[count1++] = strdup(buf); }  fclose(fin1);
  while (!feof(fin2) && fgets(buf, BUFLEN, fin2) != NULL) { strings2[count2++] = strdup(buf); }  fclose(fin2);

  
}

void print_option(const char* name, int value) { printf("%17s: %s\n", name, yesorno(value)); }

void diff_output_conflict_error(void) {
  fprintf(stderr, "diff: conflicting output style options\n");
  fprintf(stderr, "diff: Try `diff --help' for more information.)\n");
  exit(CONFLICTING_OUTPUT_OPTIONS);
}

void setoption(const char* arg, const char* s, const char* t, int* value) {
  if ((strcmp(arg, s) == 0) || ((t != NULL && strcmp(arg, t) == 0))) {
    *value = 1;
  }
}

void showoptions(const char* file1, const char* file2) {
  printf("diff options...\n");
  print_option("diffnormal", diffnormal);
  print_option("show_version", showversion);
  print_option("show_brief", showbrief);
  print_option("ignorecase", ignorecase);
  print_option("report_identical", report_identical);
  print_option("show_sidebyside", showsidebyside);
  print_option("show_leftcolumn", showleftcolumn);
  print_option("suppresscommon", suppresscommon);
  print_option("showcontext", showcontext);
  print_option("show_unified", showunified);
  
  printf("file1: %s,  file2: %s\n\n\n", file1, file2);
  
  printline();
}


void init_options_files(int argc, const char* argv[]) {
  int cnt = 0;
  const char* files[2] = { NULL, NULL };
  
  while (argc-- > 0) {
    const char* arg = *argv;
    setoption(arg, "-v",       "--version",                  &showversion);
    setoption(arg, "-q",       "--brief",                    &showbrief);
    setoption(arg, "-i",       "--ignore-case",              &ignorecase);
    setoption(arg, "-s",       "--report-identical-files",   &report_identical);
    setoption(arg, "--normal", NULL,                         &diffnormal);
    setoption(arg, "-y",       "--side-by-side",             &showsidebyside);
    setoption(arg, "--left-column", NULL,                    &showleftcolumn);
    setoption(arg, "--suppress-common-lines", NULL,          &suppresscommon);
    setoption(arg, "-c",       "--context",                  &showcontext);
    setoption(arg, "-u",       "showunified",                &showunified);
    if (arg[0] != '-') {
      if (cnt == 2) {
        fprintf(stderr, "apologies, this version of diff only handles two files\n");
        fprintf(stderr, "Usage: ./diff [options] file1 file2\n");
        exit(TOOMANYFILES_ERROR);
      } else { files[cnt++] = arg; }
    }
    ++argv;   // DEBUG only;  move increment up to top of switch at release
  }

  if (!showcontext && !showunified && !showsidebyside && !showleftcolumn) {
    diffnormal = 1;
  }
  
  if (showversion) { version();  exit(0); }
  
  if (((showsidebyside || showleftcolumn) && (diffnormal || showcontext || showunified)) ||
      (showcontext && showunified) || (diffnormal && (showcontext || showunified))) {

    diff_output_conflict_error();
  }
  
//  showoptions(files[0], files[1]);
  loadfiles(files[0], files[1]);
}


int main(int argc, const char * argv[]) {
  init_options_files(--argc, ++argv);
  int dif = 0;
  para *p = para_first(strings1, count1);
  para *q = para_first(strings2, count2);

  int foundmatch = 0;

  para *qlast = q;

  while (p != NULL) {
    qlast = q;
    foundmatch = 0;
    
    while (q != NULL && (foundmatch = para_equal(p, q)) == 0) {
      q = para_next(q);
      dif = 1;
      if(showbrief){
	printf("Files %s and %s differ\n", fn1, fn2);
	exit(0);
      }
    }

    q = qlast;

    if (foundmatch) {
      while ((foundmatch = para_equal(p, q)) == 0) {
	dif = 1;
	if(diffnormal){para_print(q, NULL, printra);}
	else{para_print(q, NULL, printright);}
        q = para_next(q);
        qlast = q;
      }
      if(diffnormal){ para_print(p, q, printdif);}
      else if(showleftcolumn || (showleftcolumn && showsidebyside)){ para_print(p, q, printleftcol);}
      else if(suppresscommon){para_print(p, q, suppresscommonline);}
      else{para_print(p, q, printboth);}
      p = para_next(p);
      q = para_next(q);
    }
    else {
      if(diffnormal){para_print(p, NULL, printla);}
      else{para_print(p, NULL, printleft);}
      p = para_next(p);
    }
  }

  while (q != NULL) {
    if(diffnormal){para_print(q, NULL, printra);}
    else{para_print(q, NULL, printright);}
    q = para_next(q);
  }
  if(dif == 0 && report_identical){ printf("Files %s and %s are identical.\n", fn1, fn2);}
 
  return 0;
}
