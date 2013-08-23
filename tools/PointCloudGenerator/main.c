/*
Copyright (c) 2011-2013 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "PointCloudIO.h"
#include "PointCloud.h"
#include "Triangle.h"
#include "Numeric.h"
#include "Memory.h"
#include "MeshIO.h"
#include "Random.h"
#include "Vector.h"
#include "Noise.h"
#include "Mesh.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

static const char USAGE[] =
"Usage: ptcgen [options] inputfile(*.mesh) outputfile(*.ptc)\n"
"Options:\n"
"  --help         Display this information\n"
"\n";

int main(int argc, const char **argv)
{
	const char *in_filename = NULL;
	const char *out_filename = NULL;

	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		printf("%s", USAGE);
		return 0;
	}

	if (argc != 3) {
		fprintf(stderr, "error: invalid number of arguments.\n");
		fprintf(stderr, "%s", USAGE);
		return -1;
	}

	in_filename = argv[1];
	out_filename = argv[2];

	{
		struct XorShift xr;
		struct Mesh *mesh = MshNew();
		struct PtcOutputFile *out = PtcOpenOutputFile(out_filename);
		struct Vector *P = NULL;
		double *radius = NULL;
		int *point_count_list = 0;
		int total_point_count = 0;
		int face_count = 0;
		int point_id = 0;
		int i;

		MshLoadFile(mesh, in_filename);

		face_count = MshGetFaceCount(mesh);
		point_count_list = MEM_ALLOC_ARRAY(int, face_count);

		for (i = 0; i < face_count; i++) {
			struct Vector P0 = {0, 0, 0};
			struct Vector P1 = {0, 0, 0};
			struct Vector P2 = {0, 0, 0};
			struct Vector center = {0, 0, 0};
			double noise_val = 0;
			double area = 0;
			int npt_on_face = 0;

			MshGetFaceVertexPosition(mesh, i, &P0, &P1, &P2);

			center.x = (P0.x + P1.x + P2.x) / 3;
			center.y = (P0.y + P1.y + P2.y) / 3;
			center.z = (P0.z + P1.z + P2.z) / 3;
			center.x *= 2.5;
			center.y *= 2.5;
			center.z *= 2.5;
			noise_val = PerlinNoise(&center, 2, .5, 8);
			noise_val = Fit(noise_val, -.2, 1, 0, 1);

			area = TriComputeArea(&P0, &P1, &P2);
			area *= noise_val;
			npt_on_face = (int) 2000000 * area;

			total_point_count += npt_on_face;
			point_count_list[i] = npt_on_face;
		}
		printf("total_point_count %d\n", total_point_count);

		P = MEM_ALLOC_ARRAY(struct Vector, total_point_count);
		radius = MEM_ALLOC_ARRAY(double, total_point_count);

		XorInit(&xr);
		point_id = 0;
		for (i = 0; i < face_count; i++) {
			struct Vector P0 = {0, 0, 0};
			struct Vector P1 = {0, 0, 0};
			struct Vector P2 = {0, 0, 0};
			struct Vector N0 = {0, 0, 0};
			struct Vector N1 = {0, 0, 0};
			struct Vector N2 = {0, 0, 0};
			const int npt_on_face = point_count_list[i];
			int j;

			MshGetFaceVertexPosition(mesh, i, &P0, &P1, &P2);
			MshGetFaceVertexNormal(mesh, i, &N0, &N1, &N2);

			for (j = 0; j < npt_on_face; j++) {
				struct Vector normal = {0, 0, 0};
				struct Vector *P_out = &P[point_id];
				double u = 0, v = 0, t = 0;
				double offset = 0;

				u = XorNextFloat01(&xr);
				v = (1 - u) * XorNextFloat01(&xr);

				TriComputeNormal(&normal, &N0, &N1, &N2, u, v);

				t = 1 - u - v;
				P_out->x = t * P0.x + u * P1.x + v * P2.x;
				P_out->y = t * P0.y + u * P1.y + v * P2.y;
				P_out->z = t * P0.z + u * P1.z + v * P2.z;

				offset = XorNextFloat01(&xr);
				offset *= -.05;
				P_out->x += offset * normal.x;
				P_out->y += offset * normal.y;
				P_out->z += offset * normal.z;

				radius[point_id] = .01 * .2;

				point_id++;
			}
		}

		PtcSetOutputPosition(out, P, total_point_count);
		PtcSetOutputAttributeDouble(out, "radius", radius);

		PtcWriteFile(out);

		PtcCloseOutputFile(out);
		MshFree(mesh);

		MEM_FREE(point_count_list);
		MEM_FREE(P);
		MEM_FREE(radius);
	}

	return 0;
}
