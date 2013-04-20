#include <pbc.h>
#include <pbc_test.h>

int main(int argc, char **argv) {
  pairing_t pairing; //initialize the pairing, d159 MNT curve 

  pbc_demo_pairing_init(pairing, argc, argv);//why use this function???
  
  //generate two identity matrixes.
  //this is a special case to test the dual pairing vector space.
  printf("-Generating the dual pairing vector space:");
  element_t d11, d12, d13, d14, d21, d22, d23, d24, d31, d32, d33, d34, d41, d42, d43, d44;
  element_t D11, D12, D13, D14, D21, D22, D23, D24, D31, D32, D33, D34, D41, D42, D43, D44;
  element_init_Zr(d11,pairing);element_init_Zr(d12,pairing);element_init_Zr(d13,pairing);
  element_init_Zr(d14,pairing);element_init_Zr(d21,pairing);element_init_Zr(d22,pairing);
  element_init_Zr(d23,pairing);element_init_Zr(d24,pairing);element_init_Zr(d31,pairing);
  element_init_Zr(d32,pairing);element_init_Zr(d33,pairing);element_init_Zr(d34,pairing);
  element_init_Zr(d41,pairing);element_init_Zr(d42,pairing);element_init_Zr(d43,pairing);
  element_init_Zr(d44,pairing);
  element_init_Zr(D11,pairing);element_init_Zr(D12,pairing);element_init_Zr(D13,pairing);
  element_init_Zr(D14,pairing);element_init_Zr(D21,pairing);element_init_Zr(D22,pairing);
  element_init_Zr(D23,pairing);element_init_Zr(D24,pairing);element_init_Zr(D31,pairing);
  element_init_Zr(D32,pairing);element_init_Zr(D33,pairing);element_init_Zr(D34,pairing);
  element_init_Zr(D41,pairing);element_init_Zr(D42,pairing);element_init_Zr(D43,pairing);
  element_init_Zr(D44,pairing);
  /*element_set1(d11);element_set1(d22);element_set1(d33);element_set1(d44);
  element_set0(d12);element_set0(d13);element_set0(d14);element_set0(d21);
  element_set0(d23);element_set0(d24);element_set0(d31);element_set0(d32);
  element_set0(d34);element_set0(d41);element_set0(d42);element_set0(d43);
  element_set1(D11);element_set1(D22);element_set1(D33);element_set1(D44);
  element_set0(D12);element_set0(D13);element_set0(D14);element_set0(D21);
  element_set0(D23);element_set0(D24);element_set0(D31);element_set0(D32);
  element_set0(D34);element_set0(D41);element_set0(D42);element_set0(D43);
  printf("----------hard coded DPVS generated, two identity matrix-------------------\n");*/

  //Generate a real dual space vector spaces:e E
  //the following code are generating the dual pairing vector space. One has to notice 
  //that I hard coded all of them, which means the following code can only generate 
  //two 4*4 matrix. If a 5*5 matrix is in need. All the code below should be rewrite.
  //to be more specific, Gaussian elimination should be done again. 
  element_t one;
  element_init_Zr(one, pairing);
  //element_set1(one);
  element_random(one);
  element_t e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;
  element_t E11, E12, E13, E14, E21, E22, E23, E24, E31, E32, E33, E34, E41, E42, E43, E44;
  element_init_Zr(E11,pairing);element_init_Zr(E12,pairing);element_init_Zr(E13,pairing);
  element_init_Zr(E14,pairing);element_init_Zr(E21,pairing);element_init_Zr(E22,pairing);
  element_init_Zr(E23,pairing);element_init_Zr(E24,pairing);element_init_Zr(E31,pairing);
  element_init_Zr(E32,pairing);element_init_Zr(E33,pairing);element_init_Zr(E34,pairing);
  element_init_Zr(E41,pairing);element_init_Zr(E42,pairing);element_init_Zr(E43,pairing);
  element_init_Zr(E44,pairing);
  element_init_Zr(e11,pairing);element_init_Zr(e12,pairing);element_init_Zr(e13,pairing);
  element_init_Zr(e14,pairing);element_init_Zr(e21,pairing);element_init_Zr(e22,pairing);
  element_init_Zr(e23,pairing);element_init_Zr(e24,pairing);element_init_Zr(e31,pairing);
  element_init_Zr(e32,pairing);element_init_Zr(e33,pairing);element_init_Zr(e34,pairing);
  element_init_Zr(e41,pairing);element_init_Zr(e42,pairing);element_init_Zr(e43,pairing);
  element_init_Zr(e44,pairing);
  element_random(e11);element_random(e12);element_random(e13);element_random(e14);
  element_random(e21);element_random(e22);element_random(e23);element_random(e24);
  element_random(e31);element_random(e32);element_random(e33);element_random(e34);
  element_random(e41);element_random(e42);element_random(e43);element_random(e44);
  //Calculate E11, E21, E31, E41
  element_t f1,f2,f3,f4,f5,f6,f7,f8,f9;
  element_init_Zr(f1,pairing);element_init_Zr(f2,pairing);element_init_Zr(f3,pairing);
  element_init_Zr(f4,pairing);element_init_Zr(f5,pairing);element_init_Zr(f6,pairing);
  element_init_Zr(f7,pairing);element_init_Zr(f8,pairing);element_init_Zr(f9,pairing);
  element_t t1, t2, t3, t4;
  element_init_Zr(t1, pairing); element_init_Zr(t2, pairing);
  element_init_Zr(t3, pairing); element_init_Zr(t4, pairing);
  element_mul(t1, e22, e11); element_div(t2, t1, e12); element_sub(f1, e21, t2);
  element_mul(t1, e32, e11); element_div(t2, t1, e12); element_sub(f2, e31, t2);
  element_mul(t1, e42, e11); element_div(t2, t1, e12); element_sub(f3, e41, t2);
  element_mul(t1, e23, e11); element_div(t2, t1, e13); element_sub(f4, e21, t2);
  element_mul(t1, e33, e11); element_div(t2, t1, e13); element_sub(f5, e31, t2);
  element_mul(t1, e43, e11); element_div(t2, t1, e13); element_sub(f6, e41, t2);
  element_mul(t1, e24, e11); element_div(t2, t1, e14); element_sub(f7, e21, t2);
  element_mul(t1, e34, e11); element_div(t2, t1, e14); element_sub(f8, e31, t2);
  element_mul(t1, e44, e11); element_div(t2, t1, e14); element_sub(f9, e41, t2);
  element_t h1, h2, h3, h4, h5, h6;
  element_init_Zr(h1,pairing);element_init_Zr(h2,pairing);element_init_Zr(h3,pairing);
  element_init_Zr(h4,pairing);element_init_Zr(h5,pairing);element_init_Zr(h6,pairing);
  element_div(t1, f1, f4); element_mul(t2, t1, f5); element_sub(h1, f2, t2);
  element_div(t1, f1, f4); element_mul(t2, t1, f6); element_sub(h2, f3, t2);
  element_div(t1, f1, f7); element_mul(t2, t1, f8); element_sub(h3, f2, t2);
  element_div(t1, f1, f7); element_mul(t2, t1, f9); element_sub(h4, f3, t2);
  element_div(t1, f1, f4); element_mul(t2, one, t1); element_sub(h5, one, t2);
  element_div(t1, f1, f7); element_mul(t2, one, t1); element_sub(h6, one, t2);
  element_div(t1, h1, h3); element_mul(t2, t1, h6); element_sub(t3, h5, t2);
  element_div(t1, h1, h3); element_mul(t2, t1, h4); element_sub(t4, h2, t2);
  element_div(E41, t3, t4);
  element_mul(t1, h2, E41); element_sub(t2, h5, t1); element_div(E31, t2, h1);
  element_mul(t1, f2, E31); element_mul(t2, f3, E41); element_sub(t3, one, t1);
  element_sub(t4, t3, t2); element_div(E21, t4, f1);
  element_mul(t1, E21, e21); element_sub(t2, one, t1);
  element_mul(t1, E31, e31); element_sub(t3, t2, t1);
  element_mul(t1, E41, e41); element_sub(t4, t3, t1);
  element_div(E11, t4, e11);
  //Calculate E12, E22, E32, E42
  element_mul(t1, e21, e12); element_div(t2, t1, e11); element_sub(f1, e22, t2);
  element_mul(t1, e31, e12); element_div(t2, t1, e11); element_sub(f2, e32, t2);
  element_mul(t1, e41, e12); element_div(t2, t1, e11); element_sub(f3, e42, t2);
  element_mul(t1, e23, e12); element_div(t2, t1, e13); element_sub(f4, e22, t2);
  element_mul(t1, e33, e12); element_div(t2, t1, e13); element_sub(f5, e32, t2);
  element_mul(t1, e43, e12); element_div(t2, t1, e13); element_sub(f6, e42, t2);
  element_mul(t1, e24, e12); element_div(t2, t1, e14); element_sub(f7, e22, t2);
  element_mul(t1, e34, e12); element_div(t2, t1, e14); element_sub(f8, e32, t2);
  element_mul(t1, e44, e12); element_div(t2, t1, e14); element_sub(f9, e42, t2);
  element_div(t1, f1, f4); element_mul(t2, t1, f5); element_sub(h1, f2, t2);
  element_div(t1, f1, f4); element_mul(t2, t1, f6); element_sub(h2, f3, t2);
  element_div(t1, f1, f7); element_mul(t2, t1, f8); element_sub(h3, f2, t2);
  element_div(t1, f1, f7); element_mul(t2, t1, f9); element_sub(h4, f3, t2);
  element_div(t1, f1, f4); element_mul(t2, one, t1); element_sub(h5, one, t2);
  element_div(t1, f1, f7); element_mul(t2, one, t1); element_sub(h6, one, t2);
  element_div(t1, h1, h3); element_mul(t2, t1, h6); element_sub(t3, h5, t2);
  element_div(t1, h1, h3); element_mul(t2, t1, h4); element_sub(t4, h2, t2);
  element_div(E42, t3, t4);
  element_mul(t1, h2, E42); element_sub(t2, h5, t1); element_div(E32, t2, h1);
  element_mul(t1, f2, E32); element_mul(t2, f3, E42); element_sub(t3, one, t1);
  element_sub(t4, t3, t2); element_div(E22, t4, f1);
  element_mul(t1, E22, e22); element_sub(t2, one, t1);
  element_mul(t1, E32, e32); element_sub(t3, t2, t1);
  element_mul(t1, E42, e42); element_sub(t4, t3, t1);
  element_div(E12, t4, e12);
  //Calculate E13, E23, E33, E43
  element_mul(t1, e21, e13); element_div(t2, t1, e11); element_sub(f1, e23, t2);
  element_mul(t1, e31, e13); element_div(t2, t1, e11); element_sub(f2, e33, t2);
  element_mul(t1, e41, e13); element_div(t2, t1, e11); element_sub(f3, e43, t2);
  element_mul(t1, e22, e13); element_div(t2, t1, e12); element_sub(f4, e23, t2);
  element_mul(t1, e32, e13); element_div(t2, t1, e12); element_sub(f5, e33, t2);
  element_mul(t1, e42, e13); element_div(t2, t1, e12); element_sub(f6, e43, t2);
  element_mul(t1, e24, e13); element_div(t2, t1, e14); element_sub(f7, e23, t2);
  element_mul(t1, e34, e13); element_div(t2, t1, e14); element_sub(f8, e33, t2);
  element_mul(t1, e44, e13); element_div(t2, t1, e14); element_sub(f9, e43, t2);
  element_div(t1, f1, f4); element_mul(t2, t1, f5); element_sub(h1, f2, t2);
  element_div(t1, f1, f4); element_mul(t2, t1, f6); element_sub(h2, f3, t2);
  element_div(t1, f1, f7); element_mul(t2, t1, f8); element_sub(h3, f2, t2);
  element_div(t1, f1, f7); element_mul(t2, t1, f9); element_sub(h4, f3, t2);
  element_div(t1, f1, f4); element_mul(t2, one, t1); element_sub(h5, one, t2);
  element_div(t1, f1, f7); element_mul(t2, one, t1); element_sub(h6, one, t2);
  element_div(t1, h1, h3); element_mul(t2, t1, h6); element_sub(t3, h5, t2);
  element_div(t1, h1, h3); element_mul(t2, t1, h4); element_sub(t4, h2, t2);
  element_div(E43, t3, t4);
  element_mul(t1, h2, E43); element_sub(t2, h5, t1); element_div(E33, t2, h1);
  element_mul(t1, f2, E33); element_mul(t2, f3, E43); element_sub(t3, one, t1);
  element_sub(t4, t3, t2); element_div(E23, t4, f1);
  element_mul(t1, E23, e23); element_sub(t2, one, t1);
  element_mul(t1, E33, e33); element_sub(t3, t2, t1);
  element_mul(t1, E43, e43); element_sub(t4, t3, t1);
  element_div(E13, t4, e13);
  //Calculate E14, E24, E34, E44
  element_mul(t1, e21, e14); element_div(t2, t1, e11); element_sub(f1, e24, t2);
  element_mul(t1, e31, e14); element_div(t2, t1, e11); element_sub(f2, e34, t2);
  element_mul(t1, e41, e14); element_div(t2, t1, e11); element_sub(f3, e44, t2);
  element_mul(t1, e22, e14); element_div(t2, t1, e12); element_sub(f4, e24, t2);
  element_mul(t1, e32, e14); element_div(t2, t1, e12); element_sub(f5, e34, t2);
  element_mul(t1, e42, e14); element_div(t2, t1, e12); element_sub(f6, e44, t2);
  element_mul(t1, e23, e14); element_div(t2, t1, e13); element_sub(f7, e24, t2);
  element_mul(t1, e33, e14); element_div(t2, t1, e13); element_sub(f8, e34, t2);
  element_mul(t1, e43, e14); element_div(t2, t1, e13); element_sub(f9, e44, t2);
  element_div(t1, f1, f4); element_mul(t2, t1, f5); element_sub(h1, f2, t2);
  element_div(t1, f1, f4); element_mul(t2, t1, f6); element_sub(h2, f3, t2);
  element_div(t1, f1, f7); element_mul(t2, t1, f8); element_sub(h3, f2, t2);
  element_div(t1, f1, f7); element_mul(t2, t1, f9); element_sub(h4, f3, t2);
  element_div(t1, f1, f4); element_mul(t2, one, t1); element_sub(h5, one, t2);
  element_div(t1, f1, f7); element_mul(t2, one, t1); element_sub(h6, one, t2);
  element_div(t1, h1, h3); element_mul(t2, t1, h6); element_sub(t3, h5, t2);
  element_div(t1, h1, h3); element_mul(t2, t1, h4); element_sub(t4, h2, t2);
  element_div(E44, t3, t4);
  element_mul(t1, h2, E44); element_sub(t2, h5, t1); element_div(E34, t2, h1);
  element_mul(t1, f2, E34); element_mul(t2, f3, E44); element_sub(t3, one, t1);
  element_sub(t4, t3, t2); element_div(E24, t4, f1);
  element_mul(t1, E24, e24); element_sub(t2, one, t1);
  element_mul(t1, E34, e34); element_sub(t3, t2, t1);
  element_mul(t1, E44, e44); element_sub(t4, t3, t1);
  element_div(E14, t4, e14);
  //the following are codes that test the correctness of dual pairing spaces
  /*element_t R;
  element_init_Zr(R, pairing);
  element_mul(t1, e12, E11); element_mul(t2, e22, E21); element_add(t3, t1, t2);
  element_mul(t1, e32, E31); element_mul(t2, e42, E41); element_add(t4, t1, t2);
  element_add(R, t3, t4);
  element_printf("R = %B\n, one = %B\n", R, one);
  if (!element_cmp(one, R)) {
    printf("----------Confirm----------------\n");}
  else{
    printf("----------damn--------\n");
   }*/
  // d=e, D=E
  element_set(d11, e11);element_set(d12, e12);element_set(d13, e13);
  element_set(d14, e14);element_set(d21, e21);element_set(d22, e22);
  element_set(d23, e23);element_set(d24, e24);element_set(d31, e31);
  element_set(d32, e32);element_set(d33, e33);element_set(d34, e34);
  element_set(d41, e41);element_set(d42, e42);element_set(d43, e43);
  element_set(d44, e44);
  element_set(D11, E11);element_set(D12, E12);element_set(D13, E13);
  element_set(D14, E14);element_set(D21, E21);element_set(D22, E22);
  element_set(D23, E23);element_set(D24, E24);element_set(D31, E31);
  element_set(D32, E32);element_set(D33, E33);element_set(D34, E34);
  element_set(D41, E41);element_set(D42, E42);element_set(D43, E43);
  element_set(D44, E44);
  printf("100%% Complete\n\n");

  //Its time to generate other public parameters
  printf("-Generating public parameters:");
  element_t g1, g2, alpha;
  element_init_G1(g1, pairing);
  element_init_G2(g2, pairing);
  element_init_Zr(alpha, pairing);
  element_random(g1);
  element_printf("\n-g1 = %B\n", g1);
  element_random(g2);
  element_printf("-g2 = %B\n", g2);
  element_random(alpha);
  element_printf("-alpha = %B\n", alpha);
  // g1^d1, g1^d2
  element_t gd11, gd12, gd13, gd14, gd21, gd22, gd23, gd24;
  element_init_G1(gd11, pairing);element_init_G1(gd12, pairing);
  element_init_G1(gd13, pairing);element_init_G1(gd14, pairing);
  element_init_G1(gd21, pairing);element_init_G1(gd22, pairing);
  element_init_G1(gd23, pairing);element_init_G1(gd24, pairing);
  element_pow_zn(gd11, g1, d11); element_pow_zn(gd12, g1, d12);
  element_pow_zn(gd13, g1, d13); element_pow_zn(gd14, g1, d14);
  element_pow_zn(gd21, g1, d21); element_pow_zn(gd22, g1, d22);
  element_pow_zn(gd23, g1, d23); element_pow_zn(gd24, g1, d24);
  //e(g1, g2)^alpha d1 D1
  //we already know that d1*D1 equals to the variable 'one', anyway, lets calculate d1*D1 = d11*D11+d12*D12+d13*D13+d14*D14
  element_t R; // R should be d1*D1
  element_init_Zr(R, pairing);
  element_mul(t1, e11, E11); element_mul(t2, e21, E21); element_add(t3, t1, t2);
  element_mul(t1, e31, E31); element_mul(t2, e41, E41); element_add(t4, t1, t2);
  element_add(R, t3, t4);
  element_t PP2, eg1g2, alpha_R; //alpha_R = alpha*d1*D1
  element_init_Zr(alpha_R, pairing);
  element_init_GT(PP2, pairing);
  element_init_GT(eg1g2, pairing);
  element_pairing(eg1g2, g1, g2);
  element_mul(alpha_R, alpha, R);
  element_pow_zn(PP2, eg1g2, alpha_R);
  element_printf("-PP2 = %B\n", PP2);
  printf("100%% Complete\n\n");
  
  printf("-Generating private parameters:");
  //generate MK, g2^D1, g2^D2
  element_t gD11, gD12, gD13, gD14, gD21, gD22, gD23, gD24;
  element_init_G2(gD11, pairing);element_init_G2(gD12, pairing);
  element_init_G2(gD13, pairing);element_init_G2(gD14, pairing);
  element_init_G2(gD21, pairing);element_init_G2(gD22, pairing);
  element_init_G2(gD23, pairing);element_init_G2(gD24, pairing);
  element_pow_zn(gD11, g2, D11); element_pow_zn(gD12, g2, D12);
  element_pow_zn(gD13, g2, D13); element_pow_zn(gD14, g2, D14);
  element_pow_zn(gD21, g2, D21); element_pow_zn(gD22, g2, D22);
  element_pow_zn(gD23, g2, D23); element_pow_zn(gD24, g2, D24);
  printf("100%% Complete\n\n");
  
  printf("-Generate SK_ID:");
  //generate SK_ID
  element_t r, ID, SK_ID1, SK_ID2, SK_ID3, SK_ID4, SK_ID_top, SK_ID_bot, SK_temp1, SK_temp2;
  element_init_Zr(r, pairing);
  element_random(r);
  element_init_Zr(ID, pairing);
  element_random(ID);
  element_init_Zr(SK_temp1, pairing);
  element_init_Zr(SK_temp2, pairing);
  element_init_G2(SK_ID1, pairing);element_init_G2(SK_ID2, pairing);
  element_init_G2(SK_ID3, pairing);element_init_G2(SK_ID4, pairing);
  element_init_G2(SK_ID_top, pairing);element_init_G2(SK_ID_bot, pairing);
  element_mul(SK_temp1, r, ID);
  element_add(SK_temp2, alpha, SK_temp1);
  element_pow_zn(SK_ID_top, gD11, SK_temp2);
  element_pow_zn(SK_ID_bot, gD21, r);
  element_div(SK_ID1, SK_ID_top, SK_ID_bot);
  element_pow_zn(SK_ID_top, gD12, SK_temp2);
  element_pow_zn(SK_ID_bot, gD22, r);
  element_div(SK_ID2, SK_ID_top, SK_ID_bot);
  element_pow_zn(SK_ID_top, gD13, SK_temp2);
  element_pow_zn(SK_ID_bot, gD23, r);
  element_div(SK_ID3, SK_ID_top, SK_ID_bot);
  element_pow_zn(SK_ID_top, gD14, SK_temp2);
  element_pow_zn(SK_ID_bot, gD24, r);
  element_div(SK_ID4, SK_ID_top, SK_ID_bot);
  //generate SK_ID_prime
  //for test purpuse, the following code are used to verify the correctness of SK
  /*element_t r_ID, alpha_r_ID, SK_ID_1e, SK_ID_2e, SK_ID_3e, SK_ID_4e;
  element_t SK_ID1p, SK_ID2p, SK_ID3p, SK_ID4p;
  element_init_G2(SK_ID1p, pairing);
  element_init_G2(SK_ID2p, pairing);
  element_init_G2(SK_ID3p, pairing);
  element_init_G2(SK_ID4p, pairing);
  element_init_Zr(r_ID, pairing);
  element_mul(r_ID, r, ID);
  element_init_Zr(alpha_r_ID, pairing);
  element_add(alpha_r_ID, alpha, r_ID);
  element_t SKel, SKer;
  element_init_Zr(SKel, pairing);
  element_init_Zr(SKer, pairing);
  element_init_Zr(SK_ID_1e, pairing);
  element_mul(SKel, D11, alpha_r_ID);
  element_mul(SKer, D21, r);
  element_sub(SK_ID_1e, SKel, SKer);
  element_pow_zn(SK_ID1p, g2, SK_ID_1e);
  element_init_Zr(SK_ID_2e, pairing);
  element_mul(SKel, D12, alpha_r_ID);
  element_mul(SKer, D22, r);
  element_sub(SK_ID_2e, SKel, SKer);
  element_pow_zn(SK_ID2p, g2, SK_ID_2e);
  element_init_Zr(SK_ID_3e, pairing);
  element_mul(SKel, D13, alpha_r_ID);
  element_mul(SKer, D23, r);
  element_sub(SK_ID_3e, SKel, SKer);
  element_pow_zn(SK_ID3p, g2, SK_ID_3e);
  element_init_Zr(SK_ID_4e, pairing);
  element_mul(SKel, D14, alpha_r_ID);
  element_mul(SKer, D24, r);
  element_sub(SK_ID_4e, SKel, SKer);
  element_pow_zn(SK_ID4p, g2, SK_ID_4e);
  
   if (!element_cmp(SK_ID1, SK_ID1p)) {
    printf("----------Same----------------\n");}
  else{
    printf("----------Different--------\n");
   } 
   if (!element_cmp(SK_ID2, SK_ID2p)) {
    printf("----------Same----------------\n");}
  else{
    printf("----------Different--------\n");
   } 
   if (!element_cmp(SK_ID3, SK_ID3p)) {
    printf("----------Same----------------\n");}
  else{
    printf("----------Different--------\n");
   } 
   if (!element_cmp(SK_ID4, SK_ID4p)) {
    printf("----------Same----------------\n");}
  else{
    printf("----------Different--------\n");
   } 
  */
  printf("100%% Complete\n\n");

  printf("Encrypt a message M:");
  //Encrypt Message M
  element_t M, s, s_ID, C0, C0_left;
  element_init_Zr(s, pairing);
  element_init_Zr(s_ID, pairing);
  element_random(s);
  element_mul(s_ID, s, ID);
  element_init_GT(M, pairing);
  element_random(M);
  element_printf("M = %B\n", M);
  element_init_GT(C0, pairing);
  element_init_GT(C0_left, pairing);
  element_pow_zn(C0_left, PP2, s);
  element_mul(C0, M, C0_left);
  element_printf("\n-C0 = %B\n", C0);
  element_t Ctemp1, Ctemp2, C11, C12, C13, C14;
  element_init_G1(Ctemp1, pairing);
  element_init_G1(Ctemp2, pairing);
  element_init_G1(C11, pairing);
  element_init_G1(C12, pairing);
  element_init_G1(C13, pairing);
  element_init_G1(C14, pairing);
  element_pow_zn(Ctemp1, gd11, s);
  element_pow_zn(Ctemp2, gd21, s_ID);
  element_mul(C11, Ctemp1, Ctemp2);
  element_pow_zn(Ctemp1, gd12, s);
  element_pow_zn(Ctemp2, gd22, s_ID);
  element_mul(C12, Ctemp1, Ctemp2);
  element_pow_zn(Ctemp1, gd13, s);
  element_pow_zn(Ctemp2, gd23, s_ID);
  element_mul(C13, Ctemp1, Ctemp2);
  element_pow_zn(Ctemp1, gd14, s);
  element_pow_zn(Ctemp2, gd24, s_ID);
  element_mul(C14, Ctemp1, Ctemp2);
  //generate C1_prime
  //the following code are for test purpose, verify C1.
  /*element_t C11p, C12p, C13p, C14p;
  element_init_G1(C11p, pairing);
  element_init_G1(C12p, pairing);
  element_init_G1(C13p, pairing);
  element_init_G1(C14p, pairing);
  element_t C1l, C1r, C1e;
  element_init_Zr(C1l, pairing);
  element_init_Zr(C1r, pairing);
  element_init_Zr(C1e, pairing);
  element_mul(C1l, d11, s);
  element_mul(C1r, d21, s_ID);
  element_add(C1e, C1r, C1l);
  element_pow_zn(C11p, g1, C1e);
  element_mul(C1l, d12, s);
  element_mul(C1r, d22, s_ID);
  element_add(C1e, C1r, C1l);
  element_pow_zn(C12p, g1, C1e);
  element_mul(C1l, d13, s);
  element_mul(C1r, d23, s_ID);
  element_add(C1e, C1r, C1l);
  element_pow_zn(C13p, g1, C1e);
  element_mul(C1l, d14, s);
  element_mul(C1r, d24, s_ID);
  element_add(C1e, C1r, C1l);
  element_pow_zn(C14p, g1, C1e);
  if (!element_cmp(C11, C11p)) {
    printf("----------Same----------------\n");}
  else{
    printf("----------Different--------\n");
   } 
  if (!element_cmp(C12, C12p)) {
    printf("----------Same----------------\n");}
  else{
    printf("----------Different--------\n");
   } 
  if (!element_cmp(C13, C13p)) {
    printf("----------Same----------------\n");}
  else{
    printf("----------Different--------\n");
   }  
  if (!element_cmp(C14, C14p)) {
    printf("----------Same----------------\n");}
  else{
    printf("----------Different--------\n");
   } */
  printf("100%% Complete\n\n");

  printf("-Decrypt the message:");
  //Decrypt Message M'
  element_t Dec1, Dec2, Dec3, Dec4, Dec_bot12, Dec_bot123, Dec_bot1234;
  element_init_GT(Dec1, pairing);
  element_init_GT(Dec2, pairing);
  element_init_GT(Dec3, pairing);
  element_init_GT(Dec4, pairing);
  element_init_GT(Dec_bot12, pairing);
  element_init_GT(Dec_bot123, pairing);
  element_init_GT(Dec_bot1234, pairing);
  element_pairing(Dec1, C11, SK_ID1);
  element_pairing(Dec2, C12, SK_ID2);
  element_pairing(Dec3, C13, SK_ID3);
  element_pairing(Dec4, C14, SK_ID4);
  element_mul(Dec_bot12, Dec1, Dec2);
  element_mul(Dec_bot123, Dec_bot12, Dec3);
  element_mul(Dec_bot1234, Dec_bot123, Dec4);

  element_t M_prime;
  element_init_GT(M_prime, pairing);
  element_div(M_prime, C0, Dec_bot1234);
  //Assume M_prime is canonical. I don't know actually, lets just assume and I'll look into it if I have chance.
  element_printf("M prime = %B\n", M_prime);

  if (!element_cmp(M, M_prime)) {
    printf("Dec Success\n");}
  else{
    printf("Dec Fail\n");
  } 
  
  
  return 0;
}

