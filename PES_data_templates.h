////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

/*
        DATA TEMPLATES
Contains the template for the 2d vectors used in PES.
Also contains algebraic operations, scalar product, modulus, distance between vectors.

Usually inclued from PES_types.h
*/

#ifndef DATA_TEMPLATES
#define DATA_TEMPLATES

//std includes, to be used throughout the whole library
#include <cmath>
#include <vector>
#include <iostream>
#include <ios>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "SDL_FontCache.h"

#define DEBUG

template <class T> class PES_vector2d_template 
{
 public:
  PES_vector2d_template(T input_x = 0, T input_y = 0) : x(input_x), y(input_y) {}
  T x;
  T y;
};



/////////////////////////////////////////////////////////////////////////
// operations with vectors
// operations cannot be written in the .cpp file

//algebraic operations
template <class T> PES_vector2d_template<T> operator+ 
(const PES_vector2d_template<T>& v1, const PES_vector2d_template<T>& v2)
{
 PES_vector2d_template<T> sum(v1.x + v2.x,v1.y + v2.y) ;
 return sum;
};

template <class T> PES_vector2d_template<T>& operator+= 
(PES_vector2d_template<T>& v1, const PES_vector2d_template<T>& v2)
{
 v1.x += v2.x; v1.y += v2.y;
 return v1;
};


template <class T> PES_vector2d_template<T> operator-
(const PES_vector2d_template<T>& v1, const PES_vector2d_template<T>& v2)
{
 PES_vector2d_template<T> difference(v1.x - v2.x,v1.y - v2.y) ;
 return difference;
};


template <class T, class U> 
PES_vector2d_template<T> operator* (const PES_vector2d_template<T>& v, U c) 
{
 PES_vector2d_template<T> product(v.x*c, v.y*c);
 return product;
}


template <class T, class U> 
PES_vector2d_template<T> operator* (U c,const PES_vector2d_template<T>& v) //inverse order
{
 return v*c;
}


template <class T, class U> 
PES_vector2d_template<T> operator/ (const PES_vector2d_template<T>& v, U c) //float?
{
 return v*(1/static_cast<T>(c));
}

//euclidean distance
template <class T> T
pit_dist (const PES_vector2d_template<T>& v1, const PES_vector2d_template<T>& v2)
{
 return static_cast<T>(v2_modulus(v1 - v2));
}

//scalar product & modulus
template <class T> T operator* (const PES_vector2d_template<T>& v1, const PES_vector2d_template<T>& v2)
{
 return v1.x*v2.x + v1.y*v2.y;
}

template <class T> T v2_modulus(const PES_vector2d_template<T>& v)
{
 return sqrt(v*v);
}


//SDL errors
void
log_SDL_err(std::ostream &os, const std::string &msg);

//creates an USEREVENT SDL_Event and adds it in the queue
void
PES_push_user_event(Uint32 type);


#endif











 
