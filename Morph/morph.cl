__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;
;

int getIndex(float2 v, int width)
{
	return 4 * round(v.y) * width + 4 * round(v.x);;;
}

__kernel void morph(__write_only image2d_t current, const unsigned int width, const unsigned int height, __read_only image2d_t image_a, __read_only image2d_t image_b, float lambda, int n, __global  float* keys, __global  float* tmp_p, __global  float* tmp_q, __global  float* tmp_c)
{

	int xpos = get_global_id(0);
	int ypos = get_global_id(1);

	float2 pos = (float2)(xpos, ypos);
	int p = 4 * ypos *width + 4*xpos;

	float4 A = read_imagef(image_a, sampler, pos)/255.0f;
	float4 B = read_imagef(image_b, sampler, pos) / 255.0f;

	float2 licznikP = (float2)(0, 0);
	float2 licznikQ = (float2)(0, 0);
	float mianownik = 0;

	// pêtla po kluczach
	int j = 0;
	for (int i = 0; i < 4 * n; i+=4)
	{
		// klucz na obrazie pocz¹tkowym
		//float2 a = (float2)(keys[i] , keys[i + 1]);
		// klucz na obrazie koñcowym
		//float2 b = (float2)(keys[i + 2] , keys[i + 3] ); 

		// punkt przejœciowy
		float2 c = (float2)(tmp_c[j], tmp_c[j + 1]);;// (1.0 - lambda) * a + lambda * b;
		// dwa punkty pomocnicze
		float2 p = (float2)(tmp_p[j], tmp_p[j + 1]);; // a-c
		float2 q = (float2)(tmp_q[j], tmp_q[j + 1]);;//b-c

		// kwadrat odleg³oœci od punktu przejœciowego i aktualnego
		float dist2 = (pos.x - c.x)*(pos.x - c.x) + (pos.y - c.y)*(pos.y - c.y);

		licznikP += p / dist2;
		licznikQ += q / dist2;

		mianownik += 1.0 / dist2;
		j += 2;
	}

	float2 P = licznikP / mianownik + pos;
	float2 Q = licznikQ / mianownik + pos;
	
	if (P.x > width) P.x = width;
	if (P.y > height) P.y = height;
	if (P.x < 0) P.x = 0;
	if (P.y < 0) P.y = 0;
	if (Q.x > width) Q.x = width;
	if (Q.y > height) Q.y = height;
	if (Q.x < 0) Q.x = 0;
	if (Q.y < 0) Q.y = 0;

	float4 Pc = read_imagef(image_a, sampler, P) / 255.0f;  
	float4 Qc = read_imagef(image_b, sampler, Q) / 255.0f;

	float4 licznikPc = (float4)(0, 0, 0, 0);
	float mianownikPc = 0;
	float4 licznikQc = (float4)(0, 0, 0, 0);
	float mianownikQc = 0;

	for (int i = 0; i < 4 * n; i += 4)
	{

		float2 c = (float2)(tmp_c[j], tmp_c[j + 1]);;// (1.0 - lambda) * a + lambda * b;


		// kwadrat odleg³oœæi wyliczonego P od c
		float Pdist2 = (P.x - c.x)* (P.x - c.x) + (P.y - c.y)* (P.y - c.y);
	//	// kwadrat odleg³oœæi wyliczonego Q od c
		float Qdist2 = (Q.x - c.x)* (Q.x - c.x) + (Q.y - c.y)* (Q.y - c.y);

		mianownikPc += 1.0 / Pdist2;
		licznikPc += Pc  / Pdist2;

		mianownikQc += 1.0 / Qdist2;
		licznikQc += Qc   / Qdist2;
	}

	float4 Pcolor = licznikPc / mianownikPc;
	float4 Qcolor = licznikQc / mianownikQc;

	float4 color = (1.0 - lambda)*Pcolor + lambda*Qcolor;



	float4 out = lambda * A + (1 - lambda) * B;
	out = color*255;
	if(out.x < 0) out.x = 0;if(out.x > 255) out.x = 255;
	if(out.y < 0) out.y = 0;if(out.y > 255) out.y = 255;
	if(out.z < 0) out.z = 0;if(out.z > 255) out.z = 255;
	if(out.w < 0) out.w = 0;if(out.w > 255) out.w = 255;
	//  out = (float4)(124, 0, 255, 0);
	write_imagef(current, (int2)(xpos, ypos), out);


}
