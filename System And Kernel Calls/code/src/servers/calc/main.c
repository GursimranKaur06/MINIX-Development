/*
 * Calc server
 */

#include "inc.h"
#include <minix/endpoint.h>

/* Allocate space for the global variables. */
static endpoint_t who_e;	/* caller's proc number */
static int callnr;		/* system call number */

/* Declare some local functions. */
static void get_work(message *m_ptr);
static void reply(endpoint_t whom, message *m_ptr);

/* SEF functions and variables. */
static void sef_local_startup(void);

/*===========================================================================*
 *                                    main                                   *
 *===========================================================================*/

int main(int argc, char **argv)
{
    message m;
    int result;
    int err;

    /* Setup for SEF */
    env_setargs(argc, argv);
    sef_local_startup();

    while (TRUE) {
        get_work(&m);

        if (is_notify(callnr)) {
            printf("Calc Server: Got illegal notify from: %d\n", m.m_source);
            result = EINVAL;
            goto send_reply;
        }

        switch (callnr) {
            case ADD:
                result = do_add(&m);
                break;
            case MULTIPLY:
                if ((err = do_multiply(&m))) {
                    printf("Calc Server: Multiply kernel call failed with code %d.\n", err);
                }
                result = m.CALC_MULTIPLY_PRODUCT;
                break;
            default:
                printf("Calc Server: Received illegal request from %d\n", m.m_source);
                break;
        }

send_reply:
        /* Finally send reply message, unless disabled. */
        if (result != EDONTREPLY) {
            m.m_type = result;
            reply(who_e, &m);
        }
    }

    return(OK);
}



/*===========================================================================*
 *			       sef_local_startup			     *
 *===========================================================================*/
static void sef_local_startup()
{
    /* Register init callbacks. */

    /* No live update support for now. */

    /* Let SEF perform startup. */
    sef_startup();
}

/*===========================================================================*
 *				get_work                                     *
 *===========================================================================*/
static void get_work(
        message *m_ptr			/* message buffer */
)
{
    int status = sef_receive(ANY, m_ptr);   /* blocks until message arrives */
    if (OK != status)
        panic("failed to receive message!: %d", status);
    who_e = m_ptr->m_source;        /* message arrived! set sender */
    callnr = m_ptr->m_type;       /* set function call number */
}

/*===========================================================================*
 *				reply					     *
 *===========================================================================*/
static void reply(
        endpoint_t who_e,			/* destination */
        message *m_ptr			/* message buffer */
)
{
    int s = send(who_e, m_ptr);    /* send the message */
    if (OK != s)
        printf("DS: unable to send reply to %d: %d\n", who_e, s);
}
