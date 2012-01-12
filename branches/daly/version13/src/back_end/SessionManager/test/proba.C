// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fstream.h>
#include "global.h"
#include "MagnusHome.h"
#include "Vector.h"
#include "Word.h"
#include "Set.h"


int main ( )
{
  Chars s = MagnusHome::magnusHome() + "/back_end/black_boxes/orwp/";
  char *ex = s + "orwp";
  char *in = s + "in";
  char *out = s + "out";
  char *cmdline = "./orwp < ./in"; 
  
  execl("/bin/sh", "sh", "-c", cmdline, 0);
}
