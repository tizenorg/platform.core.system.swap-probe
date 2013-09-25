/*
 * this file genereted by <swap-probe> project with cmd <make headers>
 */

#ifndef __API_ID_MAP__
#define __API_ID_MAP__

#ifdef __cplusplus
extern "C"{
#endif

#define API_ID_result_UiApp__Execute_UiAppInstanceFactory_pUiAppFactory__const_IList__pArguments_                                               1 // result UiApp::Execute(UiAppInstanceFactory pUiAppFactory, const IList* pArguments)
#define API_ID_void__AppImpl__OnTerminate_void__user_data_                                                                                      2 // void _AppImpl::OnTerminate(void* user_data)
#define API_ID_void__AppInfo__SetAppState_AppState_appState_                                                                                    3 // void _AppInfo::SetAppState(AppState appState)
#define API_ID_void__UiAppImpl__OnBackground_void_                                                                                              4 // void _UiAppImpl::OnBackground(void)
#define API_ID_void__UiAppImpl__OnForeground_void_                                                                                              5 // void _UiAppImpl::OnForeground(void)

#define API_ID_result_Mutex__Create_void_                                                                                                       6 // result Mutex::Create(void)
#define API_ID_result_Mutex__Create_const_Tizen__Base__String__name_                                                                            7 // result Mutex::Create(const Tizen::Base::String& name)
#define API_ID_result_Mutex__Release_void_                                                                                                      8 // result Mutex::Release(void)
#define API_ID_result_Mutex__Acquire_void_                                                                                                      9 // result Mutex::Acquire(void)
#define API_ID_result_Mutex__TryToAcquire_void_                                                                                                 10 // result Mutex::TryToAcquire(void)
#define API_ID_result_Semaphore__Create_int_count_                                                                                              11 // result Semaphore::Create(int count)
#define API_ID_result_Semaphore__Create_const_Tizen__Base__String__name__int_count_                                                             12 // result Semaphore::Create(const Tizen::Base::String& name, int count)
#define API_ID_result_Semaphore__Acquire_long_timeout_                                                                                          13 // result Semaphore::Acquire(long timeout)
#define API_ID_result_Semaphore__TryToAcquire_void_                                                                                             14 // result Semaphore::TryToAcquire(void)
#define API_ID_result_Semaphore__Release_void_                                                                                                  15 // result Semaphore::Release(void)
#define API_ID_result_Monitor__Construct_void_                                                                                                  16 // result Monitor::Construct(void)
#define API_ID_result_Monitor__Enter_void_                                                                                                      17 // result Monitor::Enter(void)
#define API_ID_result_Monitor__Exit_void_                                                                                                       18 // result Monitor::Exit(void)
#define API_ID_result_Monitor__Wait_void_                                                                                                       19 // result Monitor::Wait(void)
#define API_ID_result_Monitor__Notify_void_                                                                                                     20 // result Monitor::Notify(void)
#define API_ID_result_Monitor__NotifyAll_void_                                                                                                  21 // result Monitor::NotifyAll(void)

#define API_ID_void___ThreadImpl__ThreadProc_void__params_                                                                                      22 // void *_ThreadImpl::ThreadProc(void* params)
#define API_ID_result_Thread__Sleep_long_milliSeconds_                                                                                          23 // result Thread::Sleep(long milliSeconds)
#define API_ID_result_Thread__Yield_void_                                                                                                       24 // result Thread::Yield(void)
#define API_ID_result_Thread__Exit_int_exitCode_                                                                                                25 // result Thread::Exit(int exitCode)
#define API_ID_result_Thread__Construct_ThreadType_threadType__long_stackSize__ThreadPriority_priority_                                         26 // result Thread::Construct(ThreadType threadType, long stackSize, ThreadPriority priority)
#define API_ID_result_Thread__Construct_long_stackSize__ThreadPriority_priority_                                                                27 // result Thread::Construct(long stackSize, ThreadPriority priority)
#define API_ID_result_Thread__Construct_const_Tizen__Base__String__name__long_stackSize__ThreadPriority_priority_                               28 // result Thread::Construct(const Tizen::Base::String &name, long stackSize, ThreadPriority priority)
#define API_ID_result_Thread__Construct_const_Tizen__Base__String__name__ThreadType_threadType__long_stackSize__ThreadPriority_priority_        29 // result Thread::Construct(const Tizen::Base::String &name, ThreadType threadType, long stackSize, ThreadPriority priority)
#define API_ID_result_Thread__Construct_IRunnable__target__long_stackSize__ThreadPriority_priority_                                             30 // result Thread::Construct(IRunnable &target, long stackSize, ThreadPriority priority)
#define API_ID_result_Thread__Construct_const_Tizen__Base__String__name__IRunnable__target__long_stackSize__ThreadPriority_priority_            31 // result Thread::Construct(const Tizen::Base::String &name, IRunnable &target, long stackSize, ThreadPriority priority)
#define API_ID_result_Thread__GetExitCode_int__exitCode__const                                                                                  32 // result Thread::GetExitCode(int &exitCode) const
#define API_ID_const_Tizen__Base__String___Thread__GetName_void__const                                                                          33 // const Tizen::Base::String & Thread::GetName(void) const
#define API_ID_result_Thread__Join_void_                                                                                                        34 // result Thread::Join(void)
#define API_ID_result_Thread__Start_void_                                                                                                       35 // result Thread::Start(void)
#define API_ID_result_Thread__Stop_void_                                                                                                        36 // result Thread::Stop(void)
#define API_ID_result_EventDrivenThread__Construct_long_stackSize__ThreadPriority_priority_                                                     37 // result EventDrivenThread::Construct(long stackSize, ThreadPriority priority)
#define API_ID_result_EventDrivenThread__Construct_const_Tizen__Base__String__name__long_stackSize__ThreadPriority_priority_                    38 // result EventDrivenThread::Construct(const Tizen::Base::String &name, long stackSize, ThreadPriority priority)
#define API_ID_result_EventDrivenThread__Quit__                                                                                                 39 // result EventDrivenThread::Quit()

#define API_ID_result_File__Construct_const_Tizen__Base__String__filePath__const_Tizen__Base__String__openMode__bool_createParentDirectories_   40 // result File::Construct(const Tizen::Base::String& filePath, const Tizen::Base::String& openMode, bool createParentDirectories)
#define API_ID_result_File__Construct_const_Tizen__Base__String__filePath__const_Tizen__Base__String__openMode_                                 41 // result File::Construct(const Tizen::Base::String& filePath, const Tizen::Base::String& openMode)
#define API_ID_result_File__Construct_const_Tizen__Base__String__filePath__const_char__pOpenMode_                                               42 // result File::Construct(const Tizen::Base::String& filePath, const char *pOpenMode)
#define API_ID_result_File__Construct_const_Tizen__Base__String__filePath__const_char__pOpenMode__const_Tizen__Base__ByteBuffer__secretKey_     43 // result File::Construct(const Tizen::Base::String& filePath, const char *pOpenMode, const Tizen::Base::ByteBuffer& secretKey)
#define API_ID_result_File__Flush_void_                                                                                                         44 // result File::Flush(void)
#define API_ID_Tizen__Base__String_File__GetName_void__const                                                                                    45 // Tizen::Base::String File::GetName(void) const
#define API_ID_result_File__Read_Tizen__Base__String__buffer_                                                                                   46 // result File::Read(Tizen::Base::String& buffer)
#define API_ID_result_File__Read_Tizen__Base__ByteBuffer__buffer_                                                                               47 // result File::Read(Tizen::Base::ByteBuffer& buffer)
#define API_ID_int_File__Read_void__buffer__int_length_                                                                                         48 // int File::Read(void *buffer, int length)
#define API_ID_result_File__Seek_FileSeekPosition_position__long_offset_                                                                        49 // result File::Seek(FileSeekPosition position, long offset)
#define API_ID_int_File__Tell_void__const                                                                                                       50 // int File::Tell(void) const
#define API_ID_result_File__Truncate_int_length_                                                                                                51 // result File::Truncate(int length)
#define API_ID_result_File__Write_const_void__buffer__int_length_                                                                               52 // result File::Write(const void *buffer, int length)
#define API_ID_result_File__Write_const_Tizen__Base__ByteBuffer__buffer_                                                                        53 // result File::Write(const Tizen::Base::ByteBuffer& buffer)
#define API_ID_result_File__Write_const_Tizen__Base__String__buffer_                                                                            54 // result File::Write(const Tizen::Base::String& buffer)
#define API_ID_File___File_void_                                                                                                                55 // File::~File(void)

#define API_ID_result_UiApp__AddFrame_const_Tizen__Ui__Controls__Frame__frame_                                                                  56 // result UiApp::AddFrame(const Tizen::Ui::Controls::Frame& frame)
#define API_ID_result_UiApp__RemoveFrame_const_Tizen__Ui__Controls__Frame__frame_                                                               57 // result UiApp::RemoveFrame(const Tizen::Ui::Controls::Frame &frame)
#define API_ID_void_Control__SetName_const_Tizen__Base__String__name_                                                                           58 // void Control::SetName(const Tizen::Base::String &name)
#define API_ID_result_Container__AddControl_const_Control__control_                                                                             59 // result Container::AddControl(const Control &control)
#define API_ID_result_Container__RemoveControl_const_Control__control_                                                                          60 // result Container::RemoveControl(const Control &control)
#define API_ID_result_Container__RemoveControl_int_index_                                                                                       61 // result Container::RemoveControl(int index)
#define API_ID_void_Container__RemoveAllControls_void_                                                                                          62 // void Container::RemoveAllControls(void)

#define API_ID__chart_timerThread                                                                                                               63 // _chart_timerThread
#define API_ID_da_mark                                                                                                                          64 // da_mark
#define API_ID_da_create_chart                                                                                                                  65 // da_create_chart
#define API_ID_da_create_series                                                                                                                 66 // da_create_series
#define API_ID_da_log                                                                                                                           67 // da_log

#define API_ID_malloc                                                                                                                           68 // malloc
#define API_ID_free                                                                                                                             69 // free
#define API_ID_realloc                                                                                                                          70 // realloc
#define API_ID_calloc                                                                                                                           71 // calloc

#define API_ID_void__operator_new_std__size_t_size__throw__std__bad_alloc_                                                                      72 // void *operator new(std::size_t size) throw (std::bad_alloc)
#define API_ID_void__operator_new___std__size_t_size__throw__std__bad_alloc_                                                                    73 // void *operator new[](std::size_t size) throw (std::bad_alloc)
#define API_ID_void_operator_delete_void__ptr__throw__                                                                                          74 // void operator delete(void *ptr) throw()
#define API_ID_void_operator_delete___void__ptr__throw__                                                                                        75 // void operator delete[](void *ptr) throw()
#define API_ID_void__operator_new_std__size_t_size__const_std__nothrow_t__nothrow__throw__                                                      76 // void *operator new(std::size_t size, const std::nothrow_t& nothrow) throw()
#define API_ID_void__operator_new___std__size_t_size__const_std__nothrow_t__nothrow__throw__                                                    77 // void *operator new[](std::size_t size, const std::nothrow_t& nothrow) throw()
#define API_ID_void_operator_delete_void__ptr__const_std__nothrow_t__nothrow__throw__                                                           78 // void operator delete(void *ptr, const std::nothrow_t& nothrow) throw()
#define API_ID_void_operator_delete___void__ptr__const_std__nothrow_t__nothrow__throw__                                                         79 // void operator delete[](void *ptr, const std::nothrow_t& nothrow) throw()

#define API_ID_memset                                                                                                                           80 // memset
#define API_ID_memcmp                                                                                                                           81 // memcmp
#define API_ID_memcpy                                                                                                                           82 // memcpy

#define API_ID__dalc_app_create                                                                                                                 83 // _dalc_app_create
#define API_ID__dalc_app_terminate                                                                                                              84 // _dalc_app_terminate
#define API_ID__dalc_app_pause                                                                                                                  85 // _dalc_app_pause
#define API_ID__dalc_app_resume                                                                                                                 86 // _dalc_app_resume
#define API_ID__dalc_app_service                                                                                                                87 // _dalc_app_service

#define API_ID__da_cleanup_handler                                                                                                              88 // _da_cleanup_handler
#define API_ID__da_ThreadProc                                                                                                                   89 // _da_ThreadProc
#define API_ID_pthread_create                                                                                                                   90 // pthread_create
#define API_ID_pthread_join                                                                                                                     91 // pthread_join
#define API_ID_pthread_exit                                                                                                                     92 // pthread_exit
#define API_ID_pthread_cancel                                                                                                                   93 // pthread_cancel
#define API_ID_pthread_detach                                                                                                                   94 // pthread_detach
#define API_ID_pthread_self                                                                                                                     95 // pthread_self
#define API_ID_pthread_equal                                                                                                                    96 // pthread_equal
#define API_ID_pthread_setcancelstate                                                                                                           97 // pthread_setcancelstate
#define API_ID_pthread_setcanceltype                                                                                                            98 // pthread_setcanceltype
#define API_ID_pthread_attr_init                                                                                                                99 // pthread_attr_init
#define API_ID_pthread_attr_destroy                                                                                                             100 // pthread_attr_destroy
#define API_ID_pthread_attr_getdetachstate                                                                                                      101 // pthread_attr_getdetachstate
#define API_ID_pthread_attr_setdetachstate                                                                                                      102 // pthread_attr_setdetachstate
#define API_ID_pthread_attr_getstacksize                                                                                                        103 // pthread_attr_getstacksize
#define API_ID_pthread_attr_setstacksize                                                                                                        104 // pthread_attr_setstacksize
#define API_ID_pthread_attr_getstackaddr                                                                                                        105 // pthread_attr_getstackaddr
#define API_ID_pthread_attr_setstackaddr                                                                                                        106 // pthread_attr_setstackaddr
#define API_ID_pthread_attr_getinheritsched                                                                                                     107 // pthread_attr_getinheritsched
#define API_ID_pthread_attr_setinheritsched                                                                                                     108 // pthread_attr_setinheritsched
#define API_ID_pthread_attr_getschedparam                                                                                                       109 // pthread_attr_getschedparam
#define API_ID_pthread_attr_setschedparam                                                                                                       110 // pthread_attr_setschedparam
#define API_ID_pthread_attr_getschedpolicy                                                                                                      111 // pthread_attr_getschedpolicy
#define API_ID_pthread_attr_setschedpolicy                                                                                                      112 // pthread_attr_setschedpolicy
#define API_ID_pthread_attr_getguardsize                                                                                                        113 // pthread_attr_getguardsize
#define API_ID_pthread_attr_setguardsize                                                                                                        114 // pthread_attr_setguardsize
#define API_ID_pthread_attr_getscope                                                                                                            115 // pthread_attr_getscope
#define API_ID_pthread_attr_setscope                                                                                                            116 // pthread_attr_setscope
#define API_ID_pthread_attr_getstack                                                                                                            117 // pthread_attr_getstack
#define API_ID_pthread_attr_setstack                                                                                                            118 // pthread_attr_setstack

#define API_ID_pthread_mutex_init                                                                                                               119 // pthread_mutex_init
#define API_ID_pthread_mutex_destroy                                                                                                            120 // pthread_mutex_destroy
#define API_ID_real_pthread_mutex_lock                                                                                                          121 // real_pthread_mutex_lock
#define API_ID_pthread_mutex_lock                                                                                                               122 // pthread_mutex_lock
#define API_ID_pthread_mutex_timedlock                                                                                                          123 // pthread_mutex_timedlock
#define API_ID_pthread_mutex_trylock                                                                                                            124 // pthread_mutex_trylock
#define API_ID_real_pthread_mutex_unlock                                                                                                        125 // real_pthread_mutex_unlock
#define API_ID_pthread_mutex_unlock                                                                                                             126 // pthread_mutex_unlock
#define API_ID_pthread_mutexattr_init                                                                                                           127 // pthread_mutexattr_init
#define API_ID_pthread_mutexattr_destroy                                                                                                        128 // pthread_mutexattr_destroy
#define API_ID_pthread_mutexattr_getprioceiling                                                                                                 129 // pthread_mutexattr_getprioceiling
#define API_ID_pthread_mutexattr_setprioceiling                                                                                                 130 // pthread_mutexattr_setprioceiling
#define API_ID_pthread_mutexattr_getprotocol                                                                                                    131 // pthread_mutexattr_getprotocol
#define API_ID_pthread_mutexattr_setprotocol                                                                                                    132 // pthread_mutexattr_setprotocol
#define API_ID_pthread_mutexattr_getpshared                                                                                                     133 // pthread_mutexattr_getpshared
#define API_ID_pthread_mutexattr_setpshared                                                                                                     134 // pthread_mutexattr_setpshared
#define API_ID_pthread_mutexattr_gettype                                                                                                        135 // pthread_mutexattr_gettype
#define API_ID_pthread_mutexattr_settype                                                                                                        136 // pthread_mutexattr_settype
#define API_ID_pthread_cond_init                                                                                                                137 // pthread_cond_init
#define API_ID_pthread_cond_destroy                                                                                                             138 // pthread_cond_destroy
#define API_ID_pthread_cond_wait                                                                                                                139 // pthread_cond_wait
#define API_ID_pthread_cond_timedwait                                                                                                           140 // pthread_cond_timedwait
#define API_ID_pthread_cond_signal                                                                                                              141 // pthread_cond_signal
#define API_ID_pthread_cond_broadcast                                                                                                           142 // pthread_cond_broadcast

#define API_ID_open                                                                                                                             143 // open
#define API_ID_openat                                                                                                                           144 // openat
#define API_ID_creat                                                                                                                            145 // creat
#define API_ID_close                                                                                                                            146 // close
#define API_ID_access                                                                                                                           147 // access
#define API_ID_faccessat                                                                                                                        148 // faccessat
#define API_ID_lseek                                                                                                                            149 // lseek
#define API_ID_fsync                                                                                                                            150 // fsync
#define API_ID_fdatasync                                                                                                                        151 // fdatasync
#define API_ID_truncate                                                                                                                         152 // truncate
#define API_ID_ftruncate                                                                                                                        153 // ftruncate
#define API_ID_mkfifo                                                                                                                           154 // mkfifo
#define API_ID_mkfifoat                                                                                                                         155 // mkfifoat
#define API_ID_mknod                                                                                                                            156 // mknod
#define API_ID_mknodat                                                                                                                          157 // mknodat
#define API_ID_chown                                                                                                                            158 // chown
#define API_ID_fchownat                                                                                                                         159 // fchownat
#define API_ID_fchown                                                                                                                           160 // fchown
#define API_ID_lchown                                                                                                                           161 // lchown
#define API_ID_lockf                                                                                                                            162 // lockf
#define API_ID_chmod                                                                                                                            163 // chmod
#define API_ID_fchmodat                                                                                                                         164 // fchmodat
#define API_ID_fchmod                                                                                                                           165 // fchmod
#define API_ID_pread                                                                                                                            166 // pread
#define API_ID_read                                                                                                                             167 // read
#define API_ID_pwrite                                                                                                                           168 // pwrite
#define API_ID_write                                                                                                                            169 // write
#define API_ID_readv                                                                                                                            170 // readv
#define API_ID_rmdir                                                                                                                            171 // rmdir
#define API_ID_fchdir                                                                                                                           172 // fchdir
#define API_ID_chdir                                                                                                                            173 // chdir
#define API_ID_link                                                                                                                             174 // link
#define API_ID_linkat                                                                                                                           175 // linkat
#define API_ID_unlink                                                                                                                           176 // unlink
#define API_ID_unlinkat                                                                                                                         177 // unlinkat
#define API_ID_symlink                                                                                                                          178 // symlink
#define API_ID_symlinkat                                                                                                                        179 // symlinkat
#define API_ID_readlink                                                                                                                         180 // readlink
#define API_ID_readlinkat                                                                                                                       181 // readlinkat
#define API_ID_mkdir                                                                                                                            182 // mkdir
#define API_ID_mkdirat                                                                                                                          183 // mkdirat
#define API_ID_closedir                                                                                                                         184 // closedir
#define API_ID_fdopendir                                                                                                                        185 // fdopendir
#define API_ID_opendir                                                                                                                          186 // opendir
#define API_ID_readdir                                                                                                                          187 // readdir
#define API_ID_readdir_r                                                                                                                        188 // readdir_r
#define API_ID_rewinddir                                                                                                                        189 // rewinddir
#define API_ID_seekdir                                                                                                                          190 // seekdir
#define API_ID_telldir                                                                                                                          191 // telldir
#define API_ID_fcntl                                                                                                                            192 // fcntl
#define API_ID_dup                                                                                                                              193 // dup
#define API_ID_dup2                                                                                                                             194 // dup2
#define API_ID_fstatat                                                                                                                          195 // fstatat
#define API_ID_lstat                                                                                                                            196 // lstat
#define API_ID_futimens                                                                                                                         197 // futimens
#define API_ID_utimensat                                                                                                                        198 // utimensat
#define API_ID_utimes                                                                                                                           199 // utimes
#define API_ID_utime                                                                                                                            200 // utime

#define API_ID_clearerr                                                                                                                         201 // clearerr
#define API_ID_fclose                                                                                                                           202 // fclose
#define API_ID_fdopen                                                                                                                           203 // fdopen
#define API_ID_feof                                                                                                                             204 // feof
#define API_ID_ferror                                                                                                                           205 // ferror
#define API_ID_fflush                                                                                                                           206 // fflush
#define API_ID_fgetc                                                                                                                            207 // fgetc
#define API_ID_fgetpos                                                                                                                          208 // fgetpos
#define API_ID_fileno                                                                                                                           209 // fileno
#define API_ID_fopen                                                                                                                            210 // fopen
#define API_ID_fprintf                                                                                                                          211 // fprintf
#define API_ID_fputc                                                                                                                            212 // fputc
#define API_ID_fputs                                                                                                                            213 // fputs
#define API_ID_fread                                                                                                                            214 // fread
#define API_ID_freopen                                                                                                                          215 // freopen
#define API_ID_fscanf                                                                                                                           216 // fscanf
#define API_ID_fseek                                                                                                                            217 // fseek
#define API_ID_fsetpos                                                                                                                          218 // fsetpos
#define API_ID_fwrite                                                                                                                           219 // fwrite
#define API_ID_getc                                                                                                                             220 // getc
#define API_ID_getchar                                                                                                                          221 // getchar
#define API_ID_gets                                                                                                                             222 // gets
#define API_ID_int                                                                                                                              223 // int
#define API_ID_perror                                                                                                                           224 // perror
#define API_ID_printf                                                                                                                           225 // printf
#define API_ID_putc                                                                                                                             226 // putc
#define API_ID_putchar                                                                                                                          227 // putchar
#define API_ID_puts                                                                                                                             228 // puts
#define API_ID_remove                                                                                                                           229 // remove
#define API_ID_rename                                                                                                                           230 // rename
#define API_ID_rewind                                                                                                                           231 // rewind
#define API_ID_scanf                                                                                                                            232 // scanf
#define API_ID_setbuf                                                                                                                           233 // setbuf
#define API_ID_setbuffer                                                                                                                        234 // setbuffer
#define API_ID_setlinebuf                                                                                                                       235 // setlinebuf
#define API_ID_setvbuf                                                                                                                          236 // setvbuf
#define API_ID_tmpnam                                                                                                                           237 // tmpnam
#define API_ID_ungetc                                                                                                                           238 // ungetc
#define API_ID_vfprintf                                                                                                                         239 // vfprintf
#define API_ID_vfscanf                                                                                                                          240 // vfscanf

#define API_ID_void_SceneManagerEventListener__OnSceneTransitionCompleted_const_SceneId__previousSceneId__const_SceneId__currentSceneId_        241 // void SceneManagerEventListener::OnSceneTransitionCompleted(const SceneId &previousSceneId, const SceneId &currentSceneId)

#define API_ID_void_GestureEventListener__OnCustomGestureCanceled__TouchGestureDetector__gestureDetector_                                       242 // void GestureEventListener::OnCustomGestureCanceled (TouchGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnCustomGestureChanged__TouchGestureDetector__gestureDetector_                                        243 // void GestureEventListener::OnCustomGestureChanged (TouchGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnCustomGestureFinished__TouchGestureDetector__gestureDetector_                                       244 // void GestureEventListener::OnCustomGestureFinished (TouchGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnCustomGestureStarted__TouchGestureDetector__gestureDetector_                                        245 // void GestureEventListener::OnCustomGestureStarted (TouchGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnFlickGestureCanceled__TouchFlickGestureDetector__gestureDetector_                                   246 // void GestureEventListener::OnFlickGestureCanceled (TouchFlickGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnFlickGestureDetected__TouchFlickGestureDetector__gestureDetector_                                   247 // void GestureEventListener::OnFlickGestureDetected (TouchFlickGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnLongPressGestureCanceled__TouchLongPressGestureDetector__gestureDetector_                           248 // void GestureEventListener::OnLongPressGestureCanceled (TouchLongPressGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnLongPressGestureDetected__TouchLongPressGestureDetector__gestureDetector_                           249 // void GestureEventListener::OnLongPressGestureDetected (TouchLongPressGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnPanningGestureCanceled__TouchPanningGestureDetector__gestureDetector_                               250 // void GestureEventListener::OnPanningGestureCanceled (TouchPanningGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnPanningGestureChanged__TouchPanningGestureDetector__gestureDetector_                                251 // void GestureEventListener::OnPanningGestureChanged (TouchPanningGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnPanningGestureFinished__TouchPanningGestureDetector__gestureDetector_                               252 // void GestureEventListener::OnPanningGestureFinished (TouchPanningGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnPanningGestureStarted__TouchPanningGestureDetector__gestureDetector_                                253 // void GestureEventListener::OnPanningGestureStarted (TouchPanningGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnPinchGestureCanceled__TouchPinchGestureDetector__gestureDetector_                                   254 // void GestureEventListener::OnPinchGestureCanceled (TouchPinchGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnPinchGestureChanged__TouchPinchGestureDetector__gestureDetector_                                    255 // void GestureEventListener::OnPinchGestureChanged (TouchPinchGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnPinchGestureFinished__TouchPinchGestureDetector__gestureDetector_                                   256 // void GestureEventListener::OnPinchGestureFinished (TouchPinchGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnPinchGestureStarted__TouchPinchGestureDetector__gestureDetector_                                    257 // void GestureEventListener::OnPinchGestureStarted (TouchPinchGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnRotationGestureCanceled__TouchRotationGestureDetector__gestureDetector_                             258 // void GestureEventListener::OnRotationGestureCanceled (TouchRotationGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnRotationGestureChanged__TouchRotationGestureDetector__gestureDetector_                              259 // void GestureEventListener::OnRotationGestureChanged (TouchRotationGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnRotationGestureFinished__TouchRotationGestureDetector__gestureDetector_                             260 // void GestureEventListener::OnRotationGestureFinished (TouchRotationGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnRotationGestureStarted__TouchRotationGestureDetector__gestureDetector_                              261 // void GestureEventListener::OnRotationGestureStarted (TouchRotationGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnTapGestureCanceled__TouchTapGestureDetector__gestureDetector_                                       262 // void GestureEventListener::OnTapGestureCanceled (TouchTapGestureDetector &gestureDetector)
#define API_ID_void_GestureEventListener__OnTapGestureDetected__TouchTapGestureDetector__gestureDetector_                                       263 // void GestureEventListener::OnTapGestureDetected (TouchTapGestureDetector &gestureDetector)

#define API_ID_on_orientation_changed                                                                                                           264 // on_orientation_changed

#define API_ID_ecore_event_evas_key_down                                                                                                        265 // ecore_event_evas_key_down
#define API_ID_ecore_event_evas_key_up                                                                                                          266 // ecore_event_evas_key_up
#define API_ID_ecore_event_evas_mouse_button_down                                                                                               267 // ecore_event_evas_mouse_button_down
#define API_ID_ecore_event_evas_mouse_button_up                                                                                                 268 // ecore_event_evas_mouse_button_up
#define API_ID_ecore_event_evas_mouse_move                                                                                                      269 // ecore_event_evas_mouse_move

#define API_ID_accept                                                                                                                           270 // accept
#define API_ID_accept4                                                                                                                          271 // accept4
#define API_ID_bind                                                                                                                             272 // bind
#define API_ID_gai_strerror                                                                                                                     273 // gai_strerror
#define API_ID_hstrerror                                                                                                                        274 // hstrerror
#define API_ID_inet_ntop                                                                                                                        275 // inet_ntop
#define API_ID_connect                                                                                                                          276 // connect
#define API_ID_endhostent                                                                                                                       277 // endhostent
#define API_ID_endnetent                                                                                                                        278 // endnetent
#define API_ID_endprotoent                                                                                                                      279 // endprotoent
#define API_ID_endservent                                                                                                                       280 // endservent
#define API_ID_freeaddrinfo                                                                                                                     281 // freeaddrinfo
#define API_ID_freeifaddrs                                                                                                                      282 // freeifaddrs
#define API_ID_gai_cancel                                                                                                                       283 // gai_cancel
#define API_ID_gai_error                                                                                                                        284 // gai_error
#define API_ID_gai_suspend                                                                                                                      285 // gai_suspend
#define API_ID_getaddrinfo                                                                                                                      286 // getaddrinfo
#define API_ID_getaddrinfo_a                                                                                                                    287 // getaddrinfo_a
#define API_ID_getdomainname                                                                                                                    288 // getdomainname
#define API_ID_gethostbyaddr_r                                                                                                                  289 // gethostbyaddr_r
#define API_ID_gethostbyname2_r                                                                                                                 290 // gethostbyname2_r
#define API_ID_gethostbyname_r                                                                                                                  291 // gethostbyname_r
#define API_ID_gethostent_r                                                                                                                     292 // gethostent_r
#define API_ID_gethostname                                                                                                                      293 // gethostname
#define API_ID_getifaddrs                                                                                                                       294 // getifaddrs
#define API_ID_getnameinfo                                                                                                                      295 // getnameinfo
#define API_ID_getnetbyaddr_r                                                                                                                   296 // getnetbyaddr_r
#define API_ID_getnetbyname_r                                                                                                                   297 // getnetbyname_r
#define API_ID_getnetent_r                                                                                                                      298 // getnetent_r
#define API_ID_getpeername                                                                                                                      299 // getpeername
#define API_ID_getprotobyname_r                                                                                                                 300 // getprotobyname_r
#define API_ID_getprotobynumber_r                                                                                                               301 // getprotobynumber_r
#define API_ID_getprotoent_r                                                                                                                    302 // getprotoent_r
#define API_ID_getservbyname_r                                                                                                                  303 // getservbyname_r
#define API_ID_getservbyport_r                                                                                                                  304 // getservbyport_r
#define API_ID_getservent_r                                                                                                                     305 // getservent_r
#define API_ID_getsockname                                                                                                                      306 // getsockname
#define API_ID_getsockopt                                                                                                                       307 // getsockopt
#define API_ID_herror                                                                                                                           308 // herror
#define API_ID_gethostbyaddr                                                                                                                    309 // gethostbyaddr
#define API_ID_gethostbyname                                                                                                                    310 // gethostbyname
#define API_ID_gethostbyname2                                                                                                                   311 // gethostbyname2
#define API_ID_gethostent                                                                                                                       312 // gethostent
#define API_ID_htonl                                                                                                                            313 // htonl
#define API_ID_htons                                                                                                                            314 // htons
#define API_ID_if_freenameindex                                                                                                                 315 // if_freenameindex
#define API_ID_if_indextoname                                                                                                                   316 // if_indextoname
#define API_ID_if_nameindex                                                                                                                     317 // if_nameindex
#define API_ID_inet_makeaddr                                                                                                                    318 // inet_makeaddr
#define API_ID_inet_addr                                                                                                                        319 // inet_addr
#define API_ID_inet_aton                                                                                                                        320 // inet_aton
#define API_ID_inet_lnaof                                                                                                                       321 // inet_lnaof
#define API_ID_inet_netof                                                                                                                       322 // inet_netof
#define API_ID_inet_network                                                                                                                     323 // inet_network
#define API_ID_inet_ntoa                                                                                                                        324 // inet_ntoa
#define API_ID_inet_pton                                                                                                                        325 // inet_pton
#define API_ID_if_nametoindex                                                                                                                   326 // if_nametoindex
#define API_ID_le32toh                                                                                                                          327 // le32toh
#define API_ID_le64toh                                                                                                                          328 // le64toh
#define API_ID_listen                                                                                                                           329 // listen
#define API_ID_getnetbyaddr                                                                                                                     330 // getnetbyaddr
#define API_ID_getnetbyname                                                                                                                     331 // getnetbyname
#define API_ID_getnetent                                                                                                                        332 // getnetent
#define API_ID_ntohl                                                                                                                            333 // ntohl
#define API_ID_ntohs                                                                                                                            334 // ntohs
#define API_ID_getprotobyname                                                                                                                   335 // getprotobyname
#define API_ID_getprotobynumber                                                                                                                 336 // getprotobynumber
#define API_ID_getprotoent                                                                                                                      337 // getprotoent
#define API_ID_pselect                                                                                                                          338 // pselect
#define API_ID_recv                                                                                                                             339 // recv
#define API_ID_recvfrom                                                                                                                         340 // recvfrom
#define API_ID_recvmsg                                                                                                                          341 // recvmsg
#define API_ID_select                                                                                                                           342 // select
#define API_ID_send                                                                                                                             343 // send
#define API_ID_sendmsg                                                                                                                          344 // sendmsg
#define API_ID_sendto                                                                                                                           345 // sendto
#define API_ID_getservbyname                                                                                                                    346 // getservbyname
#define API_ID_getservbyport                                                                                                                    347 // getservbyport
#define API_ID_getservent                                                                                                                       348 // getservent
#define API_ID_setdomainname                                                                                                                    349 // setdomainname
#define API_ID_sethostent                                                                                                                       350 // sethostent
#define API_ID_sethostname                                                                                                                      351 // sethostname
#define API_ID_setnetent                                                                                                                        352 // setnetent
#define API_ID_setprotoent                                                                                                                      353 // setprotoent
#define API_ID_setservent                                                                                                                       354 // setservent
#define API_ID_setsockopt                                                                                                                       355 // setsockopt
#define API_ID_shutdown                                                                                                                         356 // shutdown
#define API_ID_socket                                                                                                                           357 // socket
#define API_ID_socketpair                                                                                                                       358 // socketpair
#define API_ID_sockatmark                                                                                                                       359 // sockatmark
#define API_ID_isfdtype                                                                                                                         360 // isfdtype
#define API_ID_poll                                                                                                                             361 // poll
#define API_ID_ppoll                                                                                                                            362 // ppoll
#define API_ID_epoll_create                                                                                                                     363 // epoll_create
#define API_ID_epoll_create1                                                                                                                    364 // epoll_create1
#define API_ID_epoll_wait                                                                                                                       365 // epoll_wait
#define API_ID_epoll_pwait                                                                                                                      366 // epoll_pwait
#define API_ID_epoll_ctl                                                                                                                        367 // epoll_ctl

#define API_ID_Thread__Thread__GetCurrentThread_void_                                                                                           368 // Thread* Thread::GetCurrentThread(void)

#define API_ID_tmpfile                                                                                                                          369 // tmpfile
#define API_ID_ftell                                                                                                                            370 // ftell

#define API_ID_captureScreen                                                                                                                    371 // captureScreen
#define API_ID_result_Container__AddControl_Control__control_                                                                                   372 // result Container::AddControl(Control* control)
#define API_ID_result_Container__RemoveControl_Control__control_                                                                                373 // result Container::RemoveControl(Control* control)

#define API_ID_glActiveTexture                                                                                                                  374 // glActiveTexture
#define API_ID_glAttachShader                                                                                                                   375 // glAttachShader
#define API_ID_glBindAttribLocation                                                                                                             376 // glBindAttribLocation
#define API_ID_glBindBuffer                                                                                                                     377 // glBindBuffer
#define API_ID_glBindFramebuffer                                                                                                                378 // glBindFramebuffer
#define API_ID_glBindRenderbuffer                                                                                                               379 // glBindRenderbuffer
#define API_ID_glBindTexture                                                                                                                    380 // glBindTexture
#define API_ID_glBlendColor                                                                                                                     381 // glBlendColor
#define API_ID_glBlendEquation                                                                                                                  382 // glBlendEquation
#define API_ID_glBlendEquationSeparate                                                                                                          383 // glBlendEquationSeparate
#define API_ID_glBlendFunc                                                                                                                      384 // glBlendFunc
#define API_ID_glBlendFuncSeparate                                                                                                              385 // glBlendFuncSeparate
#define API_ID_glBufferData                                                                                                                     386 // glBufferData
#define API_ID_glBufferSubData                                                                                                                  387 // glBufferSubData
#define API_ID_glClear                                                                                                                          388 // glClear
#define API_ID_glClearColor                                                                                                                     389 // glClearColor
#define API_ID_glClearDepthf                                                                                                                    390 // glClearDepthf
#define API_ID_glClearStencil                                                                                                                   391 // glClearStencil
#define API_ID_glColorMask                                                                                                                      392 // glColorMask
#define API_ID_glCompileShader                                                                                                                  393 // glCompileShader
#define API_ID_glCompressedTexImage2D                                                                                                           394 // glCompressedTexImage2D
#define API_ID_glCompressedTexSubImage2D                                                                                                        395 // glCompressedTexSubImage2D
#define API_ID_glCopyTexImage2D                                                                                                                 396 // glCopyTexImage2D
#define API_ID_glCopyTexSubImage2D                                                                                                              397 // glCopyTexSubImage2D
#define API_ID_glCullFace                                                                                                                       398 // glCullFace
#define API_ID_glDeleteBuffers                                                                                                                  399 // glDeleteBuffers
#define API_ID_glDeleteFramebuffers                                                                                                             400 // glDeleteFramebuffers
#define API_ID_glDeleteProgram                                                                                                                  401 // glDeleteProgram
#define API_ID_glDeleteRenderbuffers                                                                                                            402 // glDeleteRenderbuffers
#define API_ID_glDeleteShader                                                                                                                   403 // glDeleteShader
#define API_ID_glDeleteTextures                                                                                                                 404 // glDeleteTextures
#define API_ID_glDepthFunc                                                                                                                      405 // glDepthFunc
#define API_ID_glDepthMask                                                                                                                      406 // glDepthMask
#define API_ID_glDepthRangef                                                                                                                    407 // glDepthRangef
#define API_ID_glDetachShader                                                                                                                   408 // glDetachShader
#define API_ID_glDisable                                                                                                                        409 // glDisable
#define API_ID_glDisableVertexAttribArray                                                                                                       410 // glDisableVertexAttribArray
#define API_ID_glDrawArrays                                                                                                                     411 // glDrawArrays
#define API_ID_glDrawElements                                                                                                                   412 // glDrawElements
#define API_ID_glEnable                                                                                                                         413 // glEnable
#define API_ID_glEnableVertexAttribArray                                                                                                        414 // glEnableVertexAttribArray
#define API_ID_glFinish                                                                                                                         415 // glFinish
#define API_ID_glFlush                                                                                                                          416 // glFlush
#define API_ID_glFramebufferRenderbuffer                                                                                                        417 // glFramebufferRenderbuffer
#define API_ID_glFramebufferTexture2D                                                                                                           418 // glFramebufferTexture2D
#define API_ID_glFrontFace                                                                                                                      419 // glFrontFace
#define API_ID_glGenBuffers                                                                                                                     420 // glGenBuffers
#define API_ID_glGenFramebuffers                                                                                                                421 // glGenFramebuffers
#define API_ID_glGenRenderbuffers                                                                                                               422 // glGenRenderbuffers
#define API_ID_glGenTextures                                                                                                                    423 // glGenTextures
#define API_ID_glGenerateMipmap                                                                                                                 424 // glGenerateMipmap
#define API_ID_glGetBooleanv                                                                                                                    425 // glGetBooleanv
#define API_ID_glGetFloatv                                                                                                                      426 // glGetFloatv
#define API_ID_glGetIntegerv                                                                                                                    427 // glGetIntegerv
#define API_ID_glGetActiveAttrib                                                                                                                428 // glGetActiveAttrib
#define API_ID_glGetActiveUniform                                                                                                               429 // glGetActiveUniform
#define API_ID_glGetAttachedShaders                                                                                                             430 // glGetAttachedShaders
#define API_ID_glGetAttribLocation                                                                                                              431 // glGetAttribLocation
#define API_ID_glGetBufferParameteriv                                                                                                           432 // glGetBufferParameteriv
#define API_ID_glGetFramebufferAttachmentParameteriv                                                                                            433 // glGetFramebufferAttachmentParameteriv
#define API_ID_glGetProgramInfoLog                                                                                                              434 // glGetProgramInfoLog
#define API_ID_glGetProgramiv                                                                                                                   435 // glGetProgramiv
#define API_ID_glGetRenderbufferParameteriv                                                                                                     436 // glGetRenderbufferParameteriv
#define API_ID_glGetShaderInfoLog                                                                                                               437 // glGetShaderInfoLog
#define API_ID_glGetShaderPrecisionFormat                                                                                                       438 // glGetShaderPrecisionFormat
#define API_ID_glGetShaderSource                                                                                                                439 // glGetShaderSource
#define API_ID_glGetShaderiv                                                                                                                    440 // glGetShaderiv
#define API_ID_glGetTexParameterfv                                                                                                              441 // glGetTexParameterfv
#define API_ID_glGetTexParameteriv                                                                                                              442 // glGetTexParameteriv
#define API_ID_glGetUniformfv                                                                                                                   443 // glGetUniformfv
#define API_ID_glGetUniformiv                                                                                                                   444 // glGetUniformiv
#define API_ID_glGetVertexAttribfv                                                                                                              445 // glGetVertexAttribfv
#define API_ID_glGetVertexAttribiv                                                                                                              446 // glGetVertexAttribiv
#define API_ID_glGetVertexAttribPointerv                                                                                                        447 // glGetVertexAttribPointerv
#define API_ID_glHint                                                                                                                           448 // glHint
#define API_ID_glLineWidth                                                                                                                      449 // glLineWidth
#define API_ID_glLinkProgram                                                                                                                    450 // glLinkProgram
#define API_ID_glPixelStorei                                                                                                                    451 // glPixelStorei
#define API_ID_glPolygonOffset                                                                                                                  452 // glPolygonOffset
#define API_ID_glReadPixels                                                                                                                     453 // glReadPixels
#define API_ID_glReleaseShaderCompiler                                                                                                          454 // glReleaseShaderCompiler
#define API_ID_glRenderbufferStorage                                                                                                            455 // glRenderbufferStorage
#define API_ID_glSampleCoverage                                                                                                                 456 // glSampleCoverage
#define API_ID_glScissor                                                                                                                        457 // glScissor
#define API_ID_glShaderBinary                                                                                                                   458 // glShaderBinary
#define API_ID_glShaderSource                                                                                                                   459 // glShaderSource
#define API_ID_glStencilFunc                                                                                                                    460 // glStencilFunc
#define API_ID_glStencilFuncSeparate                                                                                                            461 // glStencilFuncSeparate
#define API_ID_glStencilMask                                                                                                                    462 // glStencilMask
#define API_ID_glStencilMaskSeparate                                                                                                            463 // glStencilMaskSeparate
#define API_ID_glStencilOp                                                                                                                      464 // glStencilOp
#define API_ID_glStencilOpSeparate                                                                                                              465 // glStencilOpSeparate
#define API_ID_glTexImage2D                                                                                                                     466 // glTexImage2D
#define API_ID_glTexParameterf                                                                                                                  467 // glTexParameterf
#define API_ID_glTexParameterfv                                                                                                                 468 // glTexParameterfv
#define API_ID_glTexParameteri                                                                                                                  469 // glTexParameteri
#define API_ID_glTexParameteriv                                                                                                                 470 // glTexParameteriv
#define API_ID_glTexSubImage2D                                                                                                                  471 // glTexSubImage2D
#define API_ID_glUniform1f                                                                                                                      472 // glUniform1f
#define API_ID_glUniform2f                                                                                                                      473 // glUniform2f
#define API_ID_glUniform3f                                                                                                                      474 // glUniform3f
#define API_ID_glUniform4f                                                                                                                      475 // glUniform4f
#define API_ID_glUniform1fv                                                                                                                     476 // glUniform1fv
#define API_ID_glUniform2fv                                                                                                                     477 // glUniform2fv
#define API_ID_glUniform3fv                                                                                                                     478 // glUniform3fv
#define API_ID_glUniform4fv                                                                                                                     479 // glUniform4fv
#define API_ID_glUniform1i                                                                                                                      480 // glUniform1i
#define API_ID_glUniform2i                                                                                                                      481 // glUniform2i
#define API_ID_glUniform3i                                                                                                                      482 // glUniform3i
#define API_ID_glUniform4i                                                                                                                      483 // glUniform4i
#define API_ID_glUniform1iv                                                                                                                     484 // glUniform1iv
#define API_ID_glUniform2iv                                                                                                                     485 // glUniform2iv
#define API_ID_glUniform3iv                                                                                                                     486 // glUniform3iv
#define API_ID_glUniformMatrix2fv                                                                                                               487 // glUniformMatrix2fv
#define API_ID_glUniformMatrix3fv                                                                                                               488 // glUniformMatrix3fv
#define API_ID_glUniformMatrix4fv                                                                                                               489 // glUniformMatrix4fv
#define API_ID_glUniform4iv                                                                                                                     490 // glUniform4iv
#define API_ID_glUseProgram                                                                                                                     491 // glUseProgram
#define API_ID_glValidateProgram                                                                                                                492 // glValidateProgram
#define API_ID_glVertexAttrib1f                                                                                                                 493 // glVertexAttrib1f
#define API_ID_glVertexAttrib2f                                                                                                                 494 // glVertexAttrib2f
#define API_ID_glVertexAttrib3f                                                                                                                 495 // glVertexAttrib3f
#define API_ID_glVertexAttrib4f                                                                                                                 496 // glVertexAttrib4f
#define API_ID_glVertexAttrib1fv                                                                                                                497 // glVertexAttrib1fv
#define API_ID_glVertexAttrib2fv                                                                                                                498 // glVertexAttrib2fv
#define API_ID_glVertexAttrib3fv                                                                                                                499 // glVertexAttrib3fv
#define API_ID_glVertexAttrib4fv                                                                                                                500 // glVertexAttrib4fv
#define API_ID_glVertexAttribPointer                                                                                                            501 // glVertexAttribPointer
#define API_ID_glViewport                                                                                                                       502 // glViewport

#define API_ID_glCheckFramebufferStatus                                                                                                         503 // glCheckFramebufferStatus
#define API_ID_glCreateProgram                                                                                                                  504 // glCreateProgram
#define API_ID_glCreateShader                                                                                                                   505 // glCreateShader
#define API_ID_glGetError                                                                                                                       506 // glGetError
#define API_ID_glGetString                                                                                                                      507 // glGetString
#define API_ID_glGetUniformLocation                                                                                                             508 // glGetUniformLocation
#define API_ID_glIsBuffer                                                                                                                       509 // glIsBuffer
#define API_ID_glIsEnabled                                                                                                                      510 // glIsEnabled
#define API_ID_glIsFramebuffer                                                                                                                  511 // glIsFramebuffer
#define API_ID_glIsProgram                                                                                                                      512 // glIsProgram
#define API_ID_glIsRenderbuffer                                                                                                                 513 // glIsRenderbuffer
#define API_ID_glIsShader                                                                                                                       514 // glIsShader
#define API_ID_glIsTexture                                                                                                                      515 // glIsTexture

#define API_ID__SglSwapBuffers                                                                                                                  516 // _SglSwapBuffers
#define API_ID_eglSwapBuffers                                                                                                                   517 // eglSwapBuffers

#define API_ID__ZN5Tizen3Net7Sockets6Socket10SetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameEi                                        518 // _ZN5Tizen3Net7Sockets6Socket10SetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameEi###Socket::SetSockOpt
#define API_ID__ZN5Tizen3Net7Sockets6Socket10SetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERKNS1_12LingerOptionE                    519 // _ZN5Tizen3Net7Sockets6Socket10SetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERKNS1_12LingerOptionE###Socket::SetSockOpt
#define API_ID__ZN5Tizen3Net7Sockets6Socket17AddSocketListenerERNS1_20ISocketEventListenerE                                                     520 // _ZN5Tizen3Net7Sockets6Socket17AddSocketListenerERNS1_20ISocketEventListenerE###Socket::AddSocketListener
#define API_ID__ZN5Tizen3Net7Sockets6Socket20RemoveSocketListenerERNS1_20ISocketEventListenerE                                                  521 // _ZN5Tizen3Net7Sockets6Socket20RemoveSocketListenerERNS1_20ISocketEventListenerE###Socket::RemoveSocketListener
#define API_ID__ZN5Tizen3Net7Sockets6Socket21AsyncSelectByListenerEm                                                                            522 // _ZN5Tizen3Net7Sockets6Socket21AsyncSelectByListenerEm###Socket::AsyncSelectByListener
#define API_ID__ZN5Tizen3Net7Sockets6Socket4BindERKNS0_11NetEndPointE                                                                           523 // _ZN5Tizen3Net7Sockets6Socket4BindERKNS0_11NetEndPointE###Socket::Bind
#define API_ID__ZN5Tizen3Net7Sockets6Socket4SendERNS_4Base10ByteBufferE                                                                         524 // _ZN5Tizen3Net7Sockets6Socket4SendERNS_4Base10ByteBufferE###Socket::Send
#define API_ID__ZN5Tizen3Net7Sockets6Socket5CloseEv                                                                                             525 // _ZN5Tizen3Net7Sockets6Socket5CloseEv###Socket::Close
#define API_ID__ZN5Tizen3Net7Sockets6Socket6ListenEi                                                                                            526 // _ZN5Tizen3Net7Sockets6Socket6ListenEi###Socket::Listen
#define API_ID__ZN5Tizen3Net7Sockets6Socket6SendToEPviRKNS0_11NetEndPointERi                                                                    527 // _ZN5Tizen3Net7Sockets6Socket6SendToEPviRKNS0_11NetEndPointERi###Socket::SendTo
#define API_ID__ZN5Tizen3Net7Sockets6Socket6SendToERNS_4Base10ByteBufferERKNS0_11NetEndPointE                                                   528 // _ZN5Tizen3Net7Sockets6Socket6SendToERNS_4Base10ByteBufferERKNS0_11NetEndPointE###Socket::SendTo
#define API_ID__ZN5Tizen3Net7Sockets6Socket7ConnectERKNS0_11NetEndPointE                                                                        529 // _ZN5Tizen3Net7Sockets6Socket7ConnectERKNS0_11NetEndPointE###Socket::Connect
#define API_ID__ZN5Tizen3Net7Sockets6Socket9ConstructENS1_22NetSocketAddressFamilyENS1_13NetSocketTypeENS1_17NetSocketProtocolE                 530 // _ZN5Tizen3Net7Sockets6Socket9ConstructENS1_22NetSocketAddressFamilyENS1_13NetSocketTypeENS1_17NetSocketProtocolE###Socket::Construct
#define API_ID__ZN5Tizen3Net7Sockets6Socket9ConstructERKNS0_13NetConnectionENS1_22NetSocketAddressFamilyENS1_13NetSocketTypeENS1_17NetSocketProtocolE 531 // _ZN5Tizen3Net7Sockets6Socket9ConstructERKNS0_13NetConnectionENS1_22NetSocketAddressFamilyENS1_13NetSocketTypeENS1_17NetSocketProtocolE###Socket::Construct
#define API_ID__ZNK5Tizen3Net7Sockets6Socket10GetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERi                                      532 // _ZNK5Tizen3Net7Sockets6Socket10GetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERi###Socket::GetSockOpt
#define API_ID__ZNK5Tizen3Net7Sockets6Socket10GetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERNS1_12LingerOptionE                    533 // _ZNK5Tizen3Net7Sockets6Socket10GetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERNS1_12LingerOptionE###Socket::GetSockOpt
#define API_ID__ZNK5Tizen3Net7Sockets6Socket11ReceiveFromEPviRNS0_11NetEndPointERi                                                              534 // _ZNK5Tizen3Net7Sockets6Socket11ReceiveFromEPviRNS0_11NetEndPointERi###Socket::Receive/Socket::ReceiveFrom
#define API_ID__ZNK5Tizen3Net7Sockets6Socket11ReceiveFromERNS_4Base10ByteBufferERNS0_11NetEndPointE                                             535 // _ZNK5Tizen3Net7Sockets6Socket11ReceiveFromERNS_4Base10ByteBufferERNS0_11NetEndPointE###Socket::ReceiveFrom
#define API_ID__ZNK5Tizen3Net7Sockets6Socket5IoctlENS1_17NetSocketIoctlCmdERm                                                                   536 // _ZNK5Tizen3Net7Sockets6Socket5IoctlENS1_17NetSocketIoctlCmdERm###Socket::Ioctl
#define API_ID__ZNK5Tizen3Net7Sockets6Socket7AcceptNEv                                                                                          537 // _ZNK5Tizen3Net7Sockets6Socket7AcceptNEv###Socket::AcceptN
#define API_ID__ZNK5Tizen3Net7Sockets6Socket7ReceiveERNS_4Base10ByteBufferE                                                                     538 // _ZNK5Tizen3Net7Sockets6Socket7ReceiveERNS_4Base10ByteBufferE###Socket::Receive
#define API_ID__ZZN5Tizen3Net7Sockets6Socket4SendEPviRiE19__PRETTY_FUNCTION__                                                                   539 // _ZZN5Tizen3Net7Sockets6Socket4SendEPviRiE19__PRETTY_FUNCTION__###Socket::Send


#ifdef __cplusplus
}
#endif

#endif /* __API_ID_MAP__ */
