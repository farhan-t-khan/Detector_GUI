#pragma once

#include <map>
#define PROFILER_FILE "c:\\BascomTurnerMLDS\\Data\\profiler.txt"
#define USE_PROFILER
#define PROFILER_OPEN_NOTEPAD
struct ltsztr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};
class CProfiler;
struct structProfiler
{
   structProfiler(): dTotalTime(0.0), nFunctionCalls(0), pRunning(0){};
   double dTotalTime;
   int nFunctionCalls;
   CProfiler* pRunning;
};

struct struct2Profiler
{
   struct2Profiler(const char* szFN, structProfiler profiler): szFunctName(szFN), refProfiler(profiler){};
   const char* szFunctName;
   structProfiler refProfiler;
};

class CProfiler
{
public:
   CProfiler(const char* szFunctionName);
   ~CProfiler(void);
   static std::map<const char*, structProfiler, ltsztr> m_st_TimesMap;
   static void PrintProfileInfo();
   static void PrintProfileInfo(char* szFileName, int nDisplay);
   static void RestartAll(void);
   static bool m_st_bDoNothing;
   static int m_st_nCounter;
   //use ResetStart and AddTime only for none recursive calls
   void ResetStart(void);
   void Pause(void);
   void AddTime(void);
   void IgnoreDestructor(void) { m_bIgnoreDestructor = true; };
protected:
   LARGE_INTEGER m_clockStart;
   LARGE_INTEGER m_frequency;        // ticks per second
   structProfiler* pstructProfiler;
   int m_nIamRunning;
   bool m_bIgnoreDestructor;
};
