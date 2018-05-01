# include "so_curve_eval.h"

void eval_lagrange(const GsArray<GsVec>& ctrlpnts, GsArray<GsVec>& curvepnts, int segments) {
	GsVec add;
	float term, t;
	//For each line A-----B save A
	curvepnts.push(ctrlpnts[0]);
	for (int s = 1; s < segments; s++) {
		t = (float)(ctrlpnts.size() - 1) * s / segments;
//Start Lagrange
		add = GsVec(0, 0, 0);
		for (int i = 0; i < ctrlpnts.size(); i++) {
			term = 1;
			for (int j = 0; j <  ctrlpnts.size(); j++) {
				if (j != i) 
					term *= (t - j) / (i - j);
			}
			add += ctrlpnts[i] * term;
		}
		curvepnts.push(add);
//End Lagrange
		}
	//Save B
	curvepnts.push(ctrlpnts[ctrlpnts.size() - 1]);
}

void eval_bezier(const GsArray<GsVec>& ctrlpnts, GsArray<GsVec>& curvepnts, int segments) {
	GsArray<GsVec> temp1, temp2;
	float t;

	curvepnts.push(ctrlpnts[0]);
	for(int s = 1; s < segments; s++){
		t = (float)s / segments;
//Bezier
		temp1 = ctrlpnts;
		while(temp1.size() != 1){
			temp2.remove(0, temp2.size());
			for(int i = 0; i < temp1.size() - 1; i++){
				temp2.push( temp1[i] + (temp1[i + 1] - temp1[i]) * t );
			}
			temp1 = temp2;
		}
//End Bezier
		curvepnts.push(temp1[0]);
	}
	curvepnts.push(ctrlpnts[ctrlpnts.size() - 1]);
}

float calcN(int i, int k, float u) {
	float ui = float(i);
	if (k == 1)
		return ui <= u && u < ui + 1 ? 1.0f : 0;
	else
		return ((u - ui) / (k - 1)) * calcN(i, k - 1, u) + ((ui + k - u) / (k - 1)) * calcN(i + 1, k - 1, u);
}

void eval_bspline(const GsArray<GsVec>& ctrlpnts, GsArray<GsVec>& curvepnts, int segments) {
	int k = 3;
	GsVec add;
	float t, N;

	for (int s = 1; s < segments; s++) {
		t = (float)(ctrlpnts.size() - k + 1) * s / segments + k - 1;
//Bspline
		add = GsVec(0, 0, 0);
		for(int i = 0; i < ctrlpnts.size(); i++){
			N = calcN(i, k, t);
			add += ctrlpnts[i] * N;
		}
//End Bezier
		curvepnts.push(add);
	}
}

void eval_crspline(const GsArray<GsVec>& ctrlpnts, GsArray<GsVec>& curvepnts, int segments) {
	GsVec add;
	float t;
	int i;
	curvepnts.push(ctrlpnts[1]);
	for (int s = 1; s < segments; s++) {
		t = (float)(ctrlpnts.size() - 3) * s / segments;
//Start Catmull
		add = GsVec(0, 0, 0);
		i = (int)t + 1;
		t -= (int)t;

		add = 0.5 * (
			(2 * ctrlpnts[i]) +
			(-ctrlpnts[i - 1] + ctrlpnts[i + 1]) * t +
			(2 * ctrlpnts[i - 1] - 5 * ctrlpnts[i] + 4 * ctrlpnts[i + 1] - ctrlpnts[i + 2]) * t * t +
			(-ctrlpnts[i - 1] + 3 * ctrlpnts[i] - 3 * ctrlpnts[i + 1] + ctrlpnts[i + 2]) * t * t * t);
//End
		curvepnts.push(add);
	}
	curvepnts.push(ctrlpnts[ctrlpnts.size() - 2]);
}

void eval_bospline(const GsArray<GsVec>& ctrlpnts, GsArray<GsVec>& curvepnts, int segments) {
	GsArray<GsVec> splinepnts;
	GsVec vP, vM, v;
	float t, dP, dM, N;
	splinepnts.push(ctrlpnts[0]);
	for(int i = 1; i < ctrlpnts.size() - 1; i++){
		vM = ctrlpnts[i] - ctrlpnts[i - 1]; dM = vM.norm();
		vP = ctrlpnts[i + 1] - ctrlpnts[i]; dP = vP.norm();
		vM /= dM;
		vP /= dP;
		v = (dP * vM + dM * vP) / (dM + dP);
		splinepnts.push(ctrlpnts[i] - (dM * v / 3));
		splinepnts.push(ctrlpnts[i]);
		splinepnts.push(ctrlpnts[i] + (dP * v / 3));
	}
	splinepnts.push(ctrlpnts[ctrlpnts.size() - 1]);
	eval_bspline(splinepnts, curvepnts, segments);
}