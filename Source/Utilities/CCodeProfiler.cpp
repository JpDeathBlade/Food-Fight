/**
 * 0===========================================================================0
 * | CCodeProfiler                                                             |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   Author        : Joseph Stuhr                                            |
 * |   Version       : 03.20 J.Stuhr                                           |
 * |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
 * |   File Name     : CCodeProfiler.cpp                                       |
 * |   Bundle        : FF_JS_CCODEPROFILER_H_                                  |
 * |   Line Count    : 000,465                                                 |
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

#include "CCodeProfiler.h"

// 0===========================================================================0
// | ~ Includes ~                                                              |
// 0===========================================================================0
#include <fstream>		// Allows us to open a file and write out data
using std::ofstream;	//   to be veiwed later.

// Initialize the static object
CCodeProfiler CCodeProfiler::m_Instance;

// 0===========================================================================0
// | Function  :	CCodeProfiler()                                   09/15/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	void                                                       |
// |                                                                           |
// |   Return  :	N/A                                                        |
// |                                                                           |
// |   Purpose :	Constructor.                                               |
// 0===========================================================================0
CCodeProfiler::CCodeProfiler(void)
{
	#if _DEBUG
		// Initialize the Frequeny
		//  - This can fail on older machines!
		if(!QueryPerformanceFrequency(&m_liFrequeny))
			exit(-10); // m_liFrequeny is now 0, fix that (quit for now)

	#endif
}

// 0===========================================================================0
// | Function  :	~CCodeProfiler()                                  09/15/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	void                                                       |
// |                                                                           |
// |   Return  :	N/A                                                        |
// |                                                                           |
// |   Purpose :	Destructor.                                                |
// 0===========================================================================0
CCodeProfiler::~CCodeProfiler(void)
{
	#if _DEBUG
	#endif
}

// 0===========================================================================0
// | Function  :	GetInstance()                                     09/15/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	void                                                       |
// |                                                                           |
// |   Return  :	Instance to this class.                                    |
// |                                                                           |
// |   Purpose :	Make the class a singleton and allow access to it.         |
// 0===========================================================================0
CCodeProfiler* CCodeProfiler::GetInstance(void)
{
	#if _DEBUG
		return &m_Instance;
	#else
		return NULL;
	#endif
}

// 0===========================================================================0
// | Function  :	InitCodeProfiler()                                09/15/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	_D3DXVECTOR2: D3DXVECTOR2 of the Profile vector (Default: 128).          |
// |                                                                           |
// |   Return  :	True if the function was successful.                       |
// |                                                                           |
// |   Purpose :	To initialize the Code Profiler.                           |
// 0===========================================================================0
bool CCodeProfiler::InitCodeProfiler(int _D3DXVECTOR2)
{
	#if _DEBUG
		// Reserve slots for the Profiles and Errors, Default: 128
		m_CodeProfiles.reserve(_D3DXVECTOR2);
		m_Errors.reserve(_D3DXVECTOR2);

		// Start logging the Profilers run time
		QueryPerformanceCounter(&m_liProfilerStart);
		return true;
	#else
		return false;
	#endif
}

// 0===========================================================================0
// | Function  :	ShutdownCodeProfiler()                            09/15/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	void                                                       |
// |                                                                           |
// |   Return  :	void                                                       |
// |                                                                           |
// |   Purpose :	To shutdown the Code Profiler.                             |
// 0===========================================================================0
void CCodeProfiler::ShutdownCodeProfiler(void)
{
	#if _DEBUG
		// The Profiler is done logging, log the time stopped
		QueryPerformanceCounter(&m_liEnd);

		// Calculate the elapsed time of the Code Profiler
		//  (End - Start * 1000) / CPU Frequeny = Milliseconds Ran
		float timeRan = 
			(float)(((m_liEnd.QuadPart - m_liProfilerStart.QuadPart) * 1000.0f)
			  / m_liFrequeny.QuadPart);

		float	totalTime;			// Store the total time that each Profile ran
		bool	stillRunning;		// Store if the Profile is still running

		// Open the txt file to save the data
		ofstream FileOut("Resources/Data/CodeProfile.txt");

		// If the file is opened, write the data
		if(FileOut)
		{
			// Print the Profilers total Runtime
			FileOut << "Code Profiler Runtime: " << timeRan << " millisecond.\n"
					<< "====================================================\n\n";

			// Iterate through the Profiles
			for(vector<tCodeProfile>::iterator iter = m_CodeProfiles.begin();
				iter != m_CodeProfiles.end(); iter++)
			{
				// Reset for the new Profile
				stillRunning = false;
				totalTime = 0.0f;

				// If that function is still running, stop it!
				if((*iter).m_bRunning)
				{
					Stop((*iter).m_szFunctionName);	// Call stop on the Profile
					(*iter).m_nLineStopped = -1;	// Set its line stopped to -1
					stillRunning = true;			// It was still running

					LogError("Error on Shutdown: %s NEVER STOPPED!! - Called at %s Line %d",
						(*iter).m_szFunctionName,(*iter).m_szFileName,(*iter).m_nLineStarted);
				}

				// Print the details of the Profile
				FileOut << (*iter).m_szFunctionName << " in " << (*iter).m_szFileName << "\n"
						<< " - Line Started: " << (*iter).m_nLineStarted << "\n"
						<< " - Line Stopped: " << (*iter).m_nLineStopped << "\n";

				// If it was still running print that we stopped it
				if(stillRunning)
					FileOut << "    !! STOPPED ON SHUTDOWN !!\n";

				// Iterate through the times of the Profile
				for(vector<float>::iterator iterF = (*iter).m_fTimeElapsed.begin();
				iterF != (*iter).m_fTimeElapsed.end(); iterF++)
				{
					FileOut << " - Time        : " << (*iterF) << " ms\n"; // Print the time
					totalTime += (*iterF); // Update the total time
				}

				// Print the total as well as the average
				FileOut << " Total: " << totalTime << " / " << (*iter).m_fTimeElapsed.size() << " run(s)\n"
					    << "  - " << (totalTime / (*iter).m_fTimeElapsed.size()) << " average ms\n"
						<< "===================================================\n\n";
			}

			// Print the Errors the user had
			FileOut << "Errors:\n"
					<< "===================================================\n\n";

			// If there are no errors, print that
			if(m_Errors.size() == 0)
				FileOut << " - There were no errors!";
			else
			{
				// Iterate through the error list
				for(vector<tError>::iterator iter = m_Errors.begin();
					iter != m_Errors.end(); iter++)
				{
					// Print the details
					FileOut << (*iter).m_szError << "\n\n";
				}
			}

			// Close the file when we are done
			FileOut.close();
		}
	#endif
}

// 0===========================================================================0
// | Function  :	Start()                                           09/15/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	char*: Name of the Profile                                 |
// |                                                                           |
// |   Return  :	void                                                       |
// |                                                                           |
// |   Purpose :	Tell the Profiler to begin logging this Profile.           |
// 0===========================================================================0
void CCodeProfiler::Start_t(char* _function, int _line, char* _file)
{
	#if _DEBUG
		if(!AlreadyStarted(_function)) //If the function doesn't exist we want to make it
		{							   //  - AlreadyStarted (if true) saves the Profile to m_pRunningProfile
			tCodeProfile newProfile;   // Create a Profile for the new function

			// Initialize the profile
			newProfile.m_szFunctionName = _function;  // Save the function name
			newProfile.m_nLineStarted = _line;		  // Save the Starting Line
			newProfile.m_szFileName = _file;		  // Save the File name
			newProfile.m_bRunning = true;			  // This function is now running

			// Start the timer
			QueryPerformanceCounter(&newProfile.m_liStart);

			// Add the profile to the vector
			m_CodeProfiles.push_back(newProfile);
		}
		else // If the Profile exists, check if its running
		{

			if(!(*m_pRunningProfile).m_bRunning) // If its not running
			{
				// Start it up again
				(*m_pRunningProfile).m_bRunning = true;
				QueryPerformanceCounter(&(*m_pRunningProfile).m_liStart);
			}
			else
			{
				LogError("Error: %s already running - Called at %s Line %d",
						_function, _file, _line);
			}
		}
	#endif
}

// 0===========================================================================0
// | Function  :	Stop()                                            09/15/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	char*: Name of the Profile                                 |
// |                                                                           |
// |   Return  :	void                                                       |
// |                                                                           |
// |   Purpose :	Tell the Profiler to stop logging this Profile.            |
// 0===========================================================================0
void CCodeProfiler::Stop_t(char* _function, int _line, char* _file)
{
	#if _DEBUG
		if(AlreadyStarted(_function)) //If it exists
		{							  //  - AlreadyStarted (if true) saves the Profile to m_pRunningProfile
			if((*m_pRunningProfile).m_bRunning) // If its running
			{
				QueryPerformanceCounter(&m_liEnd);           // Log the stop time of the Profile
				(*m_pRunningProfile).m_bRunning = false;	 // Stop it so we can use this Profile again
				(*m_pRunningProfile).m_nLineStopped = _line; // Log the line it was stopped

				// Calculate the elapsed time
				float time = 
					(float)(((m_liEnd.QuadPart - (*m_pRunningProfile).m_liStart.QuadPart) * 1000.0f)
						/ m_liFrequeny.QuadPart);

				// Save the elapsed time
				(*m_pRunningProfile).m_fTimeElapsed.push_back(time);
			}
			else
			{
				LogError("Error: %s is NOT running - Called at %s Line %d",
						_function, _file, _line);
			}
		}
		else // It does not exists, error
		{
			LogError("Error: %s does NOT exist - Called at %s Line %d",
						_function, _file, _line);
		}
	#endif
}

// 0===========================================================================0
// | Function  :	AlreadyStarted()                                  09/15/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	char*: Name of the Profile                                 |
// |                                                                           |
// |   Return  :	True if the function exists.                               |
// |                                                                           |
// |   Purpose :	Locate the function to see if it exists.                   |
// 0===========================================================================0
bool CCodeProfiler::AlreadyStarted(char* _function)
{	
	// Iterate through the Profiles
	for(vector<tCodeProfile>::iterator iter = m_CodeProfiles.begin(); iter != m_CodeProfiles.end(); iter++)
	{
		// If the function already exists...
		if( (*iter).m_szFunctionName == _function)
		{
			// Save it so we can edit it...
			m_pRunningProfile = &(*iter);
			return true; // Break and return true
		}
	}

	return false;
}

// 0===========================================================================0
// | Function  :	LogError()                                        09/15/09 |
// |  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  |
// |   Input   :	char*: Name of the Error                                   |
// |                char*: Name of the Profile                                 |
// |                char*: The file the error happened in                      |
// |                int  : The line the error happened on                      |
// |                                                                           |
// |   Return  :	Adds the error to the error log.                           |
// |                                                                           |
// |   Purpose :	Logging Errors.                                            |
// 0===========================================================================0
void CCodeProfiler::LogError(char* _error, char* _function, char* _file, int _line)
{
	tError newError;  // Create an error
	char buffer[1024]; // Create a buffer for the error

	// Create the Error message
	sprintf_s(buffer,_countof(buffer),_error,
		_function,_file,_line);

	// Save the error
	strcpy_s(newError.m_szError,sizeof(buffer),buffer);

	// Push the error back
	m_Errors.push_back(newError);
}