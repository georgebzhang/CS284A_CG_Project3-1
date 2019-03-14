#include "triangle.h"

#include "CGL/CGL.h"
#include "GL/glew.h"

namespace CGL { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {

  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  BBox bb(p1);
  bb.expand(p2); 
  bb.expand(p3);
  return bb;

}

bool Triangle::intersect(const Ray& r) const {
	// TODO (Part 1.3):
	// implement ray-triangle intersection
	return intersect(r, nullptr);
}

bool Triangle::intersect(const Ray& r, Intersection *isect) const {
	// TODO (Part 1.3):
	// implement ray-triangle intersection. When an intersection takes
	// place, the Intersection data should be updated accordingly
	// I must credit the source provided in the HW description: http://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
	Vector3D A = mesh->positions[v1];
	Vector3D B = mesh->positions[v2];
	Vector3D C = mesh->positions[v3];
	Vector3D T = r.o - A;
	Vector3D E1 = B - A;
	Vector3D E2 = C - A;
	Vector3D P = cross(r.d, E2);
	Vector3D Q = cross(T, E1);

	double det = dot(P, E1);

	double u = dot(P, T) / det;
	if (u < 0 || u > 1) return false;

	double v = dot(Q, r.d) / det;
	if (v < 0 || u + v > 1) return false;

	double t = dot(Q, E2) / det;
	if (t < r.min_t || t > r.max_t) return false;
	r.max_t = t;

	if (isect != nullptr) {
		isect->t = t;
		isect->n = (1.0 - u - v) * mesh->normals[v1] + u * mesh->normals[v2] + v * mesh->normals[v3];
		isect->primitive = this;
		isect->bsdf = this->get_bsdf();
	}

	return true;
}

void Triangle::draw(const Color& c, float alpha) const {
  glColor4f(c.r, c.g, c.b, alpha);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c, float alpha) const {
  glColor4f(c.r, c.g, c.b, alpha);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

} // namespace StaticScene
} // namespace CGL
