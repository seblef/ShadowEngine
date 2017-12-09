
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

using namespace Core;

Vector2 Vector2::NullVector(0,0);
Vector2 Vector2::XAxisVector(1,0);
Vector2 Vector2::YAxisVector(0,1);
Vector2 Vector2::UnitVector(1,1);

Vector3 Vector3::NullVector(0,0,0);
Vector3 Vector3::XAxisVector(1,0,0);
Vector3 Vector3::YAxisVector(0,1,0);
Vector3 Vector3::ZAxisVector(0,0,1);
Vector3 Vector3::UnitVector(1,1,1);

Vector4 Vector4::NullVector(0,0,0,0);
Vector4 Vector4::XAxisVector(1,0,0,0);
Vector4 Vector4::YAxisVector(0,1,0,0);
Vector4 Vector4::ZAxisVector(0,0,1,0);
Vector4 Vector4::UnitVector(1,1,1,1);

Vector4 Vector4::White(1,1,1,1);
Vector4 Vector4::Black(0,0,0,1);
Vector4 Vector4::Red(1,0,0,1);
Vector4 Vector4::Green(0,1,0,1);
Vector4 Vector4::Blue(0,0,1,1);
