// 1. 전역제어
// L_O_G_NDEBUG                 DEBUG（기본값은 NDEBUG에 의해 자동으로 판단）
// L_O_G_SHOW_DEBUG             DEBUG 로그 강제 오픈
// L_O_G_DISABLE_ALL            모든 로그 이용정지
// L_O_G_DISABLE_COLOR          색상 표기 사용정지
// L_O_G_LINE_END_CRLF          기본값은 \n 끝에 이 매크로를 추가하면 \r\n
// L_O_G_FOR_MCU                MCU환경전용
// L_O_G_NOT_EXIT_ON_FATAL      FATAL 기본 종료 프로그램 이 매크로를 추가해도 종료되지 않음
// L_O_G_SHOW_FULL_PATH         파일 절대 경로 보이기
//
// c++11 환경기본값
// L_O_G_ENABLE_THREAD_SAFE		
// L_O_G_ENABLE_THREAD_ID       
// L_O_G_ENABLE_DATE_TIME       
// L_O_G_DISABLE_THREAD_SAFE
// L_O_G_DISABLE_THREAD_ID
// L_O_G_DISABLE_DATE_TIME
// `L_O_GET_TID_CUSTOM` 을 통해 스레드 ID를 사용자 정의
//
// 2. 사용자 정의
// L_O_G_PRINTF_CUSTOM          사용자 정의 출력
// 구현추가 `int L_O_G_PRINTF_CUSTOM(const char *fmt, ...)`
//
// 3. 라이브러리 사용시
// 3.1. `LOG`를 라이브러리 이름으로 대체
// 3.2. 정의 `LOG_IN_LIB`
// 3.3. 설정가능항목
// LOG_SHOW_DEBUG               LOGD 출력 켜기
// LOG_SHOW_VERBOSE             LOGV 출력 보이기
// LOG_DISABLE_ALL              모든 로그 닫기

#pragma once

// clang-format off

//#define LOG_IN_LIB

#if defined(LOG_DISABLE_ALL) || defined(L_O_G_DISABLE_ALL)

#define LOG(fmt, ...)           ((void)0)
#define LOGT(tag, fmt, ...)     ((void)0)
#define LOGI(fmt, ...)          ((void)0)
#define LOGW(fmt, ...)          ((void)0)
#define LOGE(fmt, ...)          ((void)0)
#define LOGF(fmt, ...)          ((void)0)
#define LOGD(fmt, ...)          ((void)0)
#define LOGV(fmt, ...)          ((void)0)

#else

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#if __cplusplus >= 201103L

#if !defined(L_O_G_DISABLE_THREAD_SAFE) && !defined(L_O_G_ENABLE_THREAD_SAFE)
#define L_O_G_ENABLE_THREAD_SAFE
#endif

#if !defined(L_O_G_DISABLE_THREAD_ID) && !defined(L_O_G_ENABLE_THREAD_ID)
#define L_O_G_ENABLE_THREAD_ID
#endif

#if !defined(L_O_G_DISABLE_DATE_TIME) && !defined(L_O_G_ENABLE_DATE_TIME)
#define L_O_G_ENABLE_DATE_TIME
#endif

#endif
#else
#include <string.h>
#include <stdlib.h>
#endif

#ifdef  L_O_G_LINE_END_CRLF
#define LOG_LINE_END            "\r\n"
#else
#define LOG_LINE_END            "\n"
#endif

#ifdef L_O_G_NOT_EXIT_ON_FATAL
#define LOG_EXIT_PROGRAM()
#else
#ifdef L_O_G_FOR_MCU
#define LOG_EXIT_PROGRAM()      do{ for(;;); } while(0)
#else
#define LOG_EXIT_PROGRAM()      exit(EXIT_FAILURE)
#endif
#endif

#ifdef L_O_G_SHOW_FULL_PATH
#define LOG_BASE_FILENAME       (__FILE__)
#else
#ifdef __FILE_NAME__
#define LOG_BASE_FILENAME       (__FILE_NAME__)
#else
#define LOG_BASE_FILENAME       (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif
#endif

#define LOG_WITH_COLOR

#if defined(_WIN32) || (defined(__ANDROID__) && !defined(ANDROID_STANDALONE)) || defined(L_O_G_FOR_MCU)
#undef LOG_WITH_COLOR
#endif

#ifdef L_O_G_DISABLE_COLOR
#undef LOG_WITH_COLOR
#endif

#ifdef LOG_WITH_COLOR
#define LOG_COLOR_RED           "\033[31m"
#define LOG_COLOR_GREEN         "\033[32m"
#define LOG_COLOR_YELLOW        "\033[33m"
#define LOG_COLOR_BLUE          "\033[34m"
#define LOG_COLOR_CARMINE       "\033[35m"
#define LOG_COLOR_CYAN          "\033[36m"
#define LOG_COLOR_DEFAULT
#define LOG_COLOR_END           "\033[m"
#else
#define LOG_COLOR_RED
#define LOG_COLOR_GREEN
#define LOG_COLOR_YELLOW
#define LOG_COLOR_BLUE
#define LOG_COLOR_CARMINE
#define LOG_COLOR_CYAN
#define LOG_COLOR_DEFAULT
#define LOG_COLOR_END
#endif

#define LOG_END                 LOG_COLOR_END LOG_LINE_END

#ifndef L_O_G_PRINTF
#ifndef LOG_PRINTF_DEFAULT
#if defined(__ANDROID__) && !defined(ANDROID_STANDALONE)
#include <android/log.h>
#define LOG_PRINTF_DEFAULT(...) __android_log_print(ANDROID_L##OG_DEBUG, "LOG", __VA_ARGS__)
#else
#define LOG_PRINTF_DEFAULT(...) printf(__VA_ARGS__)
#endif
#endif

#ifndef L_O_G_PRINTF_CUSTOM
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif
#ifdef L_O_G_ENABLE_THREAD_SAFE
#ifndef L_O_G_NS_MUTEX
#define L_O_G_NS_MUTEX L_O_G_NS_MUTEX
#include <mutex>
// 1. struct instead of namespace, ensure single instance
struct L_O_G_NS_MUTEX {
static std::mutex& mutex() {
  // 2. never delete, avoid destroy before user log
  // 3. static memory, avoid memory fragmentation
  static char memory[sizeof(std::mutex)];
  static std::mutex& mutex = *(new (memory) std::mutex());
  return mutex;
}
};
#endif
#define L_O_G_PRINTF(...) { \
  std::lock_guard<std::mutex> lock(L_O_G_NS_MUTEX::mutex()); \
  LOG_PRINTF_DEFAULT(__VA_ARGS__); \
}
#else
#define L_O_G_PRINTF(...)       LOG_PRINTF_DEFAULT(__VA_ARGS__)
#endif
#else
extern int L_O_G_PRINTF_CUSTOM(const char *fmt, ...);
#define L_O_G_PRINTF(...)       L_O_G_PRINTF_CUSTOM(__VA_ARGS__)
#endif
#endif

#ifdef L_O_G_ENABLE_THREAD_ID
#ifndef L_O_G_NS_GET_TID
#define L_O_G_NS_GET_TID L_O_G_NS_GET_TID
#include <cstdint>
#ifdef L_O_G_GET_TID_CUSTOM
extern uint32_t L_O_G_GET_TID_CUSTOM();
#elif defined(_WIN32)
#include <processthreadsapi.h>
struct L_O_G_NS_GET_TID {
static inline uint32_t get_tid() {
  return GetCurrentThreadId();
}
};
#elif defined(__linux__)
#include <sys/syscall.h>
#include <unistd.h>
struct L_O_G_NS_GET_TID {
static inline uint32_t get_tid() {
  return syscall(SYS_gettid);
}
};
#else /* for mac, bsd.. */
#include <pthread.h>
struct L_O_G_NS_GET_TID {
static inline uint32_t get_tid() {
  uint64_t x;
  pthread_threadid_np(nullptr, &x);
  return (uint32_t)x;
}
};
#endif
#endif
#ifdef L_O_G_GET_TID_CUSTOM
#define LOG_THREAD_LABEL "%u "
#define LOG_THREAD_VALUE ,L_O_G_GET_TID_CUSTOM()
#else
#define LOG_THREAD_LABEL "%u "
#define LOG_THREAD_VALUE ,L_O_G_NS_GET_TID::get_tid()
#endif
#else
#define LOG_THREAD_LABEL
#define LOG_THREAD_VALUE
#endif

#ifdef L_O_G_ENABLE_DATE_TIME
#include <chrono>
#include <sstream>
#include <iomanip> // std::put_time
#ifndef L_O_G_NS_GET_TIME
#define L_O_G_NS_GET_TIME L_O_G_NS_GET_TIME
struct L_O_G_NS_GET_TIME {
static inline std::string get_time() {
  auto now = std::chrono::system_clock::now();
  std::time_t time = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << ms.count();
  return ss.str();
}
};
#endif
#define LOG_TIME_LABEL "%s "
#define LOG_TIME_VALUE ,L_O_G_NS_GET_TIME::get_time().c_str()
#else
#define LOG_TIME_LABEL
#define LOG_TIME_VALUE
#endif

#define LOG(fmt, ...)           do{ L_O_G_PRINTF(LOG_COLOR_GREEN   LOG_TIME_LABEL LOG_THREAD_LABEL "[*]: %s:%d "       fmt LOG_END LOG_TIME_VALUE LOG_THREAD_VALUE, LOG_BASE_FILENAME, __LINE__, ##__VA_ARGS__); } while(0)
#define LOGT(tag, fmt, ...)     do{ L_O_G_PRINTF(LOG_COLOR_BLUE    LOG_TIME_LABEL LOG_THREAD_LABEL "[" tag "]: %s:%d " fmt LOG_END LOG_TIME_VALUE LOG_THREAD_VALUE, LOG_BASE_FILENAME, __LINE__, ##__VA_ARGS__); } while(0)
#define LOGI(fmt, ...)          do{ L_O_G_PRINTF(LOG_COLOR_YELLOW  LOG_TIME_LABEL LOG_THREAD_LABEL "[I]: %s:%d "       fmt LOG_END LOG_TIME_VALUE LOG_THREAD_VALUE, LOG_BASE_FILENAME, __LINE__, ##__VA_ARGS__); } while(0)
#define LOGW(fmt, ...)          do{ L_O_G_PRINTF(LOG_COLOR_CARMINE LOG_TIME_LABEL LOG_THREAD_LABEL "[W]: %s:%d [%s] "  fmt LOG_END LOG_TIME_VALUE LOG_THREAD_VALUE, LOG_BASE_FILENAME, __LINE__, __func__, ##__VA_ARGS__); } while(0)                     // NOLINT(bugprone-lambda-function-name)
#define LOGE(fmt, ...)          do{ L_O_G_PRINTF(LOG_COLOR_RED     LOG_TIME_LABEL LOG_THREAD_LABEL "[E]: %s:%d [%s] "  fmt LOG_END LOG_TIME_VALUE LOG_THREAD_VALUE, LOG_BASE_FILENAME, __LINE__, __func__, ##__VA_ARGS__); } while(0)                     // NOLINT(bugprone-lambda-function-name)
#define LOGF(fmt, ...)          do{ L_O_G_PRINTF(LOG_COLOR_CYAN    LOG_TIME_LABEL LOG_THREAD_LABEL "[!]: %s:%d [%s] "  fmt LOG_END LOG_TIME_VALUE LOG_THREAD_VALUE, LOG_BASE_FILENAME, __LINE__, __func__, ##__VA_ARGS__); LOG_EXIT_PROGRAM(); } while(0) // NOLINT(bugprone-lambda-function-name)

#if defined(LOG_IN_LIB) && !defined(LOG_SHOW_DEBUG) && !defined(L_O_G_NDEBUG)
#define LOG_NDEBUG
#endif

#if defined(L_O_G_NDEBUG) && !defined(LOG_NDEBUG)
#define LOG_NDEBUG
#endif

#if (defined(NDEBUG) || defined(LOG_NDEBUG)) && !defined(L_O_G_SHOW_DEBUG)
#define LOGD(fmt, ...)          ((void)0)
#else
#define LOGD(fmt, ...)          do{ L_O_G_PRINTF(LOG_COLOR_DEFAULT LOG_TIME_LABEL LOG_THREAD_LABEL "[D]: %s:%d "       fmt LOG_END LOG_TIME_VALUE LOG_THREAD_VALUE, LOG_BASE_FILENAME, __LINE__, ##__VA_ARGS__); } while(0)
#endif

#if defined(LOG_SHOW_VERBOSE)
#define LOGV(fmt, ...)          do{ L_O_G_PRINTF(LOG_COLOR_DEFAULT LOG_TIME_LABEL LOG_THREAD_LABEL "[V]: %s:%d "       fmt LOG_END LOG_TIME_VALUE LOG_THREAD_VALUE, LOG_BASE_FILENAME, __LINE__, ##__VA_ARGS__); } while(0)
#else
#define LOGV(fmt, ...)          ((void)0)
#endif

#endif
