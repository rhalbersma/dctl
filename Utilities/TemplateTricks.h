#pragma once
#include "IntegerTypes.h"

//+------------------------------------------------------------------------------------------------------+
//|      Trick by Andrei Alexandrescu, Dr. Dobb's, October 01, 2001                                      |
//|      http://www.drdobbs.com/cpp/184403750?_requestid=260625                                          |
//|                                                                                                      |
//|      Int2Type "generates" a distinct type for each distinct constant integral value passed.          |
//|      This is because different template instantiations are distinct types,                           |
//|      so Int2Type<0> is different from Int2Type<1> and so on.                                         |
//|                                                                                                      |
//|      This trick is used in so-called "tag dispatching", a.k.a. partial specialization of             |
//|      function templates. The universal move generator makes extensive use of this technique.         |
//+------------------------------------------------------------------------------------------------------+

template<size_t> struct Int2Type {};
