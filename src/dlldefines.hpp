#pragma once

#if defined (_WIN32)
#if defined(SaneEngine_EXPORTS)
#define  SANE_EXPORT __declspec(dllexport)
#else
#define  SANE_EXPORT __declspec(dllimport)
#endif
#else
#define  SANE_EXPORT
#endif
