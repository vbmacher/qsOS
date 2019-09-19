/*
 * DEFS.H
 *
 * (c) Copyright 2001, P. Jakubco ml.
 *
 */

#ifndef __DEFS__
#define __DEFS__

#if !defined(__COMPACT__)
#	if defined(__TINY__)
#		error Pamatovy model TINY nie je podporovany, pouzite pamatovy model COMPACT.
#	elif defined(__SMALL__)
#		error Pamatovy model SMALL nie je podporovany, pouzite pamatovy model COMPACT.
#	elif defined(__MEDIUM__)
#		error Pamatovy model MEDIUM nie je podporovany, pouzite pamatovy model COMPACT.
#	elif defined(__LARGE__)
#		error Pamatovy model LARGE nie je podporovany, pouzite pamatovy model COMPACT.
#	elif defined(__HUGE__)
#		error Pamatovy model HUGE nie je podporovany, pouzite pamatovy model COMPACT.
#	endif
#endif

#ifndef NULL
#	define NULL 0L
#endif

#ifndef TRUE
#	define TRUE (1 == 1)
#endif

#ifndef FALSE
#	define FALSE (1 == 0)
#endif

#ifndef _extern
#	define _extern extern "C"
#endif

#endif