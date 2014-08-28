#Async

##Purpose

Async is a module for asynchronous operations designed to use in games. My purpose was to create a custom thread pool that will allow to enqueue tasks to SEPARATE threads. E.g. you can enqueue resource loading tasks to thread 1, network tasks to thread 2 and etc.

This is extremely important in mobile apps due to limited processor resources. We can't create lot of threads and priority of the tasks is not enough, because there can be situtation when all workers are busy with resource loading tasks and you want to do a network query as soon as possible, but worker won't do highest priority task while it is doing other job and you have to wait.

Separating tasks by concrete threads solves this problem. Network tasks that run in thread 1 won't block other tasks running in thread 2
