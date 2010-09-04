#ifndef VERTICE_H_
#define VERTICE_H_

#include <math.h>

class Vertice {
	
	public:
		float x;
		float y;
		float z;

		Vertice() {
			x = 0.0; y = 0.0; z= 0.0;
		};

		Vertice(float a, float b, float c) {
			x = a; y = b; z= c;
		};

		Vertice (const Vertice &v) {
			*this = v;
		}

		bool operator==(const Vertice& v) const {
			return  ((x == v.x) && (y == v.y) && (z == v.z));
		};

		void operator=(const Vertice& v) {
			  x = v.x; y = v.y; z = v.z;
		};

		Vertice operator+(const Vertice& v) const {
			Vertice q(x + v.x, y + v.y, z + v.z);
			return q;
		};

		Vertice operator-(const Vertice& v) const {
			Vertice q(x - v.x, y - v.y, z - v.z);
			return q;
		};

		Vertice operator*(const float d) const {
			Vertice q(x*d, y*d, z*d);
			return q;
		};

		void operator-=(const Vertice& v) {
			  x -= v.x; y -= v.y; z -= v.z;
		};

		void operator+=(const Vertice& v) {
			  x += v.x; y += v.y; z += v.z;
		};

		void operator/=(const float a) {
			  x /= a; y /= a; z /= a;
		};

		void operator*=(const float a) {
			  x *= a; y *= a; z *= a;
		};

		bool esIgual(const float a, const float b, const float c) const {
			return  ((x == a) && (y == b) && (z == c));
		};

		float modulo() const {
			return sqrt(z*z + y*y + x*x);
		}

		void normalizar() {
			(*this) /= this->modulo();
		}

		Vertice proyeccionYZ() const {
			Vertice q(0,y,z);
			return q;
		}

		Vertice prodVectorial(const Vertice &v) const {
			Vertice q(y*v.z - z*v.y , z*v.x - x*v.z, x*v.y - y*v.x);
			return q;
		}


};

#endif /*VERTICE_H_*/
