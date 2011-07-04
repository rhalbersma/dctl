#pragma once

// Trick by Andrei Alexandrescu, Dr. Dobb's, October 01, 2001
// http://www.drdobbs.com/cpp/184403750?_requestid=260625
//
// Int2Type "generates" a distinct type for each distinct constant integral value passed.
// This is because different template instantiations are distinct types,
// so Int2Type<0> is different from Int2Type<1> and so on.
//
// This "tag dispatching" is used to simulate partial specialization of function templates.

template<int> struct Int2Type {};
