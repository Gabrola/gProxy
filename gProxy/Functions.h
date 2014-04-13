#pragma once
class Functions
{
public:
	static string ByteArrayToPhp(byte* Buffer, int BufferLen);
	static string ByteArrayToCSharp(byte* Buffer, int BufferLen);
	static string ByteArrayToString(byte* Buffer, int BufferLen);
	static vector<string> &split(const string &s, char delim, vector<string> &elems);
	static vector<string> split(const string &s, char delim);
	static int str2int (const string &str);
	static char str2char (const string &str);
	static char* PhpToByteArray(string Buffer);
	static char *read_pass(char *const prompt);
	static string HttpPost(LPCWSTR uri, char* pars, HINTERNET hHandle, LPCWSTR host);
	static string HttpGet(LPCWSTR uri, HINTERNET hHandle, LPCWSTR host);
	static string ByteArrayToHex(byte* Buffer, int BufferLen);
	static VARIANT InvokeMember(mscorlib::_AppDomainPtr pDefaultDomain, mscorlib::_AssemblyPtr dll, BSTR Typename, BSTR Method, int Numargs, ...);
	static VARIANT InvokeMember2(mscorlib::_AppDomainPtr pDefaultDomain, BSTR dll, BSTR Typename, BSTR Method, int Numargs, ...);
	static SAFEARRAY* LoadDecryptResource(LPCWSTR resource);
	static SAFEARRAY* LoadResourceF(LPCWSTR resource);
	static wchar_t* StringToWide(string str);
	static const char* PadNull(byte* data, int datalen);
	static void ReadBytes(byte* source, byte* destination, int length);
	static int Distance(int X1, int Y1, int X2, int Y2);

	static string Replace(string base, string find, string replace);
};

//! Maintains a collection of substrings that are
//! delimited by a string of one or more characters
class Splitter {
	//! Contains the split tokens
	std::vector<std::string> _tokens;
public:
	//! Subscript type for use with operator[]
	typedef std::vector<std::string>::size_type size_type;
public:
	//! Create and initialize a new Splitter
	//!
	//! \param[in] src The string to split
	//! \param[in] delim The delimiter to split the string around
	Splitter ( const std::string& src, const std::string& delim )
	{
		reset ( src, delim );
	}

	//! Retrieve a split token at the specified index
	//!
	//! \param[in] i The index to search for a token
	//! \return The token at the specified index
	//! \throw std::out_of_range If the index is invalid
	std::string& operator[] ( size_type i )
	{
		return _tokens.at ( i );
	}

	//! Retrieve the number of split tokens
	//!
	//! \return The number of split tokesn
	size_type size() const
	{
		return _tokens.size();
	}

	//! Re-initialize with a new soruce and delimiter
	//!
	//! \param[in] src The string to split
	//! \param[in] delim The delimiter to split the string around
	void reset ( const std::string& src, const std::string& delim )
	{
		std::vector<std::string> tokens;
		std::string::size_type start = 0;
		std::string::size_type end;

		for ( ; ; ) {
			end = src.find ( delim, start );
			tokens.push_back ( src.substr ( start, end - start ) );

			// We just copied the last token
			if ( end == std::string::npos )
				break;

			// Exclude the delimiter in the next search
			start = end + delim.size();
		}

		_tokens.swap ( tokens );
	}
};

