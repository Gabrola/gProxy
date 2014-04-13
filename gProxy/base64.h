#pragma once

extern SAFEARRAY* base64_decode(std::string const& s);
extern SAFEARRAY* base64_decode2(char* encoded_string, int lenth);