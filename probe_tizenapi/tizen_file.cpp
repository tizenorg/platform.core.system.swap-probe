/*
 *  DA probe
 *
 * Copyright (File::*c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 * Anastasia Lyupa <a.lyupa@samsung.com>
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
 * - Samsung RnD Institute Russia
 *
 */

#include <FIo.h>
#include <linux/limits.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"

#include "binproto.h"

static enum DaOptions _sopt = OPT_FILE;

namespace Tizen {
namespace Io {

result File::Construct(const Tizen::Base::String& filePath,
		const Tizen::Base::String& openMode, bool createParentDirectories) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode, bool createParentDirectories);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t	probeInfo;
	int blockresult;
	bool bfiltering = true;
	char temp_path[PATH_MAX];
	char temp_mode[PATH_MAX];
	FileAttributes attr;
	long long size = 0L;

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
		char buffer[PATH_MAX];

		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp_path,filePath.GetPointer());
		WcharToChar(temp_mode,openMode.GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, API_ID_result_File__Construct_const_Tizen__Base__String__filePath__const_Tizen__Base__String__openMode__bool_createParentDirectories_,
				  "ssd", absolutize_filepath(buffer,temp_path),
				  temp_mode, createParentDirectories);
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_OPEN, size, temp_path);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}

	return ret;
}

result File::Construct(const Tizen::Base::String& filePath,
		const Tizen::Base::String& openMode) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const Tizen::Base::String& openMode);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t	probeInfo;
	int blockresult;
	bool bfiltering = true;
	char temp_path[PATH_MAX];
	char temp_mode[PATH_MAX];
	FileAttributes attr;
	long long size = 0L;

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
		char buffer[PATH_MAX];

		WcharToChar(temp_path,filePath.GetPointer());
		WcharToChar(temp_mode,openMode.GetPointer());
		// Comment this because of fault during Internet application profiling
		// (it closes unexpectedly) but for DATizenTestApp it is ok
		// There is the same problem at File::~File(void)
		//if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
		//	size = attr.GetFileSize();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Construct_const_Tizen__Base__String__filePath__const_Tizen__Base__String__openMode_,
				  "ss", absolutize_filepath(buffer, temp_path), temp_mode);
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_OPEN, size, temp_path);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}

	return ret;
}

result File::Construct(const Tizen::Base::String& filePath,
		const char *pOpenMode) {
	typedef result (File::*methodType)(const Tizen::Base::String& filePath,
			const char *pOpenMode);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t	probeInfo;
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	FileAttributes attr;
	long long size = 0L;

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
		char buffer[PATH_MAX];

		WcharToChar(temp,filePath.GetPointer());
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Construct_const_Tizen__Base__String__filePath__const_char__pOpenMode_,
				  "sp", absolutize_filepath(buffer, temp),
				  voidp_to_uint64(pOpenMode));
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_OPEN, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	FileAttributes attr;
	long long size = 0L;

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
		WcharToChar(temp,filePath.GetPointer());
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Construct_const_Tizen__Base__String__filePath__const_char__pOpenMode__const_Tizen__Base__ByteBuffer__secretKey_,
				  "sp", temp,
				  voidp_to_uint64(pOpenMode));
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_OPEN, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	FileAttributes attr;
	long long size = 0L;

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
		WcharToChar(temp,this->GetName().GetPointer());
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Flush_void_,
				  "", 0);
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_OTHER, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	FileAttributes attr;
	long long size = 0L;

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
		WcharToChar(temp,ret.GetPointer());
		if (E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_Tizen__Base__String_File__GetName_void__const,
				  "", 0);
		PACK_COMMON_END('p', ret.GetPointer(), res, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_OTHER, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	int nRead = 0;
	FileAttributes attr;
	long long size = 0L;

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
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp, this->GetName().GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Read_Tizen__Base__String__buffer_,
				  "x", (uint64_t)(&buffer));
		PACK_COMMON_END('x', 0, 0, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_READ_START, size, temp);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}

	ret = (this->*Readp)(buffer);

	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp, this->GetName().GetPointer());
		nRead = buffer.GetLength();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Read_Tizen__Base__String__buffer_,
				  "x", (uint64_t)(&buffer));
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(nRead, (unsigned long)this, FD_API_READ_END, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	int nRead = 0;
	FileAttributes attr;
	long long size = 0L;

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
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp, this->GetName().GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Read_Tizen__Base__ByteBuffer__buffer_,
				  "x", (uint64_t)(&buffer));
		PACK_COMMON_END('x', 0, 0, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_READ_START, size, temp);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}

	ret = (this->*Readp)(buffer);

	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp, this->GetName().GetPointer());
		buffer.GetInt(nRead);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Read_Tizen__Base__ByteBuffer__buffer_,
				  "x", (uint64_t)(&buffer));
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(nRead, (unsigned long)this, FD_API_READ_END, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	int nRead = 0;
	FileAttributes attr;
	long long size = 0L;

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
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp, this->GetName().GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_int_File__Read_void__buffer__int_length_,
				  "xd", (uint64_t)(buffer), length);
		PACK_COMMON_END('d', 0, 0, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_READ_START, size, temp);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}

	ret = (this->*Readp)(buffer, length);
	result res = GetLastResult();

	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp, this->GetName().GetPointer());
		nRead = Tell() - nRead;

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_int_File__Read_void__buffer__int_length_,
				  "xd", (uint64_t)(buffer), length);
		PACK_COMMON_END('d', ret, res, blockresult);
		PACK_RESOURCE(nRead, (unsigned long)this, FD_API_READ_END, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	char temp_pos[PATH_MAX];
	FileAttributes attr;
	long long size = 0;

	if (!Seekp) {
		probeBlockStart();

		void *tmpPtr = dlsym(RTLD_NEXT,
				"_ZN5Tizen2Io4File4SeekENS0_16FileSeekPositionEl");

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

		 if(FILESEEKPOSITION_BEGIN == position)
			 strcpy(temp_pos, "FILESEEKPOSITION_BEGIN");
		 else if(FILESEEKPOSITION_CURRENT == position)
			 strcpy(temp_pos, "FILESEEKPOSITION_CURRENT");
		 else if(FILESEEKPOSITION_END == position)
			 strcpy(temp_pos, "FILESEEKPOSITION_END");
		 else

		sprintf(temp_pos, "%d", position);

		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp, this->GetName().GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Seek_FileSeekPosition_position__long_offset_,
				  "sx", temp_pos, (uint64_t)(offset));
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_OTHER, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	FileAttributes attr;
	long long size = 0L;

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
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp,this->GetName().GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_int_File__Tell_void__const,
				  "", 0);
		PACK_COMMON_END('d', ret, res, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_OTHER, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	FileAttributes attr;
	long long size = 0L;

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
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp,this->GetName().GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Truncate_int_length_,
				  "d", length);
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_OTHER, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	int nWritten = 0;
	FileAttributes attr;
	long long size = 0L;

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
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp, this->GetName().GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Write_const_void__buffer__int_length_,
				  "xd", (uint64_t)(buffer), length);
		PACK_COMMON_END('x', 0, 0, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_WRITE_START, size, temp);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}

	ret = (this->*Writep)(buffer, length);

	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp, this->GetName().GetPointer());
		nWritten = Tell() - nWritten;

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Write_const_void__buffer__int_length_,
				  "xd", (uint64_t)(buffer), length);
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(nWritten, (unsigned long)this, FD_API_WRITE_END, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp[PATH_MAX];
	int nWritten = 0;
	FileAttributes attr;
	long long size = 0L;

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
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp,this->GetName().GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Write_const_Tizen__Base__ByteBuffer__buffer_,
				  "x", (uint64_t)(&buffer));
		PACK_COMMON_END('x', 0, 0, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_WRITE_START, size, temp);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}

	ret = (this->*Writep)(buffer);

	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp,this->GetName().GetPointer());
		nWritten = Tell() - nWritten;

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Write_const_Tizen__Base__ByteBuffer__buffer_,
				  "x", (uint64_t)(&buffer));
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(nWritten, (unsigned long)this, FD_API_WRITE_END, size, temp);
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
	int blockresult;
	bool bfiltering = true;
	char temp_buf[PATH_MAX];
	char temp_path[PATH_MAX];
	int nWritten = 0;
	FileAttributes attr;
	long long size = 0L;

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
		WcharToChar(temp_buf, buffer.GetPointer());
		if(E_SUCCESS == File::GetAttributes(this->GetName(), attr))
			size = attr.GetFileSize();
		WcharToChar(temp_path, this->GetName().GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Write_const_Tizen__Base__String__buffer_,
				  "s", temp_buf);
		PACK_COMMON_END('x', 0, 0, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_WRITE_START, size, temp_path);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}

	ret = (this->*Writep)(buffer);

	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);
		WcharToChar(temp_buf,buffer.GetPointer());
		WcharToChar(temp_path,this->GetName().GetPointer());
		nWritten = Tell() - nWritten;
		if(E_SUCCESS == File::GetAttributes(this->GetName(),attr))
			size = attr.GetFileSize();
		nWritten = Tell() - nWritten;

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_result_File__Write_const_Tizen__Base__String__buffer_,
				  "s", temp_buf);
		PACK_COMMON_END('x', ret, ret, blockresult);
		PACK_RESOURCE(nWritten, (unsigned long)this, FD_API_WRITE_END, size, temp_path);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

File::~File(void) {
	typedef void (File::*methodType)();
	static methodType FileDp = 0;
	probeInfo_t	probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;
	// FileAttributes attr;
	long long size = 0L;

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
// error occur if File class was failed to contruct
		// if (ret == E_SUCCESS &&
		//     E_SUCCESS == File::GetAttributes(this->GetName(), attr))
		//     size = attr.GetFileSize();
		preBlockEnd();
	}

	(this->*FileDp)();

	if(postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
				  API_ID_File___File_void_,
				  "", 0);
		PACK_COMMON_END('n', 0, 0, blockresult);
		PACK_RESOURCE(0, (unsigned long)this, FD_API_CLOSE, size, "");
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
