extern "C" {
#include <stdlib.h>  /* for abort() */
#include <stdio.h>  /* for fprintf, stderr */
#include "fail.h"

  static const char * FailStrings[] = {
    "zero unused",
#define XX(x) #x, 
#include "failCodes.h"
#undef XX
    "max failure code"
  };

  const char * MFMFailCodeReason(int failCode) {
    if (failCode < 0 || failCode >= (int) (sizeof(FailStrings)/sizeof(FailStrings[0])))
      return "[failCode out of range]";
    return FailStrings[failCode];
  }

  void MFMFailHere(const char * fname, const int lineno, int code) {
    fprintf(stderr,"%s:%d: FAILED: %s\n", fname, lineno, MFMFailCodeReason(code));
    abort();
  }
}