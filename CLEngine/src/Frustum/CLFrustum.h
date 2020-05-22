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
 * @author Carlos de la Fuente Torres <delafuentetorresc@gmail.com>
 * 
 */
 
 
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
        Visibility IsInside(const glm::vec3& minPoint, const glm::vec3& maxpoint ) const;

		glm::vec4 GetPlane(Plane_enum plane) const{
			return glm::vec4(m_data[plane][A], m_data[plane][B], m_data[plane][C], m_data[plane][D]);
		}
		bool IsOutLeft(const glm::vec3& point) const;
		bool IsOutRight(const glm::vec3& point) const;
		bool IsOutBack(const glm::vec3& point) const;

	private:
        double m_data[6][4];    // 6 planos, con 4 variables (las que forman la ecuacion general del plano)

};

}

