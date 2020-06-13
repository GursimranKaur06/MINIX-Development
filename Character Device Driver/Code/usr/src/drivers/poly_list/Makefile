# Makefile for the poly_list_world driver.
PROG=   poly_list
SRCS=   poly_list.c

FILES=${PROG}.conf
FILESNAME=${PROG}
FILESDIR= /etc/system.conf.d

DPADD+= ${LIBCHARDRIVER} ${LIBSYS}
LDADD+= -lchardriver -lsys

MAN=

.include <minix.service.mk>