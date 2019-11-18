#ifndef JFileH
#define JFileH

class JFile
{

public:

	enum DATA_TYPE
	{
		GRAPH_TYPE     = 0,
		AUTO_TYPE      = 1,
		GPTS_TYPE,
      GPTSRCS_TYPE,
		NA10_TYPE,
      SIGMA_TYPE,
      SPECTRUM_TYPE,
      DIGIWIN_TYPE,
      BINARY_TYPE    = 98,
      ASCII_TYPE     = 99,
      OTHER_TYPE     = 100,
   };

	enum FILE_ERROR
   {
      F_NO_ERROR,
      F_CREATE_ERROR,
      F_OPEN_ERROR,
      F_CLOSE_ERROR,
      F_SEEK_ERROR,
      F_READ_ERROR,
      F_WRITE_ERROR,
      F_MEM_ERROR,
      F_DIR_ERROR,
      F_DRIVE_ERROR,
      F_ABORT_ERROR,
      F_ERASE_ERROR,
      F_PRINT_ERROR,
      F_FLUSH_ERROR,
      F_FORMAT_ERROR,
      F_CRC_ERROR,
      F_VERSION_ERROR,
      F_COMMS_ERROR,
      F_CONNECT_ERROR,
      F_TRANSFER_ERROR,
		F_CONFIG_ERROR,
      F_LOGIN_ERROR,
      F_COPY_ERROR,
      F_SIZE_ERROR,
      F_EOF,
		F_TEXTLINE_ERROR,
		F_DECRYPT_ERROR,
	};

private:

	DATA_TYPE DataType;
	char cFileDirection;

	HANDLE hFile;

	LARGE_INTEGER lnFileSize;
	LARGE_INTEGER lnFilePos;
	String sFileName;
	FILE_ERROR Error;

	char* pcASCIIBuf;
	DWORD dwASCIIBufSize;
	DWORD dwASCIIBufOffs;

	AnsiString sASCIIBuf;

	bool bAddASCIIChar(const char c)
	{
		if (c==0x0A)
			return true;
		else if (c!=0x0D)
			sASCIIBuf+=c;
		return false;
	}


//   void ShowError(const String sMessage, const String sHeading, const int nTimeout_sec=-1);


public:

	JFile(const char _cFileDirection='B', const DATA_TYPE _DataType=BINARY_TYPE);

	~JFile();

	void SetName(const String sName);
	String sGetName() const;

	void SetType(const DATA_TYPE _DataType)
	{
		DataType=_DataType;
	}

	DATA_TYPE GetType() const
	{
		return DataType;
	}

	void SetDirection(const char _cFileDirection)
	{
		cFileDirection=(char)toupper(cFileDirection);
	}

	HANDLE hGetFil()
	{
		return hFile;
	}

	FILE_ERROR Create(const String sName);
	FILE_ERROR Open(const String sName);
	FILE_ERROR Open();
	FILE_ERROR Close();

	FILE_ERROR Write(const DWORD dwOffset, const void* pData, const DWORD dwBytes); 				//32 bit
	FILE_ERROR Write(const LARGE_INTEGER lnOffset, const void* pData, const DWORD dwBytes);	//64 bit
	FILE_ERROR Write(const void* pData, const DWORD dwBytes);				  // 32/64 bit

	DWORD dwGetFilePos() const 			{return lnFilePos.LowPart;}	// 32 bit
	LARGE_INTEGER lnGetFilePos() const 	{return lnFilePos;}				// 64 bit

	FILE_ERROR Append(const void* pData, const DWORD dwBytes);	// 32/64 bit

	FILE_ERROR Read(const DWORD dwOffset, void* pData, const DWORD dwBytes);			// 32 bit
	FILE_ERROR Read(const LARGE_INTEGER lnOffset, void* pData, const DWORD dwBytes); // 64 bit
	FILE_ERROR Read(void* pData, const DWORD dwBytes);		// 32/64 bit
	FILE_ERROR Read(char* pszSt, const DWORD dwMaxLen);	// 32/64 bit
	FILE_ERROR Read(String& sText); // 32/64 bit

	FILE_ERROR Write(const String sText); // 32/64 bit

	FILE_ERROR Write(TFont* pFont);
	FILE_ERROR Read(TFont* pFont);

	FILE_ERROR Seek(const DWORD dwOffset);          // 32 bit
	FILE_ERROR Seek(const LARGE_INTEGER lnOffset);	// 64 bit

	FILE_ERROR GetSize(DWORD& dwSize);
	DWORD dwGetSize();
	FILE_ERROR GetSize(LARGE_INTEGER &lnSize);  // 64 bit

	FILE_ERROR GetPos(DWORD& dwPos);            // 32 bit
	FILE_ERROR GetPos(LARGE_INTEGER& lnPos);    // 64 bit

	FILE_ERROR Flush();

	FILE_ERROR Text(const String s);
	FILE_ERROR AppendText(const String s);

	FILE_ERROR TextLine(const String s);
	JFile::FILE_ERROR AppendTextLine(const String s);

	String sGetError(const FILE_ERROR Error);

	void ShowError(const FILE_ERROR Error, const int nTimeout_sec=-1);

	FILE_ERROR ReadASCIIFormat(int& yn);

	FILE_ERROR WriteSciHeader(const int yn, const long dn=0);

	FILE_ERROR ReadSciHeader(int& yn, DWORD& dwDataNum);

};

#endif
