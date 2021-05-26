#pragma once

struct semaphore
{
	int mutex = 0;
	int n_waiting = 0;
	semaphore(int m)
	{
		mutex = m;
	}
};

void wait(semaphore* s)
{
	while (s->mutex == 0);
	s->mutex--;
}

void signal(semaphore* s)
{
	s->mutex++;
}