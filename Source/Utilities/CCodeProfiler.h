/**
 * 0===========================================================================0
 * | CCodeProfiler                                                             |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Author        : Joseph Stuhr                                            |
 * |   Version       : 03.20 J.Stuhr                                           |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   File Name     : CCodeProfiler.h                                         |
 * |   Bundle        : FF_JS_CCODEPROFILER_H_                                  |
 * |   Line Count    : 000,319                                                 |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Date Created  : ??:?? pm (Eastern) on 09/13/2009                        |
 * |   Date Edited   : 03:11 pm (Eastern) on 09/16/2009                        |
 * 0=====0===============================================================0=====0
 *       |                                                               |      
 * 0=====0===============================================================0=====0
 * | Code Description:                                                         |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   A class used to profile code to view the time it takes a fucntion or    |
 * |    chunk of code to run. Using this a programmer can find spikes in       |
 * |    his/her code, allowing for optimization.                               |
 * 0===========================================================================0
 * | Version History Key:                                                      |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   [H] - Upgraded Comment Header.                                          |
 * |                                                                           |
 * |   [$] - Project name change.         [~] - Author change.                 |
 * |   [#] - Bundle name change.          [.] - File name change.              |
 * |   [;] - Updated Description.         [&] - Updated Files Needed.          |
 * |   [=] - Addition to credits.                                              |
 * |                                                                           |
 * |   [+] - Addition to the code.        [-] - Subtraction from code.         |
 * |   [^] - Improvement to the code.     [%] - Optimization to the code.      |
 * |   [!] - Known bug.                   [*] - Bug fix.                       |
 * |   [@] - Planned change/feature.                                           |
 * |                                                                           |
 * |   [c] - Added comments.              [x] - Removed comments.              |
 * |   [v] - Edited comments.                                                  |
 * |                                                                           |
 * |   [ ] - Other                                                             |
 * |                                                                           |
 * |   [a] - Alpha release.               [b] - Beta release.                  |
 * |   [i] - Initial public release.      [f] - Final release from (author).   |
 * |                                                                           |
 * 0===========================================================================0
 * | Version History:                                                          |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Version 03.20 J.Stuhr - 09/15/2009                                      |
 * |    [+] - Implemented Stop on Shutdown.                                    |
 * |    [^] - Improved Error Log.                                              |
 * |    [%] - Decreased memory usage.                                          |
 * |    [c] - Fully commented the code.                                        |
 * |                                                                           |
 * |   Version 03.00 J.Stuhr - 09/14/2009                                      |
 * |    [+] - Profiles store times and calculate the average.                  |
 * |    [^] - Improved Multi-Profiling.                                        |
 * |                                                                           |
 * |   Version 02.00 J.Sturh - 09/14/2009                                      |
 * |    [+] - Allowed for Multi-Profiling.                                     |
 * |                                                                           |
 * |   Version 01.00 J.Stuhr - 09/13/2009                                      |
 * |    [i] - Initial release from Joseph Stuhr (J.Stuhr).                     |
 * 0===========================================================================0
 * | Files Needed:                                                             |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   windows.h : Windows Only Code                                           |
 * |   vector    : Used to store data                                          |
 * |   fstream   : Used to write data to a text file                           |
 * 0===========================================================================0
 * | How to use the File:                                                      |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Call "PROFILER->InitCodeProfiler();" without quotes at the very         |
 * |    begining of your code to start the Profiler. The profiler logs how     |
 * |    long it runs as well so you can use that to time your codes overall    |
 * |    performance.                                                           |
 * |                                                                           |
 * |   Call "PROFILER->Start("PROFILENAME");" without quotes and replace       |
 * |    PROFILENAME with the name you want to register the Profile to.         |
 * |    This will begin logging that Profile. If it has not been logged before |
 * |    it will be added and if it has already been logged it will add a new   |
 * |    timer to the profile (Only if you stopped the profile, you can't start |
 * |    a new one if one is already running).                                  |
 * |                                                                           |
 * |   Call "PROFILER->Stop("PROFILENAME");" without quotes and replace        |
 * |    PROFILENAME with the name of the Profile you want to stop. This will   |
 * |    stop the current timer and store the time elapsed. It will only stop   |
 * |    a Profile that has been logged using Start (You can't stop whats not   |
 * |    running or does not exist).                                            |
 * |                                                                           |
 * |   Call "PROFILER->ShutdownCodeProfiler();" without quotes at the very     |
 * |    end of your program (or as close as you can get). This will shut down  |
 * |    any Profiles you have missed as well as create a text file inside the  |
 * |    "Resource\Data\" directory.                                            |
 * |                                                                           |
 * |   CodeProfile.txt located in the "Resource\Data\" directory, after a      |
 * |    minimum of one run with the "PROFILER->ShutdownCodeProfiler();" call,  |
 * |    will show you the File the Profile was started in as well as the line  |
 * |    it started logging and the line it stopped logging. It will also show  |
 * |    the times for each run and then average the times together automaticly.|
 * |    If there was an error it will print at the bottom of the file with the |
 * |    File and Line for easy tracking. It will also warn of Profiles that    |
 * |    were stopped at shutdown.                                              |
 * 0=====0===============================================================0=====0
 *       |                                                               |      
 * 0=====0===============================================================0=====0
 * | Comment Header SDK : Version 02.00 J.Stuhr - 12/03/2008                   |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   This header is free software: you can redistribute it and/or modify     |
 * |     it under the terms of the GNU General Public License as published by  |
 * |     the Free Software Foundation, either version 3 of the License, or     |
 * |     any later version, as long as this section remains intact.            |
 * |                                                                           |
 * |   This header is distributed in the hope that it will be useful,          |
 * |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
 * |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
 * |     GNU General Public License for more details.                          |
 * |                                                                           |
 * |   You should have received a copy of the GNU General Public License       |
 * |     along with this program.  If not, see <http://www.gnu.org/licenses/>. |
 * |                                                                           |
 * |   Contact: JoeStuhr@Gmail.com       Copyright (c) 2008-2010 Joseph Stuhr. |
 * 0===========================================================================0
 * |      111111111122222222223333333333444444444455555555556666666666777777777|
 * |456789012345678901234567890123456789012345678901234567890123456789012345678|
 * |                                                            ~ 80 Columns ~ |
 * 0===========================================================================0
 * | Credits:                                                                  |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   CCodeProfiler - Version 03.20 J.Stuhr:                                  |
 * |     Joseph Stuhr ~ Initial coder.                                         |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Comment Header - Version 02.00 J.Stuhr:                                 |
 * |     Joseph Stuhr ~ Initial author.                                        |
 * 0===========================================================================0
 */

#ifndef FF_JS_CCODEPROFILER_H_
#define FF_JS_CCODEPROFILER_H_

// 0===========================================================================0
// | ~ Includes ~                                                              |
// 0===========================================================================0
#include <windows.h>  // For QueryPerformanceCounter()
#include <vector>     // Used to store the Profiles till the end of the program.
using std::vector;	  //  as well as the error strings

// Profile MACRO to return the Profiler Instance
#define PROFILER		 CCodeProfiler::GetInstance()

// Start and Stop MACROS for the user(s) of the code
#define Start(_function) Start_t(_function,__LINE__,__FILE__);
#define Stop(_function)  Stop_t(_function,__LINE__,__FILE__);

class CCodeProfiler
{
	// 0=======================================================================0
	// | ~ VARIABLES ~                                                         |
	// 0=======================================================================0
	private:
		// A basic layout of a Code Profile
		struct tCodeProfile
		{
			int			   m_nLineStarted;   // Line the profiler started at.
			int			   m_nLineStopped;   // Line the profiler stopped at.
			char*		   m_szFunctionName; // Name of the Function being 
											 //   profiled.
			char*		   m_szFileName;     // The file the function is in.
			LARGE_INTEGER  m_liStart;		 // Log the run time of the 
										     //   Code Profile.
			vector<float>  m_fTimeElapsed;   // The total time elapsed.
			bool		   m_bRunning;	     // Is there a Profile being logged?

		}; // end of struct tCodeProfile

		// A basic layout of a Code Profile
		struct tError
		{
			char		   m_szError[1024];  // Error String

		}; // end of struct tError

		// Store the frequeny to get accurate time for that machine.
		LARGE_INTEGER		 m_liFrequeny;

		// Log the run time of the Code Profiler
		LARGE_INTEGER		 m_liProfilerStart;

		// Log the stop time of a profile to save space
		LARGE_INTEGER		 m_liEnd;

		// Holds all the Profiles to save at the end of the program.
		vector<tCodeProfile> m_CodeProfiles;

		// Holds all the Erros to save at the end of the program.
		vector<tError>		 m_Errors;

		// Holds the profile that is Already Started
		tCodeProfile*		 m_pRunningProfile;

		// Instance of the CodeProfiler
		static CCodeProfiler m_Instance;

	private:
		// 0===================================================================0
		// | ~ FUNCTIONS ~                                                     |
		// 0=====0=======================================================0=====0
		//       |                                                       |      
		// 0=====0=======================================================0=====0
		// | Function  :	CCodeProfiler()                           09/15/09 |
		// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
		// |   Input   :	void                                               |
		// |                                                                   |
		// |   Return  :	N/A                                                |
		// |                                                                   |
		// |   Purpose :	Constructor.                                       |
		// 0===================================================================0
		CCodeProfiler(void);
		CCodeProfiler(CCodeProfiler &ref);
		CCodeProfiler &operator= (CCodeProfiler &ref);

		// 0===================================================================0
		// | Function  :	AlreadyStarted()                          09/15/09 |
		// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
		// |   Input   :	char*: Name of the Profile                         |
		// |                                                                   |
		// |   Return  :	True if the function exists.                       |
		// |                                                                   |
		// |   Purpose :	Locate the function to see if it exists.           |
		// 0===================================================================0
		bool AlreadyStarted(char* _function);

		// 0===================================================================0
		// | Function  :	LogError()                                09/15/09 |
		// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
		// |   Input   :	char*: Name of the Error                           |
		// |                char*: Name of the Profile                         |
		// |                char*: The file the error happened in              |
		// |                int  : The line the error happened on              |
		// |                                                                   |
		// |   Return  :	Adds the error to the error log.                   |
		// |                                                                   |
		// |   Purpose :	Logging Errors.                                    |
		// 0===================================================================0
		void LogError(char* _error, char* _function, char* _file, int _line);

	public:
		// 0===================================================================0
		// | ~ FUNCTIONS ~                                                     |
		// 0=====0=======================================================0=====0
		//       |                                                       |      
		// 0=====0=======================================================0=====0
		// | Function  :	~CCodeProfiler()                          09/15/09 |
		// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
		// |   Input   :	void                                               |
		// |                                                                   |
		// |   Return  :	N/A                                                |
		// |                                                                   |
		// |   Purpose :	Destructor.                                        |
		// 0===================================================================0
		~CCodeProfiler(void);
 
		// 0===================================================================0
		// | Function  :	GetInstance()                             09/15/09 |
		// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
		// |   Input   :	void                                               |
		// |                                                                   |
		// |   Return  :	Instance to this class.                            |
		// |                                                                   |
		// |   Purpose :	Make the class a singleton and allow access to it. |
		// 0===================================================================0
		static CCodeProfiler* GetInstance(void);

		// 0===================================================================0
		// | Function  :	InitCodeProfiler()                        09/15/09 |
		// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
		// |   Input   :	_D3DXVECTOR2: D3DXVECTOR2 of the Profile vector (Default: 128).  |
		// |                                                                   |
		// |   Return  :	True if the function was successful.               |
		// |                                                                   |
		// |   Purpose :	To initialize the Code Profiler.                   |
		// 0===================================================================0
		bool InitCodeProfiler(int _Size = 128);

		// 0===================================================================0
		// | Function  :	ShutdownCodeProfiler()                    09/15/09 |
		// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
		// |   Input   :	void                                               |
		// |                                                                   |
		// |   Return  :	void                                               |
		// |                                                                   |
		// |   Purpose :	To shutdown the Code Profiler.                     |
		// 0===================================================================0
		void ShutdownCodeProfiler(void);

		// 0===================================================================0
		// | Function  :	Start()                                   09/15/09 |
		// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
		// |   Input   :	char*: Name of the Profile                         |
		// |                                                                   |
		// |   Return  :	void                                               |
		// |                                                                   |
		// |   Purpose :	Tell the Profiler to begin logging this Profile.   |
		// 0===================================================================0
		void Start_t(char* _function, int _line, char* _file);

		// 0===================================================================0
		// | Function  :	Stop()                                    09/15/09 |
		// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
		// |   Input   :	char*: Name of the Profile                         |
		// |                                                                   |
		// |   Return  :	void                                               |
		// |                                                                   |
		// |   Purpose :	Tell the Profiler to stop logging this Profile.    |
		// 0===================================================================0
		void Stop_t(char* _function, int _line, char* _file);

}; // end of class CCodeProfiler

#endif // end of FF_JS_CCODEPROFILER_H_