#pragma once

#include <cmath>
#include <iostream>
#include "./vec3.h"

namespace hive
{
namespace math
{
template <class T>
class tvec4
{
public:
	T x = 0;
	T y = 0;
	T z = 0;
	T w = 0;

public:
	tvec4(){};
	~tvec4(){};
};

typedef tvec4<float> vec4;
typedef tvec4<float> vec4f;
typedef tvec4<double> vec4d;
typedef tvec4<int> vec4i;
typedef tvec4<unsigned int> vec4u;
} // namespace math
} // namespace hive
