
# include "so_torso.h"

SoTorso::SoTorso()
 {
 }
void SoTorso::init (const GlProgram& prog, GlMaterial& mat, float shine)
 {
	p9.init(prog, mat, shine);
	p8.init(prog, mat, shine);
	p7.init(prog, mat, shine);
	p6.init(prog, mat, shine);
	p5.init(prog, mat, shine);
	p4.init(prog, mat, shine);
	p3.init(prog, mat, shine);
	p2.init(prog, mat, shine);
	p1.init(prog, mat, shine);
 }

void SoTorso::build ( int nface)
 {


	p9.build(nface, 0.1f,
		0.05f, 0.1f,
		0.0625f, 0.125f,
		0.0625f, 0.125f,
		0.05f, 0.1f);
	p8.build(nface, 0.1f,
		0.05f, 0.1f,
		0.0625f, 0.125f,
		0.0625f, 0.125f,
		0.05f, 0.1f);
	p7.build(nface, 0.1f,
		0.05f, 0.1f,
		0.0625f, 0.125f,
		0.0625f, 0.125f,
		0.05f, 0.1f);
	p6.build(nface, 0.1f,
		0.05f, 0.1f,
		0.0625f, 0.125f,
		0.0625f, 0.125f,
		0.05f, 0.1f);
	p5.build(nface, 0.05f,
		0.05f, 0.1f,
		0.0625f, 0.125f,
		0.0625f, 0.125f,
		0.05f, 0.1f);
	p4.build(nface, 0.05f,
		0.05f, 0.1f,
		0.0625f, 0.125f,
		0.0625f, 0.125f,
		0.05f, 0.1f);
	p3.build(nface, 0.05f,
		0.05f, 0.1f,
		0.0625f, 0.125f,
		0.0625f, 0.125f,
		0.05f, 0.1f);
	p2.build(nface, 0.05f,
		0.05f, 0.1f,
		0.0625f, 0.125f,
		0.0625f, 0.125f,
		0.04f, 0.08f);
	p1.build(nface, 0.25f,
		0.04f, 0.08f,
		0.05f, 0.1f,
		0.05f, 0.1f,
		0.04f, 0.08f);


 }

void SoTorso::draw(GsMat& tr, GsMat& pr, GlLight& light)
 {
	GsMat p1Mat, p2Mat, p3Mat, p4Mat, p5Mat, p6Mat, p7Mat, p8Mat, p9Mat, t1;

	p1Mat.rotx(GS_TORAD(-90.0f));
	t1.translation(0.0f, 0.225f, 0.0f);
	p2Mat = p1Mat * t1;
	t1.translation(0.0f, 0.045f, 0.0f);
	p3Mat = p2Mat * t1;
	p4Mat = p3Mat * t1;
	p5Mat = p4Mat * t1;
	p6Mat = p5Mat * t1;
	t1.translation(0.0f, 0.09f, 0.0f);
	p7Mat = p6Mat * t1;
	p8Mat = p7Mat * t1;
	p9Mat = p8Mat * t1;

	p9.draw(tr * p9Mat, pr, light);
	p8.draw(tr * p8Mat, pr, light);
	p7.draw(tr * p7Mat, pr, light);
	p6.draw(tr * p6Mat, pr, light);
	p5.draw(tr * p5Mat, pr, light);
	p4.draw(tr * p4Mat, pr, light);
	p3.draw(tr * p3Mat, pr, light);
	p2.draw(tr * p2Mat, pr, light);
	p1.draw(tr * p1Mat, pr, light);
 }

