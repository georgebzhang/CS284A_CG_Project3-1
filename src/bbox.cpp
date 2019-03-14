#include "bbox.h"

#include "GL/glew.h"

#include <algorithm>
#include <iostream>

namespace CGL {

void swap(double& d1, double& d2) {
	double temp = d1;
	d1 = d2;
	d2 = temp;
}

bool BBox::intersect(const Ray& r, double& t0, double& t1) const {
	// TODO (Part 2.2):
	// Implement ray - bounding box intersection test
	// If the ray intersected the bouding box within the range given by
	// t0, t1, update t0 and t1 with the new intersection times.
	// I must credit the source: http://www.cs.utah.edu/~awilliam/box/box.pdf
	double t_max, t_min, t_max_x, t_min_x, t_max_y, t_min_y, t_max_z, t_min_z;
	t_max_x = (max.x - r.o.x) / r.d.x;
	t_min_x = (min.x - r.o.x) / r.d.x;
	if (r.d.x < 0) swap(t_max_x, t_min_x);
	t_min = t_min_x;
	t_max = t_max_x;

	t_max_y = (max.y - r.o.y) / r.d.y;
	t_min_y = (min.y - r.o.y) / r.d.y;
	if (r.d.y < 0) swap(t_max_y, t_min_y);

	if (t_min_x > t_max_y || t_min_y > t_max_x) return false;
	if (t_min_y > t_min_x) t_min = t_min_y;
	if (t_max_y < t_max_x) t_max = t_max_y;

	t_max_z = (max.z - r.o.z) / r.d.z;
	t_min_z = (min.z - r.o.z) / r.d.z;
	if (r.d.z < 0) swap(t_max_z, t_min_z);

	if (t_min > t_max_z || t_min_z > t_max) return false;
	if (t_min_z > t_min) t_min = t_min_z;
	if (t_max_z < t_max) t_max = t_max_z;

	return t_min < t1 && t_max > t0;
}

void BBox::draw(Color c, float alpha) const {

  glColor4f(c.r, c.g, c.b, alpha);

  // top
  glBegin(GL_LINE_STRIP);
  glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(max.x, max.y, max.z);
  glEnd();

  // bottom
  glBegin(GL_LINE_STRIP);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, min.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, min.y, min.z);
  glEnd();

  // side
  glBegin(GL_LINES);
  glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(min.x, min.y, max.z);
  glEnd();

}

std::ostream& operator<<(std::ostream& os, const BBox& b) {
  return os << "BBOX(" << b.min << ", " << b.max << ")";
}

} // namespace CGL
