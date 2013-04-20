/*
structures are defined here.
*/

typedef struct
{
	char* pairing_desc;
	pairing_t pairing;
	element_t g1, g2; //generators
	element_t PP2; //PP2 = e(g1, g2)^alpha d1*D1
	element_t gd11,gd12,gd13,gd14; //g1^d1 = (g1^d11, g1^d12, g1^d13, g1^d14)
	element_t gd21,gd22,gd23,gd24; //g1^d2 = (g1^d21, g1^d22, g1^d23, g1^d24)
}
w12ibe_pub_params;

typedef struct
{
	element_t alpha; // alpha
	element_t gD11, gD12, gD13, gD14; // g2^D1
	element_t gD21, gD22, gD23, gD24; // g2^D2 
}
w12ibe_msk;

typedef struct
{
	element_t ID, SK_ID1, SK_ID2, SK_ID3, SK_ID4; //
}
w12ibe_sk_id;

typedef struct
{	
	element_t one;
	element_t d11, d12, d13, d14, d21, d22, d23, d24, d31, d32, d33, d34, d41, d42, d43, d44;
  element_t D11, D12, D13, D14, D21, D22, D23, D24, D31, D32, D33, D34, D41, D42, D43, D44;
}
w12ibe_DPVS;

typedef struct
{
	element_t C0, C11, C12, C13, C14;
}w12ibe_ct;

typedef struct
{
	element_t sign1, sign2, sign3, sign4;
}w12ibe_sig;
