#include "StdAfx.h"
#include ".\profiler.h"
#include <algorithm>
#include <vector>

std::map<const char*, structProfiler, ltsztr> CProfiler::m_st_TimesMap;
bool CProfiler::m_st_bDoNothing = true;
int CProfiler::m_st_nCounter = 0;
CProfiler::CProfiler(const char* szFunctionName)
{
   if (m_st_bDoNothing)
   {
      return;
   }
   m_nIamRunning = 0;
   if (m_st_TimesMap.find(szFunctionName) == m_st_TimesMap.end())
   {
      m_st_TimesMap[szFunctionName] = structProfiler();
   }
   pstructProfiler = &m_st_TimesMap[szFunctionName];
   //m_clockStart = clock();

    // get ticks per second
    QueryPerformanceFrequency(&m_frequency);

    // start timer
    QueryPerformanceCounter(&m_clockStart);

   if (!pstructProfiler->pRunning)
   {
      m_nIamRunning = 1;
      pstructProfiler->pRunning=this;
   }
   m_bIgnoreDestructor = false;
}

CProfiler::~CProfiler(void)
{
   if (m_st_bDoNothing)
   {
      return;
   }
   if (m_bIgnoreDestructor)
   {
      return;
   }
   if (m_nIamRunning)
   {
      LARGE_INTEGER stop = {0};

      // stop timer
      QueryPerformanceCounter(&stop);
      // compute and print the elapsed time in microsec
      pstructProfiler->dTotalTime += (stop.QuadPart - m_clockStart.QuadPart) / (double)m_frequency.QuadPart;

      //pstructProfiler->dTotalTime += (clock() - m_clockStart) / double(CLOCKS_PER_SEC);
      pstructProfiler->pRunning = 0;
   }
   pstructProfiler->nFunctionCalls++;
}

void CProfiler::ResetStart(void)
{
   QueryPerformanceCounter(&m_clockStart);
}

void CProfiler::AddTime(void)
{
   if (m_st_bDoNothing)
   {
      return;
   }
   if (m_nIamRunning)
   {
      LARGE_INTEGER stop = {0};
      // stop timer
      QueryPerformanceCounter(&stop);
      // compute and print the elapsed time in microsec
      pstructProfiler->dTotalTime += (stop.QuadPart - m_clockStart.QuadPart) / (double)m_frequency.QuadPart;

      //pstructProfiler->dTotalTime += (clock() - m_clockStart) / double(CLOCKS_PER_SEC);
      pstructProfiler->nFunctionCalls++;
   }
}
void CProfiler::Pause(void)
{
   if (m_st_bDoNothing)
   {
      return;
   }
   if (m_nIamRunning)
   {
      LARGE_INTEGER stop = {0};
      // stop timer
      QueryPerformanceCounter(&stop);
      // compute and print the elapsed time in microsec
      pstructProfiler->dTotalTime += (stop.QuadPart - m_clockStart.QuadPart) / (double)m_frequency.QuadPart;

      //pstructProfiler->dTotalTime += (clock() - m_clockStart) / double(CLOCKS_PER_SEC);
   }
}

// Ascending date sorting function
struct SAscendingstruct2ProfilerSort
{
     bool operator()(const struct2Profiler& rpStart, const struct2Profiler& rpEnd)
     {
          return rpStart.refProfiler.dTotalTime < rpEnd.refProfiler.dTotalTime;
     }
};

void CProfiler::PrintProfileInfo()
{
   if (m_st_bDoNothing)
   {
      return;
   }
   //std::sort(m_st_TimesMap.begin(), m_st_TimesMap.end(), SAscendingstructProfilerSort());
   std::map<const char*, structProfiler, ltsztr>::iterator theIterator = m_st_TimesMap.begin();
   CString str;
   std::vector<struct2Profiler> vTimes;
   while(theIterator != m_st_TimesMap.end())
   {
      CString s;
      //s.Format("%s total = %f, calls %d\n", (*theIterator).first, (*theIterator).second.dTotalTime, (*theIterator).second.nFunctionCalls);
      vTimes.push_back(struct2Profiler((*theIterator).first, (*theIterator).second));
      TRACE(s);
      str += s;
      theIterator++;
   }
   std::sort(vTimes.begin(), vTimes.end(), SAscendingstruct2ProfilerSort());
   //MessageBox(NULL, str, "info", MB_OK);
#ifdef USE_PROFILER
   FILE* pFile = fopen(PROFILER_FILE, "w+");
   for(unsigned int i = 0; i < vTimes.size(); i++)
   {
      fprintf(pFile, "%s total = %f, calls %d\n", vTimes[i].szFunctName, vTimes[i].refProfiler.dTotalTime, vTimes[i].refProfiler.nFunctionCalls);
   }
   fclose(pFile);
#endif
#ifdef PROFILER_OPEN_NOTEPAD
   char szCommand[MAX_PATH] = {0};
   strcpy(szCommand, "notepad.exe ");
   strcat(szCommand, PROFILER_FILE);
   system(szCommand);
#endif
}

void CProfiler::PrintProfileInfo(char* szFileName, int nDisplay)
{
   if (m_st_bDoNothing)
   {
      return;
   }
   std::map<const char*, structProfiler, ltsztr>::iterator theIterator = m_st_TimesMap.begin();
   CString str;
   std::vector<struct2Profiler> vTimes;
   while(theIterator != m_st_TimesMap.end())
   {
      CString s;
      vTimes.push_back(struct2Profiler((*theIterator).first, (*theIterator).second));
      TRACE(s);
      str += s;
      theIterator++;
   }
   std::sort(vTimes.begin(), vTimes.end(), SAscendingstruct2ProfilerSort());

   FILE* pFile = fopen(szFileName, "w+");
   for(unsigned int i = 0; i < vTimes.size(); i++)
   {
      fprintf(pFile, "%s total = %f, calls %d\n", vTimes[i].szFunctName, vTimes[i].refProfiler.dTotalTime, vTimes[i].refProfiler.nFunctionCalls);
   }
   fclose(pFile);

   if (nDisplay)
   {
      char szCommand[MAX_PATH] = {0};
      strcpy(szCommand, "notepad.exe ");
      strcat(szCommand, PROFILER_FILE);
      system(szCommand);
   }
}

void CProfiler::RestartAll(void)
{
   if (m_st_bDoNothing)
   {
      return;
   }
   std::map<const char*, structProfiler, ltsztr>::iterator theIterator = m_st_TimesMap.begin();
   while(theIterator != m_st_TimesMap.end())
   {
      CString s;
      (*theIterator).second.pRunning->m_nIamRunning = 0;
      (*theIterator).second.pRunning->m_bIgnoreDestructor = false;
      theIterator++;
   }
   m_st_TimesMap.clear();
}
