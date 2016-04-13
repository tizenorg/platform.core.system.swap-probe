#ifndef __DAFORKEXEC_PROBES_H__
#define __DAFORKEXEC_PROBES_H__

int PROBE_NAME(execl)(const char *path, const char *arg, ...);
int PROBE_NAME(execlp)(const char *file, const char *arg, ...);
int PROBE_NAME(execle)(const char *path, const char *arg, ...);
int PROBE_NAME(execv)(const char *path, char *const argv[]);
int PROBE_NAME(execvp)(const char *file, char *const argv[]);
int PROBE_NAME(execve)(const char *filename, char *const argv[],char *const envp[]);
int PROBE_NAME(execvpe)(const char *file, char *const argv[],char *const envp[]);
pid_t PROBE_NAME(fork)(void);

#endif /* __DAFORKEXEC_PROBES_H__ */
