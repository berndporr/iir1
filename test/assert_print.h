#pragma once
#include <cstdio>
#include <cstdlib>
inline void assert_print(int b, const char* t) {
	if (b) return;
	fprintf(stderr,"%s\n",t);
	exit(-1);
}

// placed there so that it only affects the cpp files
using namespace std;
