#include "File.h"
#include <iostream>

#define BUFSIZE 1000
void dumpstream(istream& src, ostream& trg) {
	char buf[BUFSIZE];
	unsigned long sz(1);
	while(src.good() && (sz > 0)) {
		sz = src.readsome(buf, BUFSIZE);
		trg.write(buf, sz);
	}
}
