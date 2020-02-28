/**************************************************************************/
/*!
    @file     comm.cpp
    @author   Arko
    @license  BSD (see license.txt)
    Robocar Communication Interface
    v1.0 - First release
*/
/**************************************************************************/

#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>

#include "comm.hpp"

// Shared Ctrl Variables
Ctrl_Cmd *shared_ctrl_cmd;		// CMD shared memory
Ctrl_Telem *shared_ctrl_telem;	// TELEM shared memory

// Shared Mem and Semaphore Ctrl Variables
int cmd_shmid;				    // CMD shared memory ID
key_t cmd_key = 1000;           // CMD shared memory key
int cmd_sem_id;				    // CMD semaphore ID
key_t cmd_sem_key = 1001;		// CMD semaphore key

int telem_shmid;			    // TELEM shared memory ID
key_t telem_key = 2000;         // TELEM shared memory key
int telem_sem_id;			    // TELEM semaphore ID
key_t telem_sem_key = 2001;		// TELEM semaphore key

/**************************************************************************/
/*!
    @brief  Intialize communication interface
    @return Error code
*/
/**************************************************************************/
int comm_init(void)
{
    // Setup CMD semaphore and shared memory
    if ((cmd_sem_id = semget(cmd_sem_key, 1, IPC_CREAT | 0666)) < 0)
    {
	printf("Error getting cmd semaphore id");
    	return(-1);
    }
    if (semctl(cmd_sem_id, 0, SETVAL, 1) < 0) { printf("CMD SEM UNLOCK FAILED\n"); } // UNLOCK CMD SEM

    if ((cmd_shmid = shmget(cmd_key, sizeof(Ctrl_Cmd), IPC_CREAT | 0666)) < 0)
    {
        printf("Error getting shared memory id");
        return(-1);
    }
    if ((shared_ctrl_cmd = (Ctrl_Cmd *)shmat(cmd_shmid, NULL, 0)) == (Ctrl_Cmd *) -1)
    {
        printf("Error attaching shared memory id");
        return(-1);
    }

    // Setup TELEM semaphore and shared memory
    if ((telem_sem_id = semget(telem_sem_key, 1, IPC_CREAT | 0666)) < 0)
    {
        printf("Error getting telem semaphore id");
        return(-1);
    }
    if (semctl(telem_sem_id, 0, SETVAL, 1) < 0) { printf("TELEM SEM UNLOCK FAILED\n"); } // UNLOCK TELEM SEM

    if ((telem_shmid = shmget(telem_key, sizeof(Ctrl_Telem), IPC_CREAT | 0666)) < 0)
    {
        printf("Error getting shared memory id");
        return(-1);
    }
    if ((shared_ctrl_telem = (Ctrl_Telem *)shmat(telem_shmid, NULL, 0)) == (Ctrl_Telem *) -1)
    {
        printf("Error attaching shared memory id");
        return(-1);
    }

    return 0;
}

/**************************************************************************/
/*!
    @brief  Close communication interface
    @return None
*/
/**************************************************************************/
void comm_close(void)
{
    // Detach and remove shared memory
    shmdt(&cmd_shmid);
    shmctl(cmd_shmid, IPC_RMID, NULL);
    shmdt(&telem_shmid);
    shmctl(telem_shmid, IPC_RMID, NULL);
}

/**************************************************************************/
/*!
    @brief  Communication transaction
    @param  ctrl_cmd   Command struct to read
    @param  ctrl_telem Telemetry struct to send
    @return None
*/
/**************************************************************************/
void comm_prcp_transaction(Ctrl_Cmd *ctrl_cmd, Ctrl_Telem *ctrl_telem)
{
    sem_acquire(telem_sem_id);
    sem_acquire(cmd_sem_id);

    memcpy(ctrl_telem, shared_ctrl_telem, sizeof(struct Ctrl_Telem));
    memcpy(shared_ctrl_cmd, ctrl_cmd, sizeof(struct Ctrl_Cmd));

    sem_release(telem_sem_id);
    sem_release(cmd_sem_id);
}

/**************************************************************************/
/*!
    @brief  Acquire semaphore for shared memory
    @param  id   Semaphore ID
    @return Error code
*/
/**************************************************************************/
int sem_acquire(int id)
{
    int retval;
    struct sembuf operations[1];
    /* Set up the sembuf structure. */
    operations[0].sem_num = 0;  /* index into semaphore array */
    operations[0].sem_op = -1;
    operations[0].sem_flg = 0;  /* wait if semaphore is <=0 */

    retval = semop(id, operations, 1);

    return retval;
}

/**************************************************************************/
/*!
    @brief  Release semaphore for shared memory
    @param  id   Semaphore ID
    @return Error code
*/
/**************************************************************************/
int sem_release(int id)
{
    int retval;
    struct sembuf operations[1];
    /* Set up the sembuf structure. */
    operations[0].sem_num = 0;
    operations[0].sem_op = 1;
    operations[0].sem_flg = 0;

    retval = semop(id, operations, 1);

    return retval;
}
