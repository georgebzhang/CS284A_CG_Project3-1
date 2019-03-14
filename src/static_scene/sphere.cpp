#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CGL { namespace StaticScene {

bool Sphere::test(const Ray& r, double& t1, double& t2) const {
	// TODO (Part 1.4):
	// Implement ray - sphere intersection test.
	// Return true if there are intersections and writing the
	// smaller of the two intersection times in t1 and the larger in t2.
	return false;
}

bool Sphere::intersect(const Ray& r) const {
	// TODO (Part 1.4):
	// Implement ray - sphere intersection.
	// Note that you might want to use the the Sphere::test helper here.
	return intersect(r, nullptr);
}

bool Sphere::intersect(const Ray& r, Intersection *i) const {
	// TODO (Part 1.4):
	// Implement ray - sphere intersection.
	// Note again that you might want to use the the Sphere::test helper here.
	// When an intersection takes place, the Intersection data should be updated
	// correspondingly.
	double a = dot(r.d, r.d);
	Vector3D sphere_to_cam = r.o - o;
	double b = 2.0 * dot(sphere_to_cam, r.d);
	double c = dot(sphere_to_cam, sphere_to_cam) - r2;

	if (b * b - 4.0 * a * c < 0) return false;

	double t1 = (- b + sqrt(b * b - 4.0 * a * c)) / (2 * a);
	double t2 = (- b - sqrt(b * b - 4.0 * a * c)) / (2 * a);
	double t_min = min(t1, t2);

	if (t_min < r.min_t || t_min > r.max_t) return false;

	r.max_t = t_min;

	Vector3D normal = (r.o + t2 * r.d) - o;
	normal.normalize();

	if (i != nullptr) {
		i->t = t_min;
		i->n = normal;
		i->primitive = this;
		i->bsdf = this->get_bsdf();
	}

	return true;
}

void Sphere::draw(const Color& c, float alpha) const {
  Misc::draw_sphere_opengl(o, r, c);
}

void Sphere::drawOutline(const Color& c, float alpha) const {
    //Misc::draw_sphere_opengl(o, r, c);
}

} // namespace StaticScene
} // namespace CGL
