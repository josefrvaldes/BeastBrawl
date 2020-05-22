/**
 * Beast Brawl
 * Game created as a final project of the Multimedia Engineering Degree in the University of Alicante.
 * Made by Clover Games Studio, with members 
 * Carlos de la Fuente Torres delafuentetorresc@gmail.com,
 * Antonio Jose Martinez Garcia https://www.linkedin.com/in/antonio-jose-martinez-garcia/,
 * Jesús Mas Carretero jmasc03@gmail.com, 
 * Judith Mula Molina https://www.linkedin.com/in/judith-mm-18099215a/, 
 * Rubén Rubio Martínez https://www.linkedin.com/in/rub%C3%A9n-rubio-mart%C3%ADnez-938700131/, 
 * and Jose Valdés Sirvent https://www.linkedin.com/in/jose-f-valdés-sirvent-6058b5a5/ github -> josefrvaldes
 * 
 * 
 * @author Clover Games Studio
 * 
 */
 
 
 
#include "CLFrustum.h"
#include <iostream>

using namespace std;
using namespace CLE;

// https://github.com/gametutorials/tutorials/blob/master/OpenGL/Frustum%20Culling/Frustum.cpp
void CLFrustum::Transform(glm::mat4& proj, glm::mat4& view){
     
	double clip[4][4];
	clip[0][0] = view[0][0] * proj[0][0] + view[0][1] * proj[1][0] + view[0][2] * proj[2][0] + view[0][3] * proj[3][0];
	clip[0][1] = view[0][0] * proj[0][1] + view[0][1] * proj[1][1] + view[0][2] * proj[2][1] + view[0][3] * proj[3][1];
	clip[0][2] = view[0][0] * proj[0][2] + view[0][1] * proj[1][2] + view[0][2] * proj[2][2] + view[0][3] * proj[3][2];
	clip[0][3] = view[0][0] * proj[0][3] + view[0][1] * proj[1][3] + view[0][2] * proj[2][3] + view[0][3] * proj[3][3];

	clip[1][0] = view[1][0] * proj[0][0] + view[1][1] * proj[1][0] + view[1][2] * proj[2][0] + view[1][3] * proj[3][0];
	clip[1][1] = view[1][0] * proj[0][1] + view[1][1] * proj[1][1] + view[1][2] * proj[2][1] + view[1][3] * proj[3][1];
	clip[1][2] = view[1][0] * proj[0][2] + view[1][1] * proj[1][2] + view[1][2] * proj[2][2] + view[1][3] * proj[3][2];
	clip[1][3] = view[1][0] * proj[0][3] + view[1][1] * proj[1][3] + view[1][2] * proj[2][3] + view[1][3] * proj[3][3];

	clip[2][0] = view[2][0] * proj[0][0] + view[2][1] * proj[1][0] + view[2][2] * proj[2][0] + view[2][3] * proj[3][0];
	clip[2][1] = view[2][0] * proj[0][1] + view[2][1] * proj[1][1] + view[2][2] * proj[2][1] + view[2][3] * proj[3][1];
	clip[2][2] = view[2][0] * proj[0][2] + view[2][1] * proj[1][2] + view[2][2] * proj[2][2] + view[2][3] * proj[3][2];
	clip[2][3] = view[2][0] * proj[0][3] + view[2][1] * proj[1][3] + view[2][2] * proj[2][3] + view[2][3] * proj[3][3];

	clip[3][0] = view[3][0] * proj[0][0] + view[3][1] * proj[1][0] + view[3][2] * proj[2][0] + view[3][3] * proj[3][0];
	clip[3][1] = view[3][0] * proj[0][1] + view[3][1] * proj[1][1] + view[3][2] * proj[2][1] + view[3][3] * proj[3][1];
	clip[3][2] = view[3][0] * proj[0][2] + view[3][1] * proj[1][2] + view[3][2] * proj[2][2] + view[3][3] * proj[3][2];
	clip[3][3] = view[3][0] * proj[0][3] + view[3][1] * proj[1][3] + view[3][2] * proj[2][3] + view[3][3] * proj[3][3];

	m_data[Right][A] = clip[0][3] - clip[0][0];
	m_data[Right][B] = clip[1][3] - clip[1][0];
	m_data[Right][C] = clip[2][3] - clip[2][0];
	m_data[Right][D] = clip[3][3] - clip[3][0];
	Normalize(Right);

	m_data[Left][A] = clip[0][3] + clip[0][0];
	m_data[Left][B] = clip[1][3] + clip[1][0];
	m_data[Left][C] = clip[2][3] + clip[2][0];
	m_data[Left][D] = clip[3][3] + clip[3][0];
	Normalize(Left);

	m_data[Bottom][A] = clip[0][3] + clip[0][1];
	m_data[Bottom][B] = clip[1][3] + clip[1][1];
	m_data[Bottom][C] = clip[2][3] + clip[2][1];
	m_data[Bottom][D] = clip[3][3] + clip[3][1];
	Normalize(Bottom);

	m_data[Top][A] = clip[0][3] - clip[0][1];
	m_data[Top][B] = clip[1][3] - clip[1][1];
	m_data[Top][C] = clip[2][3] - clip[2][1];
	m_data[Top][D] = clip[3][3] - clip[3][1];
	Normalize(Top);

	m_data[Front][A] = clip[0][3] - clip[0][2];
	m_data[Front][B] = clip[1][3] - clip[1][2];
	m_data[Front][C] = clip[2][3] - clip[2][2];
	m_data[Front][D] = clip[3][3] - clip[3][2];
	Normalize(Front);

	m_data[Back][A] = clip[0][3] + clip[0][2];
	m_data[Back][B] = clip[1][3] + clip[1][2];
	m_data[Back][C] = clip[2][3] + clip[2][2];
	m_data[Back][D] = clip[3][3] + clip[3][2];
	Normalize(Back);

}

void CLFrustum::Normalize(Plane_enum plane)
{
	double magnitude = glm::sqrt(
		m_data[plane][A] * m_data[plane][A] +
		m_data[plane][B] * m_data[plane][B] +
		m_data[plane][C] * m_data[plane][C]
	);

	m_data[plane][A] /= magnitude;
	m_data[plane][B] /= magnitude;
	m_data[plane][C] /= magnitude;
	m_data[plane][D] /= magnitude;
}

CLFrustum::Visibility CLFrustum::IsInside(const glm::vec3& point) const
{
	for (unsigned int i = 0; i < 6; i++) {
		if (m_data[i][A] * point.x +
			m_data[i][B] * point.y +
			m_data[i][C] * point.z +
			m_data[i][D] <= 0) {
			return Invisible;
		}
	}

	return Completly;
}



CLFrustum::Visibility CLFrustum::IsInside( const glm::vec3& point, const float size ) const{
	for(int i = 0; i < 6; i++ )
	{
		if(m_data[i][A] * (point.x - size) + m_data[i][B] * (point.y - size) + m_data[i][C] * (point.z - size) + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * (point.x + size) + m_data[i][B] * (point.y - size) + m_data[i][C] * (point.z - size) + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * (point.x - size) + m_data[i][B] * (point.y + size) + m_data[i][C] * (point.z - size) + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * (point.x + size) + m_data[i][B] * (point.y + size) + m_data[i][C] * (point.z - size) + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * (point.x - size) + m_data[i][B] * (point.y - size) + m_data[i][C] * (point.z + size) + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * (point.x + size) + m_data[i][B] * (point.y - size) + m_data[i][C] * (point.z + size) + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * (point.x - size) + m_data[i][B] * (point.y + size) + m_data[i][C] * (point.z + size) + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * (point.x + size) + m_data[i][B] * (point.y + size) + m_data[i][C] * (point.z + size) + m_data[i][D] > 0)
		   continue;

		// If we get here, it isn't in the frustum
		return Invisible;
	}
	return Completly;
}


CLFrustum::Visibility CLFrustum::IsInside( const glm::vec3& minPoint, const glm::vec3& maxPoint ) const{
	for(int i = 0; i < 6; i++ )
	{
		if(m_data[i][A] * minPoint.x + m_data[i][B] * minPoint.y + m_data[i][C] * -minPoint.z + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * maxPoint.x + m_data[i][B] * minPoint.y + m_data[i][C] * -minPoint.z + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * minPoint.x + m_data[i][B] * maxPoint.y + m_data[i][C] * -minPoint.z + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * maxPoint.x + m_data[i][B] * maxPoint.y + m_data[i][C] * -minPoint.z + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * minPoint.x + m_data[i][B] * minPoint.y + m_data[i][C] * -maxPoint.z + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * maxPoint.x + m_data[i][B] * minPoint.y + m_data[i][C] * -maxPoint.z + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * minPoint.x + m_data[i][B] * maxPoint.y + m_data[i][C] * -maxPoint.z + m_data[i][D] > 0)
		   continue;
		if(m_data[i][A] * maxPoint.x + m_data[i][B] * maxPoint.y + m_data[i][C] * -maxPoint.z + m_data[i][D] > 0)
		   continue;

		// If we get here, it isn't in the frustum
		return Invisible;
	}
	return Completly;
}

// Lo utilizaremos para los indicadores del totem en el hud
bool CLFrustum::IsOutRight(const glm::vec3& point) const
{
	if (m_data[Right][A] * point.x +
		m_data[Right][B] * point.y +
		m_data[Right][C] * (-point.z) +
		m_data[Right][D] <= 0) {
		return true;
	}
	return false;
}

bool CLFrustum::IsOutLeft(const glm::vec3& point) const
{
	if (m_data[Left][A] * point.x +
		m_data[Left][B] * point.y +
		m_data[Left][C] * (-point.z) +
		m_data[Left][D] <= 0) {
		return true;
	}
	return false;
}

bool CLFrustum::IsOutBack(const glm::vec3& point) const
{
	if (m_data[Back][A] * point.x +
		m_data[Back][B] * point.y +
		m_data[Back][C] * (-point.z) +
		m_data[Back][D] <= 0) {
		return true;
	}
	return false;
}




/*
CLFrustum::Visibility CLFrustum::IsInside(const Box3D& box) const
{
	auto GetVisibility = [](const dvec4& clip, const Box3D& box)
	{
		double x0 = box.GetMin().x * clip.x;
		double x1 = box.GetMax().x * clip.x;
		double y0 = box.GetMin().y * clip.y;
		double y1 = box.GetMax().y * clip.y;
		double z0 = box.GetMin().z * clip.z + clip.w;
		double z1 = box.GetMax().z * clip.z + clip.w;
		double p1 = x0 + y0 + z0;
		double p2 = x1 + y0 + z0;
		double p3 = x1 + y1 + z0;
		double p4 = x0 + y1 + z0;
		double p5 = x0 + y0 + z1;
		double p6 = x1 + y0 + z1;
		double p7 = x1 + y1 + z1;
		double p8 = x0 + y1 + z1;

		if (p1 <= 0 && p2 <= 0 && p3 <= 0 && p4 <= 0 && p5 <= 0 && p6 <= 0 && p7 <= 0 && p8 <= 0) {
			return Invisible;
		}
		if (p1 > 0 && p2 > 0 && p3 > 0 && p4 > 0 && p5 > 0 && p6 > 0 && p7 > 0 && p8 > 0) {
			return Completly;
		}

		return Partially;
	};

	Visibility v0 = GetVisibility(GetPlane(Right), box);
	if (v0 == Invisible) {
		return Invisible;
	}

	Visibility v1 = GetVisibility(GetPlane(Left), box);
	if (v1 == Invisible) {
		return Invisible;
	}

	Visibility v2 = GetVisibility(GetPlane(Bottom), box);
	if (v2 == Invisible) {
		return Invisible;
	}

	Visibility v3 = GetVisibility(GetPlane(Top), box);
	if (v3 == Invisible) {
		return Invisible;
	}

	Visibility v4 = GetVisibility(GetPlane(Front), box);
	if (v4 == Invisible) {
		return Invisible;
	}

	if (v0 == Completly && v1 == Completly &&
		v2 == Completly && v3 == Completly &&
		v4 == Completly)
	{
		return Completly;
	}

	return Partially;
}*/


























/*
//#include "PlaneCamera.h"

CLFrustum::CLFrustum(){
    planesCam.reserve(6);
    fnear.reserve(4);
    ffar.reserve(4);
}

int CLFrustum::Intersects(glm::vec3& point) {
    for(int i=0; i < 6; i++) {
        if (planesCam[i].Distance(point) > 0)
            return(0);
    }
    return(1);
}

int CLCLFrustum::Intersects(BoundingBox &box) {
    for(int p = 0; p < 6; p++) {
        if(planes[p].Distance(box.points[0]) < 0 )
            continue;
        if(planes[p].Distance(box.points[1]) < 0 )
            continue;
        if(planes[p].Distance(box.points[2]) < 0 )
            continue;
        if(planes[p].Distance(box.points[3]) < 0 )
            continue;
        if(planes[p].Distance(box.points[4]) < 0 )
            continue;
        if(planes[p].Distance(box.points[5]) < 0 )
            continue;
        if(planes[p].Distance(box.points[6]) < 0 )
            continue;
        if(planes[p].Distance(box.points[7]) < 0 )
            continue;
        return(0);
    }
    return(1);
}
*/