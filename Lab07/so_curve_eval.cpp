# include "so_curve_eval.h"

void eval_lagrange(const GsArray<GsVec>& ctrlpnts, GsArray<GsVec>& curvepnts, int segments) {
//For each line A-----B save A
	for (int a = 0; a < ctrlpnts.size() - 1; a++) {
		curvepnts.push(ctrlpnts[a]);
//For each - segment calc x and call Lagrange
		for (int b = 1; b < segments + 1; b++) {
			float x = ctrlpnts[a].x + (((ctrlpnts[a + 1].x - ctrlpnts[a].x) / segments) * b);
//Start Lagrange
			std::vector<float> num;
			float y = 0;
			float term;
			//Numerator Parts Pre-Calc
			for (int i = 0; i < ctrlpnts.size(); i++)
				num.push_back(x - ctrlpnts[i].x);

			for (int i = 0; i < ctrlpnts.size(); i++) {
				//Y component of term
				term = ctrlpnts[i].y;
				for (int j = 0; j < ctrlpnts.size(); j++) {
					if (j != i) {
						term *= num[j] / (ctrlpnts[i].x - ctrlpnts[j].x);
					}
				}
				y += term;
			}
			curvepnts.push(GsVec(x, y, 0.0f));
//End Lagrange
		}
	}
//Add the last B
	if (ctrlpnts.size() != 0)
		curvepnts.push(ctrlpnts[ctrlpnts.size() - 1]);

}

void eval_bezier(const GsArray<GsVec>& ctrlpnts, GsArray<GsVec>& curvepnts, int segments) {
	curvepnts = ctrlpnts;
	GsArray<GsVec> temp;
	for(int i = 0; i < segments; i++){
		temp.remove(0, temp.size());

		temp.push(curvepnts[0]);
		for(int j = 0; j < curvepnts.size() - 1; j++){
			temp.push((curvepnts[j] + curvepnts[j + 1] ) / 2);
		}
		temp.push(curvepnts[curvepnts.size() - 1]);
		curvepnts = temp;
	}

}