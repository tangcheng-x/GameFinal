#ifndef __GF_COMPILE_CONFIG_H_INCLUDED__
#define __GF_COMPILE_CONFIG_H_INCLUDED__

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#define _GF_WINDOWS_
#define _GF_WINDOWS_API_
#define _GF_COMPILE_WITH_WINDOWS_DEVICE_
#endif

#endif