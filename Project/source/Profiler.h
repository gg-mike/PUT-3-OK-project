#pragma once
#include "pch.h"

#define PROFILING true
#if PROFILING
#define PROFILE_SCOPE(name) ProfilerTimer timer##__LINE__(name)
#define PROFILE_FUNCITION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCITION()
#endif

struct ProfileResult
{
	std::string Name;
	long long Start, End;
	uint32_t ThreadID;
};
	
class Profiler
{
public:
	~Profiler()
	{
		EndSession();
	}
	void BeginSession(const std::string& name, const std::string& filepath = "result.json")
	{
		m_OutputStream.open(filepath);
		WriteHeader();
		m_CurrentSession = name;
	}
	void WriteProfile(const ProfileResult& result)
	{
		if (m_ProfileCount++ > 0)
			m_OutputStream << ",";
	
		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');
		int pos = name.find("__cdecl ");
		if (pos != std::string::npos)
			name.erase(pos, 8);
	
		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";
		m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":" << result.ThreadID << ",";
		m_OutputStream << "\"ts\":" << result.Start;
		m_OutputStream << "}";
	}
	static Profiler& Get()
	{
		static Profiler instance;
		return instance;
	}
	
private:
	Profiler()
		: m_CurrentSession(""), m_ProfileCount(0)
	{
	}
	inline void WriteHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
	}
	inline void WriteFooter()
	{
		m_OutputStream << "]}";
	}
	void EndSession()
	{
		WriteFooter();
		m_OutputStream.close();
		m_CurrentSession = "";
		m_ProfileCount = 0;
	}
	
private:
	std::string m_CurrentSession;
	std::ofstream m_OutputStream;
	int m_ProfileCount;

};
	
class ProfilerTimer
{
public:
	ProfilerTimer(const char* name = "ProfileTimer")
		: result{ name }, m_Stopped(false)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}
	~ProfilerTimer()
	{
		if (!m_Stopped)
			Stop();
	}
	
private:
	void Stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();
	
		result.Start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
		result.End = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
	
		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Profiler::Get().WriteProfile(result);
	
		m_Stopped = true;
	}

private:
	ProfileResult result;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
	bool m_Stopped;
	
};