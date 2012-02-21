#ifndef ROCKETCIRCULARBARHEADER_H
#define ROCKETCIRCULARBARHEADER_H

#include <Rocket/Core/Platform.h>

#if !defined STATIC_LIB
	#ifdef ROCKET_PLATFORM_WIN32
		#ifdef RocketCircularBar_EXPORTS
			#define ROCKETCIRCULARBAR_API __declspec(dllexport)
		#else
			#define ROCKETCIRCULARBAR_API __declspec(dllimport)
		#endif
	#else
		#define ROCKETCIRCULARBAR_API __attribute__((visibility("default")))
	#endif
#else
	#define ROCKETCIRCULARBAR_API
#endif

#endif
