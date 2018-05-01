
# include "so_leg.h"

SoLeg::SoLeg()
 {
 }
void SoLeg::init (const GlProgram& prog, GlMaterial& mat, float shine)
 {
	p4.init(prog, mat, shine);
	p3.init(prog, mat, shine);
	p2.init(prog, mat, shine);
	p1.init(prog, mat, shine);
 }

void SoLeg::build ( int nface)
 {
	p4.build(nface, 0.05f,
		0.02f, 0.02f,
		0.025f, 0.025f,
		0.025f, 0.025f,
		0.02f, 0.02f);
	p3.build(nface, 0.15f,
		0.02f, 0.02f,
		0.025f, 0.025f,
		0.025f, 0.025f,
		0.02f, 0.02f);
	p2.build(nface, 0.1f,
		0.02f, 0.02f,
		0.025f, 0.025f,
		0.025f, 0.025f,
		0.02f, 0.02f);
	p1.build(nface, 0.1f,
		0.02f, 0.02f,
		0.025f, 0.025f,
		0.0125f, 0.0125f,
		0.0f, 0.0f);


 }

void SoLeg::draw(GsMat& tr, GsMat& pr, GlLight& light, float deg)
 {
	GsMat p1Mat, p2Mat, p3Mat, p4Mat, t1, t2;

	t1.translation(-0.5f, -0.135f, 0.0f);
	t2.roty(GS_TORAD(deg));
	p1Mat = p1Mat * t1;
	p1Mat = p1Mat * t2;
	t1.translation(0.5f, 0.135f, 0.0f);
	p1Mat = p1Mat * t1;

	t2.rotz(GS_TORAD(20.0f));
	p1Mat = p1Mat * t2;
	t1.translation(0.0f, 0.09f, 0.0f);
	p2Mat = p1Mat * t1 * t2;
	t2.rotz(GS_TORAD(70.0f));
	p3Mat = p2Mat * t1 * t2;
	t1.translation(0.0f, 0.135f, 0.0f);
	t2.rotz(GS_TORAD(-20.0f));
	p4Mat = p3Mat * t1 * t2;



	p4.draw(tr * p4Mat, pr, light);
	p3.draw(tr * p3Mat, pr, light);
	p2.draw(tr * p2Mat, pr, light);
	p1.draw(tr * p1Mat, pr, light);
 }

