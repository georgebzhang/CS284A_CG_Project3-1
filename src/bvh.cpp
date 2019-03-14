#include "bvh.h"

#include "CGL/CGL.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CGL { namespace StaticScene {

BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
                   size_t max_leaf_size) {

  root = construct_bvh(_primitives, max_leaf_size);

}

BVHAccel::~BVHAccel() {
  if (root) delete root;
}

BBox BVHAccel::get_bbox() const {
  return root->bb;
}

void BVHAccel::draw(BVHNode *node, const Color& c, float alpha) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->draw(c, alpha);
  } else {
    draw(node->l, c, alpha);
    draw(node->r, c, alpha);
  }
}

void BVHAccel::drawOutline(BVHNode *node, const Color& c, float alpha) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->drawOutline(c, alpha);
  } else {
    drawOutline(node->l, c, alpha);
    drawOutline(node->r, c, alpha);
  }
}

int get_extent_max_index(Vector3D extent) {
	double extent_max = max({ extent.x, extent.y, extent.z });
	if (extent.x == extent_max) return 0;
	else if (extent.y == extent_max) return 1;
	else return 2;
}

BVHNode *BVHAccel::construct_bvh(const std::vector<Primitive*>& prims, size_t max_leaf_size) {
	// TODO (Part 2.1):
	// Construct a BVH from the given vector of primitives and maximum leaf
	// size configuration. The starter code build a BVH aggregate with a
	// single leaf node (which is also the root) that encloses all the
	// primitives.
	BBox centroid_box, bbox;

	for (Primitive *p : prims) {
		BBox bb = p->get_bbox();
		bbox.expand(bb);
		Vector3D c = bb.centroid();
		centroid_box.expand(c);
	}

	BVHNode *node = new BVHNode(bbox);
	node->prims = new vector<Primitive *>(prims);

	stack<BVHNode*> st;
	st.push(node); // stack to implement "recursive" function iteratively, can avoid stack overflow

	while (!st.empty()) {
		BVHNode* b = st.top();
		st.pop();

		if (b->prims->size() > max_leaf_size) {
			int extent_max_index = get_extent_max_index(b->bb.extent);

			// 1st pass through prims to find average centroid
			Vector3D c_total = Vector3D();
			for (Primitive *p : *(b->prims)) {
				Vector3D c = p->get_bbox().centroid();
				c_total += c;
				centroid_box.expand(c);
			}
			Vector3D c_avg = c_total / b->prims->size();
			double c_avg_arr[] = { c_avg.x, c_avg.y, c_avg.z };

			// 2nd pass through prims to divide primitives among left and right children
			BBox bboxl;
			BBox bboxr;
			std::vector<Primitive*> primsl;
			std::vector<Primitive*> primsr;
			for (Primitive *p : *(b->prims)) {
				BBox bb = p->get_bbox();
				Vector3D c = bb.centroid();
				double c_arr[] = { c.x, c.y, c.z };
				if (c_arr[extent_max_index] < c_avg_arr[extent_max_index]) {
					primsl.push_back(p);
					bboxl.expand(bb);
				}
				else {
					primsr.push_back(p);
					bboxr.expand(bb);
				}
			}

			BVHNode* bl = new BVHNode(bboxl);
			bl->prims = new vector<Primitive *>(primsl);
			BVHNode* br = new BVHNode(bboxr);
			br->prims = new vector<Primitive *>(primsr);
			b->l = bl;
			b->r = br;
			st.push(bl);
			st.push(br);
		}
	}

	return node;
}


bool BVHAccel::intersect(const Ray& ray, BVHNode *node) const {
	// TODO (Part 2.3):
	// Fill in the intersect function.
	// Take note that this function has a short-circuit that the
	// Intersection version cannot, since it returns as soon as it finds
	// a hit, it doesn't actually have to find the closest hit.

	//for (Primitive *p : *(root->prims)) {
	//total_isects++;
	//if (p->intersect(ray)) 
	//	return true;
	//}
	return intersect(ray, nullptr, node);
}

bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode *node) const {
	// TODO (Part 2.3):
	// Fill in the intersect function.
	if (node == nullptr) return false;
	if (!node->bb.intersect(ray, ray.min_t, ray.max_t)) return false;

	bool hit = false;

	if (node->isLeaf()) {
		for (Primitive *p : *(node->prims)) {
			total_isects++;
			if (p->intersect(ray, i))
				hit = true;
		}
	}

	bool hit1 = intersect(ray, i, node->l);
	bool hit2 = intersect(ray, i, node->r);

	return hit || hit1 || hit2;
}

}  // namespace StaticScene
}  // namespace CGL
