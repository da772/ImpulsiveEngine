#pragma once
#ifdef GE_PLATFORM_ANDROID
#ifndef ANDROID_UTIL
#define ANDROID_UTIL


class _jobject;
class _JNIEnv;

namespace AndroidUtil {

	/**
* Our saved state data.
*/
	struct filedat {
		unsigned long size;
		unsigned char* data;
	};



	void setEngine(struct engine* en);

	struct filedat LoadFileToMemory(const char* filePath);

	void* GetNativeActivity();

	void SetRewardUserId_Google(std::string id);


	_JNIEnv* GetJNIEnv();
	_jobject* GetAndroidActivity();



}
#endif
#endif