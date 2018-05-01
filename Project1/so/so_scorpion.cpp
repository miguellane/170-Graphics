
# include "so_scorpion.h"

SoScorpion::SoScorpion()
 {
 }
void SoScorpion::init (const GlProgram& prog, GlMaterial& mat, float shine)
 {
	torso.init(prog, mat, shine);
	lClaw.init(prog, mat, shine);		rClaw.init(prog, mat, shine);
	l1Leg.init(prog, mat, shine);		r1Leg.init(prog, mat, shine);
	l2Leg.init(prog, mat, shine);		r2Leg.init(prog, mat, shine);
	l3Leg.init(prog, mat, shine);		r3Leg.init(prog, mat, shine);
	l4Leg.init(prog, mat, shine);		l4Leg.init(prog, mat, shine);
	tail.init(prog, mat, shine);
 }

void SoScorpion::build ( int nface)
 {
	torso.build(nface);
	lClaw.build(nface);		rClaw.build(nface);
	l1Leg.build(nface);		r1Leg.build(nface);
	l2Leg.build(nface);		r2Leg.build(nface);
	l3Leg.build(nface);		r3Leg.build(nface);
	l4Leg.build(nface);		l4Leg.build(nface);
	tail.build(nface);
 }

void SoScorpion::draw(GsMat& tr, GsMat& pr, GlLight& light, float deg1, float deg2, float deg3, float deg4, float deg5)
 {
	GsMat t1, t2;
	GsMat Mat1, Mat2, Mat3, Mat4, Mat5, Mat6, Mat7, Mat8, Mat9, Mat10, Mat11, Mat12;
	Mat3.e11 *= -1;
	Mat5.e11 *= -1;
	Mat7.e11 *= -1;
	Mat9.e11 *= -1;
	Mat11.e11 *= -1;
	t1.translation(0.0f, 0.15f, 0.0f);
	t2.translation(0.01f, 0.0f, -0.1f);
	Mat1 = Mat1 * t1;
	Mat2 = Mat2 * t2 * t1;			Mat3 = Mat3 * t2 * t1;
	t1.translation(0.35f, 0.05f, -0.25f);
	Mat4 = Mat4 * t1;					Mat5 = Mat5 * t1;
	t1.translation(0.35f, 0.05f, -0.4f);
	Mat6 = Mat6 * t1;					Mat7 = Mat7 * t1;
	t1.translation(0.35f, 0.05f, -0.55f);
	Mat8 = Mat8 * t1;					Mat9 = Mat9 * t1;
	t1.translation(0.35f, 0.05f, -0.7f);
	Mat10 = Mat10 * t1;					Mat11 = Mat11 * t1;
	t1.translation(0.0f, 0.15f, -0.8f);
	Mat12 = Mat12 * t1;

	
	torso.draw(tr * Mat1, pr, light);
	lClaw.draw(tr * Mat2, pr, light, deg1);		rClaw.draw(tr * Mat3, pr, light, deg2);
	l1Leg.draw(tr * Mat4, pr, light, deg3);		r1Leg.draw(tr * Mat5, pr, light, deg4);
	l2Leg.draw(tr * Mat6, pr, light, deg3);		r2Leg.draw(tr * Mat7, pr, light, deg4);
	l3Leg.draw(tr * Mat8, pr, light, deg3);		r3Leg.draw(tr * Mat9, pr, light, deg4);
	l4Leg.draw(tr * Mat10, pr, light, deg3);		l4Leg.draw(tr * Mat11, pr, light, deg4);
	tail.draw(tr * Mat12, pr, light, deg5);
 }

