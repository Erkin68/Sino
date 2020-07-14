#ifndef _PROFILER_H__
#define _PROFILER_H__

//#define PROFILER

#pragma warning( disable : 4996)


#ifdef PROFILER
#include <stdio.h>
static FILE *fProfiler = fopen("Profiler.txt", "w");
#define ProfilerEnd	fclose(fProfiler);
#define profilerOutStr(x) fprintf(fProfiler, "\n%s", x);
#define profilerPrintf1(x,y) fprintf(fProfiler,x,y);
#define profilerPrintf2(x,y,z) fprintf(fProfiler,x,y,z);
#define profilerPrintf3(x,y,z,a) fprintf(fProfiler,x,y,z,a);
#define profilerPrintf4(x,y,z,a,b) fprintf(fProfiler,x,y,z,a,b);
#define profilerPrintf5(x,y,z,a,b,c) fprintf(fProfiler,x,y,z,a,b,c);
#define profilerPrintf6(x,y,z,a,b,c,d) fprintf(fProfiler,x,y,z,a,b,c,d);
#define profilerPrintf7(x,y,z,a,b,c,d,e) fprintf(fProfiler,x,y,z,a,b,c,d,e);
#define profilerPrintf8(x,y,z,a,b,c,d,e,f) fprintf(fProfiler,x,y,z,a,b,c,d,e,f);

#else
#define profilerPrintf1(x,y)
#define profilerPrintf2(x,y,z)
#define profilerPrintf3(x,y,z,a)
#define profilerPrintf4(x,y,z,a,b)
#define profilerPrintf5(x,y,z,a,b,c)
#define profilerPrintf6(x,y,z,a,b,c,d)
#define profilerPrintf7(x,y,z,a,b,c,d,e)
#define profilerPrintf8(x,y,z,a,b,c,d,e,f)

#define ProfilerEnd
#define profilerOutStr(x)
#define profilerPrintf(x, y)






#endif

#pragma warning( default : 4996)

#endif
