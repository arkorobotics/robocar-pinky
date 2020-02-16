import sysv_ipc

#cmd_key = 1000                   # CMD shared memory key
#cmd_sem_key = 1001               # CMD semaphore key

telem_key = 2000                 # TELEM shared memory key
telem_sem_key = 2001             # TELEM semaphore key

# Create shared memory object
#cmd_memory = sysv_ipc.SharedMemory(1000)
#cmd_sem = sysv_ipc.Semaphore(1001)

telem_memory = sysv_ipc.SharedMemory(2000)
telem_sem = sysv_ipc.Semaphore(2001)

# Read value from shared memory
while(1):
	telem_sem.acquire()
	#cmd_sem.acquire()

	telem_data = telem_memory.read()
	#cmd_data = cmd_memory.read()

	telem_sem.release()
	#cmd_sem.release()

	print(telem_data)
	#print(cmd_data)
