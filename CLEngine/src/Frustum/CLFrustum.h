#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

using namespace glm;
using namespace std;

namespace CLE{
//! Clase para el frustrum
//! Esta clase es utilizada para saber cuando le objeto se encuentra dentro o fuera del campo de visión de la camara
//! @see CLE::CLNode::DFSTree(glm::mat4)
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

