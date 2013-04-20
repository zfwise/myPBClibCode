#include <stdlib.h>
#include <pbc.h>
#include <string.h>
#include "w12ibe_private.h"
#include "w12ibe.h"

#define TYPE_A_PARAMS \
"type a\n" \
"q 87807107996633125224377819847540498158068831994142082" \
"1102865339926647563088022295707862517942266222142315585" \
"8769582317459277713367317481324925129998224791\n" \
"h 12016012264891146079388821366740534204802954401251311" \
"822919615131047207289359704531102844802183906537786776\n" \
"r 730750818665451621361119245571504901405976559617\n" \
"exp2 159\n" \
"exp1 107\n" \
"sign1 1\n" \
"sign0 1\n"

#define TYPE_D_PARAMS \
"type d\n" \
"q 625852803282871856053922297323874661378036491717\n" \
"n 625852803282871856053923088432465995634661283063\n" \
"h 3\n" \
"r 208617601094290618684641029477488665211553761021\n" \
"a 581595782028432961150765424293919699975513269268\n" \
"b 517921465817243828776542439081147840953753552322\n" \
"k 6\n" \
"nk\n" \
"6009429035640840713098416112731007851636003186841796" \
"8262992864809623507269833854678414046779817844853757026858774" \
"9663314341982575124579932932718490436646551464432290290694633" \
"9204683783026799422278916004733743207526661908265764036498641" \
"5435746294498140589844832666082434658532589211525696\n" \
"hk\n" \
"13808017118622124844032056990052421415416297614338991" \
"49236405232528956996854655261075303661691995273080620762287276" \
"05136144652850463328315227883118371130132976559145068025000059" \
"2437612973269056\n" \
"coeff0 472731500571015189154958232321864199355792223347\n" \
"coeff1 352243926696145937581894994871017455453604730246\n" \
"coeff2 289113341693870057212775990719504267185772707305\n" \
"nqr 431211441436589568382088865288592347194866189652\n"


/*
	One should notice that I didn't free the element in every function.
	For example, the variable t1-t4 in w12ibe_verify should be set free
	after use. One should use element_clear() to do that. If you are running
	the code on a memory restrict machine, you should probably add memory
	free statements. 
	For simplity, I didn't free any memory. I'll do it if necessary.
*/

char last_error[256];

char*
w12ibe_error()
{
	return last_error;
}

int w12ibe_verify( w12ibe_pub_params* pub, w12ibe_sig* sign, element_t mHash)
{
	element_t t1, t2, t3, t4, t11, t12, t13, t14;
	element_init_G1(t1,	pub->pairing);
	element_init_G1(t11,	pub->pairing);
	element_init_G1(t12,	pub->pairing);
	element_init_G1(t13,	pub->pairing);
	element_init_G1(t14,	pub->pairing);
	element_init_GT(t2,	pub->pairing);
	element_init_GT(t3,	pub->pairing);
	element_init_GT(t4,	pub->pairing);
	element_pow_zn(t1, pub->gd21, mHash);
	element_mul(t11, t1, pub->gd11);
	element_pow_zn(t1, pub->gd22, mHash);
	element_mul(t12, t1, pub->gd12);
	element_pow_zn(t1, pub->gd23, mHash);
	element_mul(t13, t1, pub->gd13);
	element_pow_zn(t1, pub->gd24, mHash);
	element_mul(t14, t1, pub->gd14);
	element_pairing(t2, t11, sign->sign1);
	element_pairing(t3, t12, sign->sign2);
	element_mul(t4, t2, t3);
	element_pairing(t2, t13, sign->sign3);
	element_mul(t4, t4, t2);
	element_pairing(t2, t14, sign->sign4);
	element_mul(t4, t4, t2);
	
	
	if (!element_cmp(pub->PP2, t4)) {
    		return 1;
	}
  	else{
    		return 0;
   	}
	
}

w12ibe_sig* w12ibe_sign( w12ibe_pub_params* pub, w12ibe_msk* msk, element_t mHash)
{
	w12ibe_sig* sign;
	sign = malloc(sizeof(w12ibe_sig));
	element_init_G2(sign->sign1, 	pub->pairing);
  	element_init_G2(sign->sign2, 	pub->pairing);
 	element_init_G2(sign->sign3, 	pub->pairing);
  	element_init_G2(sign->sign4, 	pub->pairing);
	element_t r, r_m, a_r_m;
	element_init_Zr(r,	pub->pairing);
	element_init_Zr(r_m,	pub->pairing);
	element_init_Zr(a_r_m,	pub->pairing);
	element_random(r);
	element_mul(r_m, r, mHash);
	element_add(a_r_m, msk->alpha, r_m);
	element_t t3, t4;
	element_init_G2(t3,	pub->pairing);
	element_init_G2(t4,	pub->pairing);
	element_pow_zn(t3, msk->gD11, a_r_m);
	element_pow_zn(t4, msk->gD21, r);
	element_div(sign->sign1, t3, t4);
	element_pow_zn(t3, msk->gD12, a_r_m);
	element_pow_zn(t4, msk->gD22, r);
	element_div(sign->sign2, t3, t4);
	element_pow_zn(t3, msk->gD13, a_r_m);
	element_pow_zn(t4, msk->gD23, r);
	element_div(sign->sign3, t3, t4);
	element_pow_zn(t3, msk->gD14, a_r_m);
	element_pow_zn(t4, msk->gD24, r);
	element_div(sign->sign4, t3, t4);
	
	return sign;
}

int w12ibe_dec( w12ibe_pub_params* pub, w12ibe_ct* CT, w12ibe_sk_id* sk, element_t m)
{
	//Decrypt Message M'
  	element_t Dec1, Dec2, Dec3, Dec4, Dec_bot12, Dec_bot123, Dec_bot1234;
  	element_init_GT(Dec1, 	pub->pairing);
  	element_init_GT(Dec2, 	pub->pairing);
 	element_init_GT(Dec3, 	pub->pairing);
  	element_init_GT(Dec4, 	pub->pairing);
  	element_init_GT(Dec_bot12, 	pub->pairing);
  	element_init_GT(Dec_bot123, 	pub->pairing);
  	element_init_GT(Dec_bot1234, 	pub->pairing);
  	element_pairing(Dec1, CT->C11, sk->SK_ID1);
  	element_pairing(Dec2, CT->C12, sk->SK_ID2);
  	element_pairing(Dec3, CT->C13, sk->SK_ID3);
  	element_pairing(Dec4, CT->C14, sk->SK_ID4);
  	element_mul(Dec_bot12, Dec1, Dec2);
  	element_mul(Dec_bot123, Dec_bot12, Dec3);
  	element_mul(Dec_bot1234, Dec_bot123, Dec4);

  	element_init_GT(m, 	pub->pairing);
  	element_div(m, CT->C0, Dec_bot1234);
	return 1;
}

w12ibe_ct* w12ibe_enc( w12ibe_pub_params* pub, element_t m, element_t ID)
{
	w12ibe_ct* CT;
	CT = malloc(sizeof(w12ibe_ct));
	//Encrypt Message M
  	element_t s, s_ID, C0_left;
  	element_init_Zr(s, 	pub->pairing);
  	element_init_Zr(s_ID, 	pub->pairing);
 	element_random(s);
  	element_mul(s_ID, s, ID);
  	element_init_GT(m, 	pub->pairing);
	element_random(m);
  	//element_printf("m = %B\n", m);
 	element_init_GT(CT->C0, pub->pairing);
  	element_init_GT(C0_left, pub->pairing);
  	element_pow_zn(C0_left, pub->PP2, s);
  	element_mul(CT->C0, m, C0_left);
  	//element_printf("\n-C0 = %B\n", C0);
  	element_t Ctemp1, Ctemp2;
  	element_init_G1(Ctemp1, pub->pairing);
  	element_init_G1(Ctemp2, pub->pairing);
  	element_init_G1(CT->C11, pub->pairing);
  	element_init_G1(CT->C12, pub->pairing);
  	element_init_G1(CT->C13, pub->pairing);
  	element_init_G1(CT->C14, pub->pairing);
  	element_pow_zn(Ctemp1, pub->gd11, s);
  	element_pow_zn(Ctemp2, pub->gd21, s_ID);
  	element_mul(CT->C11, Ctemp1, Ctemp2);
  	element_pow_zn(Ctemp1, pub->gd12, s);
  	element_pow_zn(Ctemp2, pub->gd22, s_ID);
  	element_mul(CT->C12, Ctemp1, Ctemp2);
  	element_pow_zn(Ctemp1, pub->gd13, s);
  	element_pow_zn(Ctemp2, pub->gd23, s_ID);
  	element_mul(CT->C13, Ctemp1, Ctemp2);
  	element_pow_zn(Ctemp1, pub->gd14, s);
  	element_pow_zn(Ctemp2, pub->gd24, s_ID);
  	element_mul(CT->C14, Ctemp1, Ctemp2);
	return CT;
}

w12ibe_sk_id* w12ibe_keygen( w12ibe_pub_params* pub, w12ibe_msk* msk, element_t ID)
{
	w12ibe_sk_id* SK;
	SK = malloc(sizeof(w12ibe_sk_id));

	element_t r, SK_ID_top, SK_ID_bot, SK_temp1, SK_temp2;
  	element_init_Zr(r, 		pub->pairing);
  	element_random(r);
  	element_init_Zr(SK->ID, 	pub->pairing);
	element_set(SK->ID, ID);
	element_init_Zr(SK_temp1, 	pub->pairing);
  	element_init_Zr(SK_temp2, 	pub->pairing);
  	element_init_G2(SK->SK_ID1, 	pub->pairing);
	element_init_G2(SK->SK_ID2, 	pub->pairing);
  	element_init_G2(SK->SK_ID3, 	pub->pairing);
	element_init_G2(SK->SK_ID4, 	pub->pairing);
  	element_init_G2(SK_ID_top, 	pub->pairing);
	element_init_G2(SK_ID_bot, 	pub->pairing);
  	element_mul(SK_temp1, r, SK->ID);
 	element_add(SK_temp2, msk->alpha, SK_temp1);
  	element_pow_zn(SK_ID_top, msk->gD11, SK_temp2);
  	element_pow_zn(SK_ID_bot, msk->gD21, r);
  	element_div(SK->SK_ID1, SK_ID_top, SK_ID_bot);
  	element_pow_zn(SK_ID_top, msk->gD12, SK_temp2);
  	element_pow_zn(SK_ID_bot, msk->gD22, r);
  	element_div(SK->SK_ID2, SK_ID_top, SK_ID_bot);
  	element_pow_zn(SK_ID_top, msk->gD13, SK_temp2);
  	element_pow_zn(SK_ID_bot, msk->gD23, r);
  	element_div(SK->SK_ID3, SK_ID_top, SK_ID_bot);
  	element_pow_zn(SK_ID_top, msk->gD14, SK_temp2);
  	element_pow_zn(SK_ID_bot, msk->gD24, r);
  	element_div(SK->SK_ID4, SK_ID_top, SK_ID_bot);

	return SK;
}
void w12ibe_setup( w12ibe_pub_params** pub, w12ibe_msk** msk)
{
	*pub = malloc(sizeof(w12ibe_pub_params));
	*msk = malloc(sizeof(w12ibe_msk));
	w12ibe_DPVS* dpvs; //dual pairing vector spaces
	(*pub)->pairing_desc = strdup(TYPE_D_PARAMS);
	pairing_init_set_buf((*pub)->pairing, (*pub)->pairing_desc, strlen((*pub)->pairing_desc));

	dpvs = w12ibe_DPVS_gen(*pub);//generate the dual pairing vector spaces

	element_init_G1((*pub)->g1,	(*pub)->pairing);
	element_init_G2((*pub)->g2,	(*pub)->pairing);
	element_init_Zr((*msk)->alpha, 	(*pub)->pairing);
  	element_random((*pub)->g1);
  	//element_printf("\n-g1 = %B\n", g1);
  	element_random((*pub)->g2);
 	//element_printf("-g2 = %B\n", g2);
  	element_random((*msk)->alpha);
  	//element_printf("-alpha = %B\n", alpha);

  	// g1^d1, g1^d2
  	element_init_G1((*pub)->gd11, 	(*pub)->pairing);
	element_init_G1((*pub)->gd12,	(*pub)->pairing);
  	element_init_G1((*pub)->gd13, 	(*pub)->pairing);
	element_init_G1((*pub)->gd14, 	(*pub)->pairing);
  	element_init_G1((*pub)->gd21, 	(*pub)->pairing);
	element_init_G1((*pub)->gd22, 	(*pub)->pairing);
  	element_init_G1((*pub)->gd23, 	(*pub)->pairing);
	element_init_G1((*pub)->gd24, 	(*pub)->pairing);
  	element_pow_zn((*pub)->gd11, (*pub)->g1, dpvs->d11); 
	element_pow_zn((*pub)->gd12, (*pub)->g1, dpvs->d12);
  	element_pow_zn((*pub)->gd13, (*pub)->g1, dpvs->d13); 
	element_pow_zn((*pub)->gd14, (*pub)->g1, dpvs->d14);
  	element_pow_zn((*pub)->gd21, (*pub)->g1, dpvs->d21); 
	element_pow_zn((*pub)->gd22, (*pub)->g1, dpvs->d22);
  	element_pow_zn((*pub)->gd23, (*pub)->g1, dpvs->d23); 
	element_pow_zn((*pub)->gd24, (*pub)->g1, dpvs->d24);
  	//e(g1, g2)^alpha d1 D1
  	//we already know that d1*D1 equals to the variable 'dpvs->one', anyway, lets calculate d1*D1 = d11*D11+d12*D12+d13*D13+d14*D14
	element_t t1, t2, t3, t4;
 	element_init_Zr(t1, (*pub)->pairing); element_init_Zr(t2, (*pub)->pairing);
 	element_init_Zr(t3, (*pub)->pairing); element_init_Zr(t4, (*pub)->pairing);
  	element_t R; // R should be d1*D1
  	element_init_Zr(R, 	(*pub)->pairing);
  	element_mul(t1, dpvs->d11, dpvs->D11); 
	element_mul(t2, dpvs->d21, dpvs->D21); 
	element_add(t3, t1, t2);
  	element_mul(t1, dpvs->d31, dpvs->D31); 
	element_mul(t2, dpvs->d41, dpvs->D41); element_add(t4, t1, t2);
  	element_add(R, t3, t4);
  	element_t eg1g2, alpha_R; //alpha_R = alpha*d1*D1
  	element_init_Zr(alpha_R, 	(*pub)->pairing);
  	element_init_GT((*pub)->PP2, 	(*pub)->pairing);
 	element_init_GT(eg1g2, 		(*pub)->pairing);
  	element_pairing(eg1g2, (*pub)->g1, (*pub)->g2);
  	element_mul(alpha_R, (*msk)->alpha, R);
  	element_pow_zn((*pub)->PP2, eg1g2, alpha_R);
  	//element_printf("-PP2 = %B\n", PP2);
  	//printf("100%% Complete\n\n");	

  	//generate MK, g2^D1, g2^D2
  	element_init_G2((*msk)->gD11, 	(*pub)->pairing);
	element_init_G2((*msk)->gD12, 	(*pub)->pairing);
  	element_init_G2((*msk)->gD13, 	(*pub)->pairing);
	element_init_G2((*msk)->gD14, 	(*pub)->pairing);
  	element_init_G2((*msk)->gD21, 	(*pub)->pairing);
	element_init_G2((*msk)->gD22, 	(*pub)->pairing);
  	element_init_G2((*msk)->gD23, 	(*pub)->pairing);
	element_init_G2((*msk)->gD24, 	(*pub)->pairing);
  	element_pow_zn((*msk)->gD11, (*pub)->g2, dpvs->D11); 
	element_pow_zn((*msk)->gD12, (*pub)->g2, dpvs->D12);
  	element_pow_zn((*msk)->gD13, (*pub)->g2, dpvs->D13); 
	element_pow_zn((*msk)->gD14, (*pub)->g2, dpvs->D14);
  	element_pow_zn((*msk)->gD21, (*pub)->g2, dpvs->D21); 
	element_pow_zn((*msk)->gD22, (*pub)->g2, dpvs->D22);
  	element_pow_zn((*msk)->gD23, (*pub)->g2, dpvs->D23); 
	element_pow_zn((*msk)->gD24, (*pub)->g2, dpvs->D24);
  	printf("setup 100%% Complete\n\n");
}

w12ibe_DPVS* w12ibe_DPVS_gen(w12ibe_pub_params* pub)
{
	w12ibe_DPVS* dpvs;
	dpvs = malloc(sizeof(w12ibe_DPVS));
  	/*Generate a real dual space vector spaces:e E
  	the following code are generating the dual pairing vector space. 
	One has to notice that I hard coded all of them, 
	which means the following code can only generate two 4*4 matrix.
	If a 5*5 matrix is in need. All the code below should be rewrite.
  	to be more specific, Gaussian elimination should be done again. 
	*/
  	
  	element_init_Zr(dpvs->one, pub->pairing);
  	//element_set1(dpvs->one);
  	element_random(dpvs->one);
  	element_init_Zr(dpvs->D11,pub->pairing);element_init_Zr(dpvs->D12,pub->pairing);
	element_init_Zr(dpvs->D13,pub->pairing);element_init_Zr(dpvs->D14,pub->pairing);
	element_init_Zr(dpvs->D21,pub->pairing);element_init_Zr(dpvs->D22,pub->pairing);
  	element_init_Zr(dpvs->D23,pub->pairing);element_init_Zr(dpvs->D24,pub->pairing);
	element_init_Zr(dpvs->D31,pub->pairing);element_init_Zr(dpvs->D32,pub->pairing);
	element_init_Zr(dpvs->D33,pub->pairing);element_init_Zr(dpvs->D34,pub->pairing);
  	element_init_Zr(dpvs->D41,pub->pairing);element_init_Zr(dpvs->D42,pub->pairing);
	element_init_Zr(dpvs->D43,pub->pairing);element_init_Zr(dpvs->D44,pub->pairing);
  	element_init_Zr(dpvs->d11,pub->pairing);element_init_Zr(dpvs->d12,pub->pairing);
	element_init_Zr(dpvs->d13,pub->pairing);element_init_Zr(dpvs->d14,pub->pairing);
	element_init_Zr(dpvs->d21,pub->pairing);element_init_Zr(dpvs->d22,pub->pairing);
  	element_init_Zr(dpvs->d23,pub->pairing);element_init_Zr(dpvs->d24,pub->pairing);
	element_init_Zr(dpvs->d31,pub->pairing);element_init_Zr(dpvs->d32,pub->pairing);
	element_init_Zr(dpvs->d33,pub->pairing);element_init_Zr(dpvs->d34,pub->pairing);
  	element_init_Zr(dpvs->d41,pub->pairing);element_init_Zr(dpvs->d42,pub->pairing);
	element_init_Zr(dpvs->d43,pub->pairing);element_init_Zr(dpvs->d44,pub->pairing);
  	element_random(dpvs->d11);element_random(dpvs->d12);
	element_random(dpvs->d13);element_random(dpvs->d14);
  	element_random(dpvs->d21);element_random(dpvs->d22);
	element_random(dpvs->d23);element_random(dpvs->d24);
  	element_random(dpvs->d31);element_random(dpvs->d32);
	element_random(dpvs->d33);element_random(dpvs->d34);
  	element_random(dpvs->d41);element_random(dpvs->d42);
	element_random(dpvs->d43);element_random(dpvs->d44);
  	//Calculate E11, E21, E31, E41
  	element_t f1,f2,f3,f4,f5,f6,f7,f8,f9;
  	element_init_Zr(f1,pub->pairing);element_init_Zr(f2,pub->pairing);
	element_init_Zr(f3,pub->pairing);element_init_Zr(f4,pub->pairing);
	element_init_Zr(f5,pub->pairing);element_init_Zr(f6,pub->pairing);
  	element_init_Zr(f7,pub->pairing);element_init_Zr(f8,pub->pairing);
	element_init_Zr(f9,pub->pairing);
  	element_t t1, t2, t3, t4;
  	element_init_Zr(t1, pub->pairing); element_init_Zr(t2, pub->pairing);
  	element_init_Zr(t3, pub->pairing); element_init_Zr(t4, pub->pairing);
  	element_mul(t1, dpvs->d22, dpvs->d11); element_div(t2, t1, dpvs->d12); element_sub(f1, dpvs->d21, t2);
  	element_mul(t1, dpvs->d32, dpvs->d11); element_div(t2, t1, dpvs->d12); element_sub(f2, dpvs->d31, t2);
  	element_mul(t1, dpvs->d42, dpvs->d11); element_div(t2, t1, dpvs->d12); element_sub(f3, dpvs->d41, t2);
  	element_mul(t1, dpvs->d23, dpvs->d11); element_div(t2, t1, dpvs->d13); element_sub(f4, dpvs->d21, t2);
  	element_mul(t1, dpvs->d33, dpvs->d11); element_div(t2, t1, dpvs->d13); element_sub(f5, dpvs->d31, t2);
  	element_mul(t1, dpvs->d43, dpvs->d11); element_div(t2, t1, dpvs->d13); element_sub(f6, dpvs->d41, t2);
  	element_mul(t1, dpvs->d24, dpvs->d11); element_div(t2, t1, dpvs->d14); element_sub(f7, dpvs->d21, t2);
  	element_mul(t1, dpvs->d34, dpvs->d11); element_div(t2, t1, dpvs->d14); element_sub(f8, dpvs->d31, t2);
  	element_mul(t1, dpvs->d44, dpvs->d11); element_div(t2, t1, dpvs->d14); element_sub(f9, dpvs->d41, t2);
  	element_t h1, h2, h3, h4, h5, h6;
  	element_init_Zr(h1,pub->pairing);element_init_Zr(h2,pub->pairing);
	element_init_Zr(h3,pub->pairing);element_init_Zr(h4,pub->pairing);
	element_init_Zr(h5,pub->pairing);element_init_Zr(h6,pub->pairing);
  	element_div(t1, f1, f4); element_mul(t2, t1, f5); element_sub(h1, f2, t2);
  	element_div(t1, f1, f4); element_mul(t2, t1, f6); element_sub(h2, f3, t2);
  	element_div(t1, f1, f7); element_mul(t2, t1, f8); element_sub(h3, f2, t2);
  	element_div(t1, f1, f7); element_mul(t2, t1, f9); element_sub(h4, f3, t2);
  	element_div(t1, f1, f4); element_mul(t2, dpvs->one, t1); element_sub(h5, dpvs->one, t2);
  	element_div(t1, f1, f7); element_mul(t2, dpvs->one, t1); element_sub(h6, dpvs->one, t2);
  	element_div(t1, h1, h3); element_mul(t2, t1, h6); element_sub(t3, h5, t2);
  	element_div(t1, h1, h3); element_mul(t2, t1, h4); element_sub(t4, h2, t2);
  	element_div(dpvs->D41, t3, t4);
  	element_mul(t1, h2, dpvs->D41);element_sub(t2, h5, t1);element_div(dpvs->D31, t2, h1);
  	element_mul(t1, f2, dpvs->D31); element_mul(t2, f3, dpvs->D41); element_sub(t3, dpvs->one, t1);
  	element_sub(t4, t3, t2); element_div(dpvs->D21, t4, f1);
  	element_mul(t1, dpvs->D21, dpvs->d21); element_sub(t2, dpvs->one, t1);
  	element_mul(t1, dpvs->D31, dpvs->d31); element_sub(t3, t2, t1);
  	element_mul(t1, dpvs->D41, dpvs->d41); element_sub(t4, t3, t1);
  	element_div(dpvs->D11, t4, dpvs->d11);
  	//Calculate E12, E22, E32, E42
  	element_mul(t1, dpvs->d21, dpvs->d12); element_div(t2, t1, dpvs->d11); element_sub(f1, dpvs->d22, t2);
  	element_mul(t1, dpvs->d31, dpvs->d12); element_div(t2, t1, dpvs->d11); element_sub(f2, dpvs->d32, t2);
  	element_mul(t1, dpvs->d41, dpvs->d12); element_div(t2, t1, dpvs->d11); element_sub(f3, dpvs->d42, t2);
  	element_mul(t1, dpvs->d23, dpvs->d12); element_div(t2, t1, dpvs->d13); element_sub(f4, dpvs->d22, t2);
  	element_mul(t1, dpvs->d33, dpvs->d12); element_div(t2, t1, dpvs->d13); element_sub(f5, dpvs->d32, t2);
  	element_mul(t1, dpvs->d43, dpvs->d12); element_div(t2, t1, dpvs->d13); element_sub(f6, dpvs->d42, t2);
  	element_mul(t1, dpvs->d24, dpvs->d12); element_div(t2, t1, dpvs->d14); element_sub(f7, dpvs->d22, t2);
  	element_mul(t1, dpvs->d34, dpvs->d12); element_div(t2, t1, dpvs->d14); element_sub(f8, dpvs->d32, t2);
  	element_mul(t1, dpvs->d44, dpvs->d12); element_div(t2, t1, dpvs->d14); element_sub(f9, dpvs->d42, t2);
  	element_div(t1, f1, f4); element_mul(t2, t1, f5); element_sub(h1, f2, t2);
  	element_div(t1, f1, f4); element_mul(t2, t1, f6); element_sub(h2, f3, t2);
  	element_div(t1, f1, f7); element_mul(t2, t1, f8); element_sub(h3, f2, t2);
  	element_div(t1, f1, f7); element_mul(t2, t1, f9); element_sub(h4, f3, t2);
 	element_div(t1, f1, f4); element_mul(t2, dpvs->one, t1); element_sub(h5, dpvs->one, t2);
  	element_div(t1, f1, f7); element_mul(t2, dpvs->one, t1); element_sub(h6, dpvs->one, t2);
  	element_div(t1, h1, h3); element_mul(t2, t1, h6); element_sub(t3, h5, t2);
  	element_div(t1, h1, h3); element_mul(t2, t1, h4); element_sub(t4, h2, t2);
  	element_div(dpvs->D42, t3, t4);
  	element_mul(t1, h2, dpvs->D42); element_sub(t2, h5, t1); element_div(dpvs->D32, t2, h1);
  	element_mul(t1, f2, dpvs->D32); element_mul(t2, f3, dpvs->D42); element_sub(t3, dpvs->one, t1);
 	 element_sub(t4, t3, t2); element_div(dpvs->D22, t4, f1);
  	element_mul(t1, dpvs->D22, dpvs->d22); element_sub(t2, dpvs->one, t1);
  	element_mul(t1, dpvs->D32, dpvs->d32); element_sub(t3, t2, t1);
  	element_mul(t1, dpvs->D42, dpvs->d42); element_sub(t4, t3, t1);
  	element_div(dpvs->D12, t4, dpvs->d12);
 	//Calculate E13, E23, E33, E43
  	element_mul(t1, dpvs->d21, dpvs->d13); element_div(t2, t1, dpvs->d11); element_sub(f1, dpvs->d23, t2);
  	element_mul(t1, dpvs->d31, dpvs->d13); element_div(t2, t1, dpvs->d11); element_sub(f2, dpvs->d33, t2);
  	element_mul(t1, dpvs->d41, dpvs->d13); element_div(t2, t1, dpvs->d11); element_sub(f3, dpvs->d43, t2);
  	element_mul(t1, dpvs->d22, dpvs->d13); element_div(t2, t1, dpvs->d12); element_sub(f4, dpvs->d23, t2);
  	element_mul(t1, dpvs->d32, dpvs->d13); element_div(t2, t1, dpvs->d12); element_sub(f5, dpvs->d33, t2);
  	element_mul(t1, dpvs->d42, dpvs->d13); element_div(t2, t1, dpvs->d12); element_sub(f6, dpvs->d43, t2);
  	element_mul(t1, dpvs->d24, dpvs->d13); element_div(t2, t1, dpvs->d14); element_sub(f7, dpvs->d23, t2);
  	element_mul(t1, dpvs->d34, dpvs->d13); element_div(t2, t1, dpvs->d14); element_sub(f8, dpvs->d33, t2);
  	element_mul(t1, dpvs->d44, dpvs->d13); element_div(t2, t1, dpvs->d14); element_sub(f9, dpvs->d43, t2);
  	element_div(t1, f1, f4); element_mul(t2, t1, f5); element_sub(h1, f2, t2);
  	element_div(t1, f1, f4); element_mul(t2, t1, f6); element_sub(h2, f3, t2);
  	element_div(t1, f1, f7); element_mul(t2, t1, f8); element_sub(h3, f2, t2);
  	element_div(t1, f1, f7); element_mul(t2, t1, f9); element_sub(h4, f3, t2);
  	element_div(t1, f1, f4); element_mul(t2, dpvs->one, t1); element_sub(h5, dpvs->one, t2);
  	element_div(t1, f1, f7); element_mul(t2, dpvs->one, t1); element_sub(h6, dpvs->one, t2);
  	element_div(t1, h1, h3); element_mul(t2, t1, h6); element_sub(t3, h5, t2);
  	element_div(t1, h1, h3); element_mul(t2, t1, h4); element_sub(t4, h2, t2);
  	element_div(dpvs->D43, t3, t4);
  	element_mul(t1, h2, dpvs->D43); element_sub(t2, h5, t1); element_div(dpvs->D33, t2, h1);
  	element_mul(t1, f2, dpvs->D33); element_mul(t2, f3, dpvs->D43); element_sub(t3, dpvs->one, t1);
 	element_sub(t4, t3, t2); element_div(dpvs->D23, t4, f1);
  	element_mul(t1, dpvs->D23, dpvs->d23); element_sub(t2, dpvs->one, t1);
  	element_mul(t1, dpvs->D33, dpvs->d33); element_sub(t3, t2, t1);
  	element_mul(t1, dpvs->D43, dpvs->d43); element_sub(t4, t3, t1);
  	element_div(dpvs->D13, t4, dpvs->d13);
  	//Calculate E14, E24, E34, E44
  	element_mul(t1, dpvs->d21, dpvs->d14); element_div(t2, t1, dpvs->d11); element_sub(f1, dpvs->d24, t2);
 	 element_mul(t1, dpvs->d31, dpvs->d14); element_div(t2, t1, dpvs->d11); element_sub(f2, dpvs->d34, t2);
  	element_mul(t1, dpvs->d41, dpvs->d14); element_div(t2, t1, dpvs->d11); element_sub(f3, dpvs->d44, t2);
  	element_mul(t1, dpvs->d22, dpvs->d14); element_div(t2, t1, dpvs->d12); element_sub(f4, dpvs->d24, t2);
  	element_mul(t1, dpvs->d32, dpvs->d14); element_div(t2, t1, dpvs->d12); element_sub(f5, dpvs->d34, t2);
  	element_mul(t1, dpvs->d42, dpvs->d14); element_div(t2, t1, dpvs->d12); element_sub(f6, dpvs->d44, t2);
  	element_mul(t1, dpvs->d23, dpvs->d14); element_div(t2, t1, dpvs->d13); element_sub(f7, dpvs->d24, t2);
  	element_mul(t1, dpvs->d33, dpvs->d14); element_div(t2, t1, dpvs->d13); element_sub(f8, dpvs->d34, t2);
  	element_mul(t1, dpvs->d43, dpvs->d14); element_div(t2, t1, dpvs->d13); element_sub(f9, dpvs->d44, t2);
  	element_div(t1, f1, f4); element_mul(t2, t1, f5); element_sub(h1, f2, t2);
  	element_div(t1, f1, f4); element_mul(t2, t1, f6); element_sub(h2, f3, t2);
  	element_div(t1, f1, f7); element_mul(t2, t1, f8); element_sub(h3, f2, t2);
  	element_div(t1, f1, f7); element_mul(t2, t1, f9); element_sub(h4, f3, t2);
  	element_div(t1, f1, f4); element_mul(t2, dpvs->one, t1); element_sub(h5, dpvs->one, t2);
  	element_div(t1, f1, f7); element_mul(t2, dpvs->one, t1); element_sub(h6, dpvs->one, t2);
  	element_div(t1, h1, h3); element_mul(t2, t1, h6); element_sub(t3, h5, t2);
  	element_div(t1, h1, h3); element_mul(t2, t1, h4); element_sub(t4, h2, t2);
  	element_div(dpvs->D44, t3, t4);
  	element_mul(t1, h2, dpvs->D44); element_sub(t2, h5, t1); element_div(dpvs->D34, t2, h1);
  	element_mul(t1, f2, dpvs->D34); element_mul(t2, f3, dpvs->D44); element_sub(t3, dpvs->one, t1);
  	element_sub(t4, t3, t2); element_div(dpvs->D24, t4, f1);
  	element_mul(t1, dpvs->D24, dpvs->d24); element_sub(t2, dpvs->one, t1);
 	element_mul(t1, dpvs->D34, dpvs->d34); element_sub(t3, t2, t1);
  	element_mul(t1, dpvs->D44, dpvs->d44); element_sub(t4, t3, t1);
  	element_div(dpvs->D14, t4, dpvs->d14);
  	//the following are codes that test the correctness of dual pairing spaces
  	/*element_t R;
  	element_init_Zr(R, pairing);
  	element_mul(t1, dpvs->d12, dpvs->D11); element_mul(t2, dpvs->d22, dpvs->D21); element_add(t3, t1, t2);
  	element_mul(t1, dpvs->d32, dpvs->D31); element_mul(t2, dpvs->d42, dpvs->D41); element_add(t4, t1, t2);
  	element_add(R, t3, t4);
  	element_printf("R = %B\n, one = %B\n", R, dpvs->one);
  	if (!element_cmp(dpvs->one, R)) {
    	printf("----------Confirm----------------\n");}
  	else{
    	printf("----------damn--------\n");
   	}*/
	return dpvs;
}


