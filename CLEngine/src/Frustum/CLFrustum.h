#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace glm;
using namespace std;

namespace CLE{

class CLFrustum{

	public:
		//ENUMS:
		enum Plane_enum { Right, Left, Bottom, Top, Front, Back };
		enum { A, B, C, D };
		enum Visibility { Completly, Partially, Invisible };

		CLFrustum(){};
		virtual ~CLFrustum(){};

		// SERVICES:
		void Transform(glm::mat4& proj, glm::mat4& view);
		void Normalize(Plane_enum plane);

		// CULLING:
		Visibility IsInside(const glm::vec3& point) const;
		//Visibility IsInside(const Box3D& box) const;
        Visibility IsInside(const glm::vec3& point, const float size ) const;

        glm::vec4 GetPlane(Plane_enum plane) const {
        	return glm::vec4(m_data[plane][A], m_data[plane][B], m_data[plane][C], m_data[plane][D]);
        }

	private:
        double m_data[6][4];    // 6 planos, con 4 variables (las que forman la ecuacion general del plano)

};

}

/*
namespace CLE{
    class CLFrustum {
       public:
        CLFrustum();
        ~CLFrustum(){};
        //Check a single point
        int Intersects(glm::vec3& point);
        //Check a bounding box
        //int Intersects(BoundingBox& box);
        enum {
            COORD_BOTTOMLEFT = 0,COORD_BOTTOMRIGHT,COORD_TOPLEFT,COORD_TOPRIGHT
        };
        enum {
            PLANE_LEFT = 0,PLANE_RIGHT,PLANE_BOTTOM,PLANE_TOPPLANE_NEAR,PLANE_FAR
        };
        //Defines our near and far vertices
        vector<glm::vec3> fnear;    // seran 4
        vector<glm::vec3> ffar;     // seran 4
        //The center of the frustum (useful for extents/CSM)
        glm::vec3 center;
        //Plane planes[6];
        vector<PlaneCamera> planesCam;  // seran 6
    };
}*/


