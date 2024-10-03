#pragma once

#include<iostream>
#include <iomanip>
#include <sstream>
#include <string>

template<typename T>
bool struct_to_char(T* st_input_, char * pch_output_)
{
	bool bRet = true;
	if (st_input_ != nullptr || pch_output_ != nullptr) {
		memset(pch_output_, 0x00, sizeof(T));
		memcpy_s(pch_output_, sizeof(T), st_input_, sizeof(T));
	}
	else {
		bRet = false;
	}
	return bRet;
}

template<typename T>
bool char_to_struct(char * pch_input_, T* st_output_)
{
	bool bRet = true;
	if (st_output_ != nullptr || pch_input_ != nullptr) {
		memset(st_output_, 0x00, sizeof(T));
		memcpy_s(st_output_, sizeof(T), pch_input_, sizeof(T));
	}
	else {
		bRet = false;
	}

	return bRet;
}

bool char_to_string(char * pch_input_, uint32_t size_, std::string& str_output_)
{
	bool bRet = true;
	if (pch_input_ != nullptr) {
		str_output_.assign(pch_input_, size_);
	}
	else {
		bRet = false;
	}

	return bRet;
}

bool string_to_char(std::string& str_input_, uint32_t size_, char * pch_output_)
{
	bool bRet = true;
	if (pch_output_ != nullptr) {
		memcpy_s(pch_output_, size_, str_input_.data(), size_);
	}
	else {
		bRet = false;
	}

	return bRet;
}

bool string_to_char(std::string&& str_input_, uint32_t size_, char* pch_output_)
{
	bool bRet = true;
	if (pch_output_ != nullptr) {
		memmove_s(pch_output_, size_, str_input_.data(), size_);
	}
	else {
		bRet = false;
	}

	return bRet;
}

template<typename T>
bool conv_to_struct(std::string& str_input_, uint32_t size_, T* st_output_)
{
	bool bRet = true;
	if (st_output_ != nullptr) {
		memcpy_s(st_output_, size_, str_input_.data(), size_);
	}
	else {
		bRet = false;
	}
	
	return bRet;
}

template<typename T>
bool conv_to_struct(std::string&& str_input_, uint32_t size_, T* st_output_)
{
	bool bRet = true;
	if (st_output_ != nullptr) {
		memmove_s(st_output_, size_, str_input_.data(), size_);
	}
	else {
		bRet = false;
	}

	return bRet;
}

template<typename T>
bool conv_to_string(T* st_input_, uint32_t size_, std::string& str_output_)
{
	bool bRet = true;

	std::ostringstream oss;

	std::ostream::fmtflags old_flags = oss.flags();
	char old_fill = oss.fill();

	oss << std::hex << std::setfill('0');
	
	unsigned char* uch_ptr = (unsigned char*)st_input_;
	for (uint32_t idx = 0; size_ > idx; ++idx) {
		oss << static_cast<unsigned char>(uch_ptr[idx]);
	}

	oss.flags(old_flags);
	oss.fill(old_fill);

	str_output_ = oss.str();
	
	return bRet;
}
