  
# include "so_claw.h"

SoClaw::SoClaw()
 {
 }
void SoClaw::init (const GlProgram& prog, GlMaterial& mat, float shine)
 {
	p4.init(prog, mat, shine);
	p3.init(prog, mat, shine);
	p2.init(prog, mat, shine);
	p1.init(prog, mat, shine);
 }

void SoClaw::build ( int nface)
 {
	p4.build(nface, 0.2f,
		0.0f, 0.0f,
		0.02f, 0.02f,
		0.02f, 0.02f,
		0.0f, 0.0f);
	p3.build(nface, 0.25f,
		0.0f, 0.0f,
		0.05f, 0.05f,
		0.1f, 0.1f,
		0.06f, 0.06f);
	p2.build(nface, 0.15f,
		0.06f, 0.06f,
		0.075f, 0.075f,
		0.05f, 0.05f,
		0.04f, 0.04f);
	p1.build(nface, 0.15f,
		0.04f, 0.04f,
		0.05f, 0.05f,
		0.025f, 0.025f,
		0.020f, 0.020f);
 }

void SoClaw::draw(GsMat& tr, GsMat& pr, GlLight& light, float deg)
 {
	GsMat p1Mat, p2Mat, p3Mat, p4Mat, t1, t2;

	t2.rotz(GS_TORAD(90.0f + deg));
	p1Mat = p1Mat * t2;
	t1.translation(0.0f, 0.135f, 0.0f);
	t2.rotx(GS_TORAD(45.0f));
	p2Mat = p1Mat * t1 * t2;
	p3Mat = p2Mat * t1 * t2;
	t1.translation(0.05f, 0.0f, 0.0f);
	t2.rotz(GS_TORAD(-20.0f));
	p4Mat = p3Mat * t1 * t2;

	p4.draw(tr * p4Mat, pr, light);
	p3.draw(tr * p3Mat, pr, light);
	p2.draw(tr * p2Mat, pr, light);
	p1.draw(tr * p1Mat, pr, light);
 }

