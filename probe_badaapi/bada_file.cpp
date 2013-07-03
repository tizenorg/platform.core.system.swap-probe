/*
 *  DA probe
 *
 * Copyright (File::*c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 * 
 */

#include <FBase.h>
//#include <FIo.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"

#include "binproto.h"

using namespace Tizen::Base;

enum FileSeekPosition {
	FILESEEKPOSITION_BEGIN, /**<The beginning of the file */
	FILESEEKPOSITION_CURRENT, /**<The current position of the file */
	FILESEEKPOSITION_END
/**<The end of the file */
};

static enum DaOptions _sopt = OPT_FILE;

namespace Tizen {
namespace Io {

class FileAttributes {
public:
	virtual bool Equals(const Object& object) const;
	FileAttributes(const FileAttributes &fileAttributes);
	FileAttributes(void);
	DateTime GetDateTime(void) const;
	long long GetFileSize(void) const;
	virtual int GetHashCode(void) const;
	DateTime GetLastModifiedTime(void) const;
	bool IsDirectory(void) const;
	bool IsHidden(void) const;
	bool IsReadOnly(void) const;
	FileAttributes & operator=(const FileAttributes &fileAttributes);
	virtual ~FileAttributes(void);
};

class File {
public:

/* deprecated apis
 *   
	result Construct(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode, bool secureMode,
			bool createParentDirectories);
	result Construct(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode, bool secureMode,
			const Tizen::Base::ByteBuffer& key, bool createParentDirectories);
	
	result Construct(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode,
			const Tizen::Base::ByteBuffer& secretKey);
*/	
	result Construct(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode, bool createParentDirectories);
	result Construct(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode);
	
	result Construct(const Tizen::Base::String& filePath,
			const char *pOpenMode);
	result Construct(const Tizen::Base::String& filePath,
			const char *pOpenMode, const Tizen::Base::ByteBuffer &seretKey);

	
	/*
	 File(void);
	 */
	result Flush(void);

	Tizen::Base::String GetName(void) const;
	result Read(Tizen::Base::String& buffer);
	result Read(Tizen::Base::ByteBuffer& buffer);
	int Read(void *buffer, int length);
	result Seek(FileSeekPosition position, long offset);

	int Tell(void) const;

	result Truncate(int length);

	result Write(const void *buffer, int length);

	result Write(const Tizen::Base::ByteBuffer& buffer);
	result Write(const Tizen::Base::String& buffer);
	virtual ~File(void);
	/*
	 // static methods
	 static result ConvertToSecureFile(const Tizen::Base::String &plainFilePath,
	 const Tizen::Base::String &secureFilePath);
	 static result ConvertToSecureFile(const Tizen::Base::String &plainFilePath,
	 const Tizen::Base::String &secureFilePath,
	 const Tizen::Base::ByteBuffer& key);
	 static result Copy(const Tizen::Base::String& srcFilePath,
	 const Tizen::Base::String& destFilePath, bool failIfExist);
	 static result GetAttributes(const Tizen::Base::String& filePath,
	 FileAttributes& attribute);
	 static const Tizen::Base::String GetFileExtension(
	 const Tizen::Base::String& filePath);

	 static const Tizen::Base::String GetFileName(const Tizen::Base::String& filePath);

	 const Tizen::Base::String& filePath);
	 static bool IsEncrypted(const Tizen::Base::String &filePath);
	 static bool IsFileExist(const Tizen::Base::String& filePath);
	 static result Move(const Tizen::Base::String& oldFilePath,
	 const Tizen::Base::String& newFilePath);
	 static result Remove(const Tizen::Base::String& filePath);
	 //	static Tizen::Base::Collection::IList *GetPathsToAppsN(void);
	 */
};

//File::File(void) {
//static (File::*Filep)();
//}

result File::Construct(const Tizen::Base::String& filePath,
		const Tizen::Base::String& openMode, bool createParentDirectories) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode, bool createParentDirectories);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp_path[50];
	char temp_mode[50];
	//FileAttributes attr;

	if (!Constructp) {
		probeBlockStart();
		void *tmpPtr = dlsym(RTLD_NEXT,
				"_ZN5Tizen2Io4File9ConstructERKNS_4Base6StringES5_b");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Constructp)(filePath, openMode, createParentDirectories);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Construct",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 WcharToChar(temp_path,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,%s",temp_path);
		 WcharToChar(temp_mode,openMode.GetPointer());
		 log.length += sprintf(log.data + log.length,", %s",temp_mode);
		 log.length += sprintf(log.data + log.length,", %s`,%ld",(createParentDirectories == 0 ? "false" : "true"),ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
//		 WcharToChar(temp,filePath.GetPointer());
//		 WcharToChar(temp,this->GetName().GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OPEN,temp_path);
		 //callstack

//		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);	 
		 
		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE,
				  "ssd", temp_path, temp_mode, createParentDirectories);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_OPEN, 0, temp_path);
		FLUSH_LOCAL_BUF();
		 
		 postBlockEnd();
	}

	return ret;
}

/* deprecated apis
result File::Construct(const Tizen::Base::String& filePath,
		const Tizen::Base::String& openMode, bool secureMode,
		bool createParentDirectories) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode, bool secureMode,
			bool createParentDirectories);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	//FileAttributes attr;

	if (!Constructp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT,
				"_ZN5Tizen2Io4File9ConstructERKNS_4Base6StringES5_bb");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Constructp)(filePath, openMode, secureMode,
			createParentDirectories);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Construct",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 WcharToChar(temp,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,%s",temp);
		 WcharToChar(temp,openMode.GetPointer());
		 log.length += sprintf(log.data + log.length,", %s",temp);
		 log.length += sprintf(log.data + log.length,", %s,%s`,%ld",(secureMode == 0 ? "false" : "true"),(createParentDirectories == 0 ? "false" : "true"),ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OPEN,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);
		 postBlockEnd();
	}

	return ret;
}

result File::Construct(const Tizen::Base::String& filePath,
		const Tizen::Base::String& openMode, bool secureMode,
		const Tizen::Base::ByteBuffer& key, bool createParentDirectories) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode, bool secureMode,
			const Tizen::Base::ByteBuffer& key, bool createParentDirectories);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	//FileAttributes attr;

	if (!Constructp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT,
						"_ZN5Tizen2Io4File9ConstructERKNS_4Base6StringES5_bRKNS2_10ByteBufferEb");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Constructp)(filePath, openMode, secureMode, key,
			createParentDirectories);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Construct",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 WcharToChar(temp,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,%s",temp);
		 WcharToChar(temp,openMode.GetPointer());
		 log.length += sprintf(log.data + log.length,", %s",temp);
		 log.length += sprintf(log.data + log.length,", %s,0x%x,%s`,%ld",(secureMode == 0 ? "false" : "true"),(unsigned int)&key,(createParentDirectories == 0 ? "false" : "true"),ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OPEN,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);
		 postBlockEnd();
	}

	return ret;
}
*/
result File::Construct(const Tizen::Base::String& filePath,
		const Tizen::Base::String& openMode) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp_path[50];
	char temp_mode[50];
	//FileAttributes attr;

	if(!Constructp) {
		probeBlockStart();
		void *tmpPtr = dlsym(RTLD_NEXT,
				"_ZN5Tizen2Io4File9ConstructERKNS_4Base6StringES5_");

		if(tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Constructp)(filePath, openMode);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Construct",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 WcharToChar(temp_path,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,%s",temp_path);
		 WcharToChar(temp_mode,openMode.GetPointer());
		 log.length += sprintf(log.data + log.length,", %s`,%ld",temp_mode,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
//		 WcharToChar(temp,filePath.GetPointer());
//		 WcharToChar(temp,this->GetName().GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OPEN,temp_path);
		 //callstack

//		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "ss", temp_path, temp_mode);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_OPEN, 0, temp_path);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

/* deprecated apis
result File::Construct(const Tizen::Base::String& filePath,
		const Tizen::Base::String& openMode,
		const Tizen::Base::ByteBuffer& secretKey) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const Tizen::Base::String &openMode,
			const Tizen::Base::ByteBuffer& secretKey);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	//FileAttributes attr;

	if(!Constructp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT,
						"_ZN5Tizen2Io4File9ConstructERKNS_4Base6StringES5_RKNS2_10ByteBufferE");

		if(tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Constructp)(filePath, openMode, secretKey);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Construct",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 WcharToChar(temp,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,%s",temp);
		 WcharToChar(temp,openMode.GetPointer());
		 log.length += sprintf(log.data + log.length,", %s",temp);
		 log.length += sprintf(log.data + log.length,", 0x%x`,%ld",(unsigned int)&secretKey,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OPEN,temp);
		 //callstack

//		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);
		 postBlockEnd();
	}
	
	return ret;
}
*/
result File::Construct(const Tizen::Base::String& filePath,
		const char *pOpenMode) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const char *pOpenMode);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	//FileAttributes attr;

	if(!Constructp) {
		probeBlockStart();
		void *tmpPtr = dlsym(RTLD_NEXT,
				"_ZN5Tizen2Io4File9ConstructERKNS_4Base6StringEPKc");

		if(tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Constructp)(filePath, pOpenMode);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Construct",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 WcharToChar(temp,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,%s",temp);
		 log.length += sprintf(log.data + log.length,", %s`,%ld",pOpenMode,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,filePath.GetPointer());
//		 WcharToChar(temp,this->GetName().GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OPEN,temp);
		 //callstack

//		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "sp", temp, pOpenMode);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_OPEN, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

result File::Construct(const Tizen::Base::String& filePath,
		const char *pOpenMode,
		const Tizen::Base::ByteBuffer& secretKey) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const char *pOpenMode,
			const Tizen::Base::ByteBuffer& secretKey);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	//FileAttributes attr;

	if(!Constructp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT,
						"_ZN5Tizen2Io4File9ConstructERKNS_4Base6StringEPKcRKNS2_10ByteBufferE");

		if(tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Constructp)(filePath, pOpenMode, secretKey);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Construct",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 WcharToChar(temp,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,%s",temp);
		 log.length += sprintf(log.data + log.length,", %s",pOpenMode);
		 log.length += sprintf(log.data + log.length,", 0x%x`,%ld",(unsigned int)&secretKey,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,filePath.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OPEN,temp);
		 //callstack

//		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "sp", temp, pOpenMode);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_OPEN, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}
	
	return ret;
}

result File::Flush(void) {
	typedef result (File::*methodType)(void);
	static methodType Flushp = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	//FileAttributes attr;

	if (!Flushp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT, "_ZN5Tizen2Io4File5FlushEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Flush");
			exit(0);
		}

		memcpy(&Flushp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Flushp)();

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Flush",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 log.length += sprintf(log.data + log.length,"`,`,%ld",ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,this->GetName().GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OTHER,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_OTHER, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

Tizen::Base::String File::GetName(void) const{
	typedef Tizen::Base::String (File::*methodType)(void) const;
	static methodType GetNamep = 0;
	Tizen::Base::String ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	//FileAttributes attr;

	if (!GetNamep) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT, "_ZNK5Tizen2Io4File7GetNameEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::GetName");
			exit(0);
		}

		memcpy(&GetNamep, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*GetNamep)();
	result res = GetLastResult();

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::GetName",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 WcharToChar(temp,ret.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,`,%s",temp);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",res,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OTHER,temp);
		 //callstack

//		 if(E_SUCCESS != res || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "", 0);
		PACK_COMMON_END(ret.GetPointer(), res, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_OTHER, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

result File::Read(Tizen::Base::String& buffer) {
	typedef result (File::*methodType)(Tizen::Base::String & buffer);
	static methodType Readp = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	int nRead = 0;
	//FileAttributes attr;

	if (!Readp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT,
				"_ZN5Tizen2Io4File4ReadERNS_4Base6StringE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Read");
			exit(0);
		}

		memcpy(&Readp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Readp)(buffer);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Read",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 log.length += sprintf(log.data + log.length,"`,0x%x`,%ld",(unsigned int)&buffer,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,this->GetName().GetPointer());
		 nRead = buffer.GetLength();
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,%d`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,nRead,(unsigned int)this,FD_FILE,FD_API_READ,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "x", (unsigned int)&buffer);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(nRead, (unsigned int)this, FD_FILE, FD_API_READ, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

result File::Read(Tizen::Base::ByteBuffer& buffer) {
	typedef result (File::*methodType)(Tizen::Base::ByteBuffer & buffer);
	static methodType Readp = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	int nRead = 0;
	//FileAttributes attr;

	if (!Readp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT,
				"_ZN5Tizen2Io4File4ReadERNS_4Base10ByteBufferE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Read");
			exit(0);
		}

		memcpy(&Readp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Readp)(buffer);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Read",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 log.length += sprintf(log.data + log.length,"`,0x%x`,%ld",(unsigned int)&buffer,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,this->GetName().GetPointer());
		 buffer.GetInt(nRead);
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,%d`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,nRead,(unsigned int)this,FD_FILE,FD_API_READ,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "x", (unsigned int)&buffer);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(nRead, (unsigned int)this, FD_FILE, FD_API_READ, 0, temp);
		FLUSH_LOCAL_BUF();
		
		 postBlockEnd();
	}

	return ret;
}

int File::Read(void *buffer, int length) {
	typedef int (File::*methodType)(void *buffer, int length);
	static methodType Readp = 0;
	int ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	int nRead = 0;
	//FileAttributes attr;

	if (!Readp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT, "_ZN5Tizen2Io4File4ReadEPvi");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Read");
			exit(0);
		}

		memcpy(&Readp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		nRead = Tell();
		preBlockEnd();
	}

	ret = (this->*Readp)(buffer, length);
	result res = GetLastResult();

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Read",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 log.length += sprintf(log.data + log.length,"`,0x%x, %d`,%d",(unsigned int)buffer,length,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,this->GetName().GetPointer());
		 nRead = Tell() - nRead;
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,%d`,0x%x`,%d`,%d`,?`,%s",res,blockresult,(unsigned int)CALLER_ADDRESS,nRead,(unsigned int)this,FD_FILE,FD_API_READ,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "xd", (unsigned int)buffer, length);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(nRead, (unsigned int)this, FD_FILE, FD_API_READ, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

result File::Seek(FileSeekPosition position, long offset) {
	typedef result (File::*methodType)(FileSeekPosition position, long offset);
	static methodType Seekp = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	char temp_pos[50];
	//FileAttributes attr;

	if (!Seekp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT,
				"_ZN5Tizen2Io4File4SeekE16FileSeekPositionl");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Seek");
			exit(0);
		}

		memcpy(&Seekp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Seekp)(position, offset);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Seek",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 if(FILESEEKPOSITION_BEGIN == position) {
			 strcpy(temp_pos, "FILESEEKPOSITION_BEGIN");
			 log.length += sprintf(log.data + log.length,"`,FILESEEKPOSITION_BEGIN");
		 } else if(FILESEEKPOSITION_CURRENT == position) {
			 strcpy(temp_pos, "FILESEEKPOSITION_CURRENT");
			 log.length += sprintf(log.data + log.length,"`,FILESEEKPOSITION_CURRENT");
		 } else if(FILESEEKPOSITION_END == position) {
			 strcpy(temp_pos, "FILESEEKPOSITION_END");
			 log.length += sprintf(log.data + log.length,"`,FILESEEKPOSITION_END");
		 } else {
			 sprintf(temp_pos, "%d", position);
			 log.length += sprintf(log.data + log.length,"`,%d",position);
		 }
		 log.length += sprintf(log.data + log.length,", %ld`,%ld",offset,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,this->GetName().GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OTHER,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "sx", temp_pos, offset);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_OTHER, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

int File::Tell(void) const {
	typedef int (File::*methodType)(void) const;
	static methodType Tellp = 0;
	int ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	//FileAttributes attr;

	if (!Tellp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT, "_ZNK5Tizen2Io4File4TellEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Tell");
			exit(0);
		}

		memcpy(&Tellp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Tellp)();
	result res = GetLastResult();

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Tell",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 log.length += sprintf(log.data + log.length,"`,`,%d",ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,this->GetName().GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",res,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OTHER,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_OTHER, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

result File::Truncate(int length) {
	typedef result (File::*methodType)(int length);
	static methodType Truncatep = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	//FileAttributes attr;

	if (!Truncatep) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT, "_ZN5Tizen2Io4File8TruncateEi");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Truncate");
			exit(0);
		}

		memcpy(&Truncatep, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	ret = (this->*Truncatep)(length);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Truncate",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 log.length += sprintf(log.data + log.length,"`,%d`,%ld",length,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,this->GetName().GetPointer());
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_OTHER,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "d", length);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_OTHER, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

result File::Write(const void *buffer, int length) {
	typedef result (File::*methodType)(const void *buffer, int length);
	static methodType Writep = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	int nWritten = 0;
	//FileAttributes attr;

	if (!Writep) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT, "_ZN5Tizen2Io4File5WriteEPKvi");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Write");
			exit(0);
		}

		memcpy(&Writep, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		nWritten = Tell();
		preBlockEnd();
	}

	ret = (this->*Writep)(buffer, length);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Write",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 log.length += sprintf(log.data + log.length,"`,0x%x, %d`,%ld",(unsigned int)buffer,length,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,this->GetName().GetPointer());
		 nWritten = Tell() - nWritten;
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,%d`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,nWritten,(unsigned int)this,FD_FILE,FD_API_WRITE,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "xd", (unsigned int)buffer, length);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(nWritten, (unsigned int)this, FD_FILE, FD_API_WRITE, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

result File::Write(const Tizen::Base::ByteBuffer& buffer) {
	typedef result (File::*methodType)(const Tizen::Base::ByteBuffer& buffer);
	static methodType Writep = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp[50];
	int nWritten = 0;
	//FileAttributes attr;

	if (!Writep) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT, "_ZN5Tizen2Io4File5WriteERKNS_4Base10ByteBufferE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Write");
			exit(0);
		}

		memcpy(&Writep, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		nWritten = Tell();
		preBlockEnd();
	}

	ret = (this->*Writep)(buffer);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Write",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 log.length += sprintf(log.data + log.length,"`,0x%x`,%ld",(unsigned int)&buffer,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp,this->GetName().GetPointer());
		 nWritten = Tell() - nWritten;
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,%d`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,nWritten,(unsigned int)this,FD_FILE,FD_API_WRITE,temp);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "x", (unsigned int)&buffer);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(nWritten, (unsigned int)this, FD_FILE, FD_API_WRITE, 0, temp);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}

	return ret;
}

result File::Write(const Tizen::Base::String& buffer) {
	typedef result (File::*methodType)(const Tizen::Base::String& buffer);
	static methodType Writep = 0;
	result ret;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	char temp_buf[50];
	char temp_path[50];
	int nWritten = 0;
	//FileAttributes attr;

	if (!Writep) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT, "_ZN5Tizen2Io4File5WriteERKNS_4Base6StringE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::Write");
			exit(0);
		}

		memcpy(&Writep, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		nWritten = Tell();
		preBlockEnd();
	}

	ret = (this->*Writep)(buffer);

	if(postBlockBegin(blockresult)) {
		 log.type = 0;
		 log.length = 0;
		 log.data[0] = '\0';
		 log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
		 LC_RESOURCE, probeInfo.eventIndex, "File::Write",
		 probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		 //Input,ret
		 WcharToChar(temp_buf,buffer.GetPointer());
		 log.length += sprintf(log.data + log.length,"`,%s`,%ld",temp_buf,ret);
		 //PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		 //File::GetAttributes(this->GetName(),attr);
		 WcharToChar(temp_path,this->GetName().GetPointer());
		 nWritten = Tell() - nWritten;
		 log.length += sprintf(log.data + log.length,"`,0`,%lu`,%d`,%u`,%d`,0x%x`,%d`,%d`,?`,%s",ret,blockresult,(unsigned int)CALLER_ADDRESS,nWritten,(unsigned int)this,FD_FILE,FD_API_WRITE,temp_path);
		 //callstack

		 //		 if(E_SUCCESS != ret || blockresult == 2) {
//			 log.length += sprintf(log.data + log.length,
//					 "`,\ncallstack_start`,`,callstack_end");
//		 } else{
			 log.length += sprintf(log.data + log.length,
					 "`,\ncallstack_start`,");
			 getBacktraceString(&log, 4096 - log.length - 17);
			 log.length += sprintf(log.data + log.length, "`,callstack_end");
//		 }

		 printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "s", temp_buf);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_RESOURCE(nWritten, (unsigned int)this, FD_FILE, FD_API_WRITE, 0, temp_path);
		FLUSH_LOCAL_BUF();

		 postBlockEnd();
	}
	return ret;
}

File::~File(void) {
	typedef void (File::*methodType)();
	static methodType FileDp = 0;
	probeInfo_t	probeInfo;
	log_t	log;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!FileDp) {
		probeBlockStart();

		tmpPtr = dlsym(RTLD_NEXT, "_ZN5Tizen2Io4FileD1Ev");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Io::File::~File");
			exit(0);
		}

		memcpy(&FileDp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
//
	(this->*FileDp)();
//
	if(postBlockBegin(blockresult)) {
		log.type = 0;
		log.length = 0;
		log.data[0] = '\0';
		log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",
				LC_RESOURCE, probeInfo.eventIndex, "File::~File",
				probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		//Input,ret
		log.length += sprintf(log.data + log.length,"`,`,");
		//PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
		//File::GetAttributes(this->GetName(),attr);

		log.length += sprintf(log.data + log.length,"`,0`,0`,%d`,%u`,0`,0x%x`,%d`,%d`,?`,",blockresult,(unsigned int)CALLER_ADDRESS,(unsigned int)this,FD_FILE,FD_API_CLOSE);
		//callstack

//		if(E_SUCCESS != ret || blockresult == 2) {
//			log.length += sprintf(log.data + log.length,
//					"`,\ncallstack_start`,`,callstack_end");
//		} else{
			log.length += sprintf(log.data + log.length,
					"`,\ncallstack_start`,");
			getBacktraceString(&log, 4096 - log.length - 17);
			log.length += sprintf(log.data + log.length, "`,callstack_end");
//		}

		printLog(&log, MSG_LOG);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, "", 0);
		PACK_COMMON_END(0, 0, blockresult);
		PACK_RESOURCE(0, (unsigned int)this, FD_FILE, FD_API_CLOSE, 0, "");
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
}


// static methods
//static result File::ConvertToSecureFile(const Tizen::Base::String &plainFilePath,
//		const Tizen::Base::String &secureFilePath) {
//	typedef result (File::*methodType)(
//			const Tizen::Base::String &plainFilePath,
//			const Tizen::Base::String &secureFilePath);
//	static methodType ConvertToSecureFilep = 0;
//
//}
//static result File::ConvertToSecureFile(const Tizen::Base::String &plainFilePath,
//		const Tizen::Base::String &secureFilePath,
//		const Tizen::Base::ByteBuffer& key) {
//	typedef result (File::*methodType)(
//			const Tizen::Base::String &plainFilePath,
//			const Tizen::Base::String &secureFilePath,
//			const Tizen::Base::ByteBuffer& key);
//	static methodType ConvertToSecureFilep = 0;
//
//}
//static result File::Copy(const Tizen::Base::String& srcFilePath,
//		const Tizen::Base::String& destFilePath, bool failIfExist) {
//	typedef result (File::*methodType)(const Tizen::Base::String& srcFilePath,
//			const Tizen::Base::String& destFilePath, bool failIfExist);
//	static methodType Copyp = 0;
//
//}
//static result File::GetAttributes(const Tizen::Base::String& filePath,
//		FileAttributes& attribute) {
//	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
//			FileAttributes& attribute);
//	static methodType GetAttributesp = 0;
//
//}
//static const Tizen::Base::String File::GetFileExtension(
//		const Tizen::Base::String& filePath) {
//	typedef const Tizen::Base::String (File::*methodType)(
//			const Tizen::Base::String& filePath);
//	static methodType GetFileExtensionp = 0;
//
//}
//const Tizen::Base::String File::GetFileName(
//		const Tizen::Base::String& filePath) {
//	typedef const Tizen::Base::String (File::*methodType)(
//			const Tizen::Base::String& filePath);
//	static methodType GetFileNamep = 0;
//	Tizen::Base::String ret;
//
//	if (!GetFileNamep) {
//			probeBlockStart();
//			void *tmpPtr = dlsym(RTLD_NEXT,
//					"_ZN5Tizen2Io4File11GetFileNameERKNS_4Base6StringE");
//
//			if (tmpPtr == NULL || dlerror() != NULL) {
//				perror("dlsym failed : Tizen::Io::File::GetFileName");
//				exit(0);
//			}
//
//			memcpy(&GetFileNamep, &tmpPtr, sizeof(tmpPtr));
//			probeBlockEnd();
//		}
//	ret = (File::(*GetFileNamep))(filePath);
//
//return ret;
//}
//static bool File::IsEncrypted(const Tizen::Base::String &filePath) {
//	typedef bool (File::*methodType)(const Tizen::Base::String &filePath);
//	static methodType IsEncryptedp = 0;
//
//}
//static bool File::IsFileExist(const Tizen::Base::String& filePath) {
//	typedef bool (File::*methodType)(const Tizen::Base::String& filePath);
//	static methodType IsFileExistp = 0;
//
//}
//static result File::Move(const Tizen::Base::String& oldFilePath,
//		const Tizen::Base::String& newFilePath) {
//	typedef result (File::*methodType)(const Tizen::Base::String& oldFilePath,
//			const Tizen::Base::String& newFilePath);
//	static methodType Movep = 0;
//
//}
//static result File::Remove(const Tizen::Base::String& filePath) {
//	typedef result (File::*methodType)(const Tizen::Base::String& filePath);
//	static methodType Removep = 0;
//
//}

}
}
