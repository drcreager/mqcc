#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __AUTO_PTR_H__
#define __AUTO_PTR_H__

/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) auto_ptr.h 1.1@(#)
 */
// This template class is used to automatically delete an array
// of type T allocated using new when it goes out of scope.
// This is a simplified version of what is in the new C++
// standard template library (STL). The STL version should be a
// drop-in replacement for this when available.
//
// example:
// foo()
// {
//    auto_ptr<char> x = new char[128];
//    
//	  strcpy(x,"Hello world.");
//
//    cout << x << endl;
// }
// When the auto_ptr x goes out of scope, it automatically calls
// the delete operator to return the memory allocated with new to the
// heap.
//



template <class T> class auto_ptr
{
	T*	m_p;
public:
	auto_ptr(T* t) { m_p = t; }
	operator T*() { return m_p; }
	~auto_ptr() { delete [] m_p; }
};

#endif