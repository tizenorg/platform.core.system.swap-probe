#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>

#include <sys/time.h>

#include <pthread.h>

pthread_mutex_t lock;
pthread_mutex_t lock_tmp;
pthread_mutexattr_t attr;

static int count = 100;

struct data_t {
	int thread_num;
	int count;
};

void *thread(void *data)
{
	int tmp;
	struct data_t *d = (struct data_t *)data;
	int i = d->count * count;

//	printf("tread %d\n", );

	while (--i > 0) {
//		printf("tread #%d (%d)\n", d->thread_num, i);
		pthread_mutex_lock(&lock);

		pthread_mutex_init(&lock_tmp, NULL);

		pthread_mutexattr_init(&attr);

		pthread_mutexattr_getprioceiling(&attr, &tmp);
		pthread_mutexattr_setprioceiling(&attr, tmp);

		pthread_mutexattr_getprotocol(&attr, &tmp);
		pthread_mutexattr_setprotocol(&attr, tmp);

		pthread_mutexattr_getpshared(&attr, &tmp);
		pthread_mutexattr_setpshared(&attr, tmp);

		pthread_mutexattr_gettype(&attr, &tmp);
		pthread_mutexattr_settype(&attr, tmp);

		pthread_mutexattr_destroy(&attr);


		pthread_mutex_destroy(&lock_tmp);
		pthread_mutex_unlock(&lock);
	}
}

void do_test(int cnt)
{
	pthread_t thread1;
	pthread_t thread2;
	int count = cnt;
	struct data_t d1;
	struct data_t d2;

	d1.thread_num = 1;
	d1.count = cnt;

	d2.thread_num = 2;
	d2.count = cnt;


	pthread_mutex_init(&lock, NULL);

	pthread_create(&thread1, NULL, thread, &d1);
	pthread_create(&thread2, NULL, thread, &d2);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&lock);
}

static uint64_t get_time(void)
{
	struct timeval tv = {0, 0};

	gettimeofday(&tv, NULL);

	return (uint64_t)tv.tv_sec*1000*1000 + tv.tv_usec;
}


int save_to_file(int cnt, uint64_t time)
{
	FILE *file;
	char buf[1024];
	int n;

	n = sprintf(buf, "cnt=%d\ntime=%lu\n", cnt, time);

	file = fopen("/tmp/test_out", "wb");
	if (file == NULL)
		return -1;

	fwrite(buf, 1, n, file);
	fclose(file);
}


int main(int argc, char *argv[])
{
	uint64_t time, time_0, time_1;
	int cnt;
	char *str_cnt;

	if (argc != 2)
		return -EINVAL;

	str_cnt = argv[1];
	sscanf(str_cnt, "%d", &cnt);

//	cnt = count;
	printf("cnt=%d\n", cnt);

	time_0 = get_time();

	do_test(cnt);

	time_1 = get_time();

	time = time_1 - time_0;

	printf("time=%lu\n", time);

	save_to_file(cnt, time);

	return 0;
}
