
# include "so_tail.h"

SoTail::SoTail()
{
}
void SoTail::init(const GlProgram& prog, GlMaterial& mat, float shine)
{
	p6.init(prog, mat, shine);
	p5.init(prog, mat, shine);
	p4.init(prog, mat, shine);
	p3.init(prog, mat, shine);
	p2.init(prog, mat, shine);
	p1.init(prog, mat, shine);
}

void SoTail::build(int nface)
{
	p6.build(nface, 0.2f,
		0.0f, 0.0f,
		0.01f, 0.01f,
		0.01f, 0.01f,
		0.04f, 0.08f);
	p5.build(nface, 0.2f,
		0.035f, 0.07f,
		0.0375f, 0.075f,
		0.0375f, 0.075f,
		0.04f, 0.08f);
	p4.build(nface, 0.2f,
		0.035f, 0.07f,
		0.0375f, 0.075f,
		0.0375f, 0.075f,
		0.04f, 0.08f);
	p3.build(nface, 0.2f,
		0.035f, 0.07f,
		0.0375f, 0.075f,
		0.0375f, 0.075f,
		0.04f, 0.08f);
	p2.build(nface, 0.2f,
		0.035f, 0.07f,
		0.0375f, 0.075f,
		0.0375f, 0.075f,
		0.04f, 0.08f);
	p1.build(nface, 0.2f,
		0.035f, 0.07f,
		0.0375f, 0.075f,
		0.0375f, 0.075f,
		0.04f, 0.08f);
}

void SoTail::draw(GsMat& tr, GsMat& pr, GlLight& light, float deg)
{
	GsMat p1Mat, p2Mat, p3Mat, p4Mat, p5Mat, p6Mat, t1, t2;

	t2.rotx(GS_TORAD(-15.0f + deg));
	p1Mat = p1Mat * t2;
	t2.rotx(GS_TORAD(15.0f));
	t1.translation(0.0f, 0.2f, 0.0f);
	p2Mat = p1Mat * t1 * t2;
	p3Mat = p2Mat * t1 * t2;
	p4Mat = p3Mat * t1 * t2;
	p5Mat = p4Mat * t1 * t2;
	t2.rotx(GS_TORAD(30.0f));
	p6Mat = p5Mat * t1 * t2;

	p6.draw(tr * p6Mat, pr, light);
	p5.draw(tr * p5Mat, pr, light);
	p4.draw(tr * p4Mat, pr, light);
	p3.draw(tr * p3Mat, pr, light);
	p2.draw(tr * p2Mat, pr, light);
	p1.draw(tr * p1Mat, pr, light);
}

