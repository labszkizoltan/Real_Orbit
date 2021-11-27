
#include "OctTree.h"

bool Box3D::ContainsPoint(const Vec3D& point)
{
    Vec3D dx = point - center;
    return (abs(dx.x) <= radius.x) && (abs(dx.y) <= radius.y) && (abs(dx.z) <= radius.z);
}

bool Box3D::IntersectsBox3D(const Box3D& other)
{
    Vec3D dx = other.center - center;
    return (abs(dx.x) < (radius.x+other.radius.x)) && (abs(dx.y) < (radius.y + other.radius.y)) && (abs(dx.z) < (radius.z + other.radius.z));
}
