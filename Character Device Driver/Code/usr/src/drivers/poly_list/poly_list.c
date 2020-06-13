#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <minix/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/ds.h>
//#include <stdbool. h>
//#include "hello.h"

/*
 * Function prototypes for the hello driver.
 */

#define HELLO_MESSAGE "HELLO !\n"

static char list[50][2];
static int max = 0;		// Tail of Stack/queue
static int stack = 0;	// Variable to check if it's a stack/queue

void push(char n[]);
void enqueue(char n[]);
char* pop();
char* dequeue();


static int poly_list_open(message *m);
static int poly_list_close(message *m);
static struct device * poly_list_prepare(dev_t device);
static int poly_list_ioctl(message *m_ptr);
static int poly_list_transfer(endpoint_t endpt, int opcode, u64_t position,
	iovec_t *iov, unsigned int nr_req, endpoint_t user_endpt, unsigned int
	flags);

/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init(int type, sef_init_info_t *info);
static int sef_cb_lu_state_save(int);
static int lu_state_restore(void);

/* Entry points to the hello driver. */
static struct chardriver poly_list_tab =
{
	poly_list_open,
	poly_list_close,
    poly_list_ioctl,
    poly_list_prepare,
    poly_list_transfer,
    nop_cleanup,
    nop_alarm,
    nop_cancel,
    nop_select,
    NULL
};

/** Represents the /dev/hello device. */
static struct device poly_list_device;

/** State variable to count the number of times the device has been opened. */
static int open_counter;

static int poly_list_open(message *UNUSED(m))
{
    //printf("poly_open(). Called %d time(s).\n", ++open_counter);
	stack = 0;
    ++open_counter;
    return OK;
}

static int poly_list_close(message *UNUSED(m))
{
    //printf("poly_close()\n");
    return OK;
}

static struct device * poly_list_prepare(dev_t UNUSED(dev))
{
	poly_list_device.dv_base = make64(0, 0);
	poly_list_device.dv_size = make64(strlen(HELLO_MESSAGE), 0);
    return &poly_list_device;
}

static int poly_list_ioctl(message *m_ptr){

    switch(m_ptr->COUNT) {
        case 1:
            if(max != 0)	// Queue not empty
            	printf("Can't convert to stack, queue not empty !\n");
            else
            {
            	stack = 1;
            	printf("Converted to Stack !\n");
            }
            break;

        case 0:
        	if(max != 0)	// Stack not empty
				printf("Can't convert to queue, stack not empty !\n");
			else
			{
				stack = 0;
				printf("Converted to Queue !\n");
			}
            break;

        default:
            printf("Not a valid request !");
            break;
    }
    return 0;
}

static int poly_list_transfer(endpoint_t endpt, int opcode, u64_t position,
    iovec_t *iov, unsigned nr_req, endpoint_t UNUSED(user_endpt),
    unsigned int UNUSED(flags))
{

    int ret;
    char writeChar[] = "6";

    if (nr_req != 1)
    {
        /* This should never trigger for character drivers at the moment. */
        printf("POLY: vectored transfer request, using first element only\n");
    }


    switch (opcode)
    {
        case DEV_GATHER_S:

        	if(max == 0)	// Underflow
        	{
        		printf("List is empty!\n");
        		return 0;	// Return error code
        	}
        	else
        	{
        		char readChar[] = "6";

        		if(stack == 0)			// List is currently a queue
        			strcpy(readChar, dequeue());
        		else					// List is currently a stack
        			strcpy(readChar, pop());

        		ret = sys_safecopyto(endpt, (cp_grant_id_t) iov->iov_addr, 0,
        				(vir_bytes)(readChar),
                                 sizeof(readChar));
        		iov->iov_size -= sizeof(readChar);

        		return 1;	// Return success code
        	}

            break;

        case DEV_SCATTER_S:

			ret = sys_safecopyfrom(endpt, (cp_grant_id_t) iov->iov_addr, 0,
					(vir_bytes)(writeChar), sizeof(writeChar));

			if(stack == 0)				// List is currently a queue
				enqueue(writeChar);
			else						// List is currently a stack
				push(writeChar);

			break;

        default:
            return EINVAL;
    }
    return ret;
}

static int sef_cb_lu_state_save(int UNUSED(state)) {
/* Save the state. */
    ds_publish_u32("open_counter", open_counter, DSF_OVERWRITE);

    return OK;
}

static int lu_state_restore() {
/* Restore the state. */
    u32_t value;

    ds_retrieve_u32("open_counter", &value);
    ds_delete_u32("open_counter");
    open_counter = (int) value;

    return OK;
}

static void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
/* Initialize the hello driver. */
    int do_announce_driver = TRUE;

    open_counter = 0;
    switch(type) {
        case SEF_INIT_FRESH:
            printf("%s", HELLO_MESSAGE);
        break;

        case SEF_INIT_LU:
            /* Restore the state. */
            lu_state_restore();
            do_announce_driver = FALSE;

            printf("Hey, I'm a new version!\n");
        break;

        case SEF_INIT_RESTART:
            printf("Hey, I've just been restarted!\n");
        break;
    }

    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        chardriver_announce();
    }

    /* Initialization completed successfully. */
    return OK;
}

int main(void)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    chardriver_task(&poly_list_tab, CHARDRIVER_SYNC);
    return OK;
}

void push(char n[])
{
	if(max == 50)	// Overflow
		printf("Cannot add element. Limit reached!\n");
	else
	{
		strcpy(list[max],n);
		printf("Element added is : %s\n", list[max]);
		max++;
	}
}

void enqueue(char n[])
{
	if(max == 50)	// Overflow
			printf("Cannot add element. Limit reached!\n");
		else
		{
			strcpy(list[max],n);
			printf("Element added is : %s\n", list[max]);
			max++;
		}
}

char* pop()
{
	max--;
	char temp[] = "";
	strcpy(temp, list[max]);
	return temp;
}

char* dequeue()
{
	char temp[] = "d";
	strcpy(temp, list[0]);

	for(int i = 0; i < max-1; i++)
	{
		strcpy(list[i], list[i+1]);
	}
	max--;

	return temp;
}
