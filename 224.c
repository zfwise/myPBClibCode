#include <pbc.h>
#include <pbc_test.h>

int main(int argc, char **argv) {
  pairing_t pairing; //initialize the pairing, d159 MNT curve 
  element_t g_1, g_2; // g_1 is the generator in G1, g_2 is the generator in G2
  element_t d_1, d_2, D_1, D_2; //d_1*D_1 = d_2*D_2
  element_t alpha;
  element_t PP2, PP3, PP4;
  element_t inverse_test;

  pbc_demo_pairing_init(pairing, argc, argv);//why use this function???
  element_init_G1(g_1, pairing);
  element_init_G2(g_2, pairing);
  element_init_GT(PP2, pairing);
  element_init_G1(PP3, pairing);
  element_init_G1(PP4, pairing);
  element_init_Zr(alpha, pairing);
  element_init_Zr(d_1, pairing);
  element_init_Zr(d_2, pairing);
  element_init_Zr(D_1, pairing);
  element_init_Zr(D_2, pairing);
  element_init_Zr(inverse_test, pairing);

  printf("Generating PP\n");
  //generate public parameters:
  element_random(g_1);
  element_printf("g_1 = %B\n",g_1);
  element_random(g_2);
  element_printf("g_2 = %B\n",g_2);
  element_random(alpha);
  element_printf("alpha = %B\n",alpha);
  //element_random(d_1);
  //element_random(d_2);
  //element_invert(D_1, d_1);
  //element_invert(D_2, d_2);
  //test the inverse
  //element_mul(inverse_test, D_1, d_1);
  //element_printf("test the inverse = %B\n", inverse_test);
  //we need to find the dual pairing vector space.
  element_t rho, cross1, cross2, thezero;
  element_init_Zr(rho, pairing);
  element_init_Zr(cross1, pairing);
  element_init_Zr(cross2, pairing);
  element_init_Zr(thezero, pairing);
  element_random(d_1);
  element_random(d_2);
  element_set0(thezero);
  while(1){
  element_random(D_1);
  element_printf("testing: %B\n",D_1);
  element_mul(rho, d_1, D_1);
  element_div(D_2, rho, d_2);
  element_mul(cross1, d_1, D_2);
  element_mul(cross2, d_2, D_1);
    if(!element_cmp(cross1, thezero)){
      printf("found\n"); 
      break;
    }
  }
  element_printf("d_1 = %B\n", d_1);
  element_printf("d_2 = %B\n", d_2);
  element_printf("D_1 = %B\n", D_1);
  element_printf("D_2 = %B\n", D_2);

  //calculate PP2
  element_t d_1_D_1, d_1_D_1_alpha, e_g_1_g_2;
  element_init_Zr(d_1_D_1, pairing);
  element_init_Zr(d_1_D_1_alpha, pairing);
  element_init_GT(e_g_1_g_2, pairing);
  element_mul(d_1_D_1, d_1, D_1);
  element_mul(d_1_D_1_alpha, d_1_D_1, alpha);
  element_pairing(e_g_1_g_2, g_1, g_2);
  element_pow_zn(PP2, e_g_1_g_2, d_1_D_1_alpha);
  element_t testPP2;
  element_init_GT(testPP2, pairing);
  element_pow_zn(testPP2, PP2, alpha);
  element_printf("PP2 = %B\n", PP2);
  element_printf("PP2test = %B\n", testPP2);
  //calculate g_1^d_1 g_1^d_2, which are PP3, PP4
  element_pow_zn(PP3, g_1, d_1);
  element_printf("PP3 = %B\n", PP3);
  element_pow_zn(PP4, g_1, d_2);
  element_printf("PP4 = %B\n", PP4);
  //calculate MK2, MK3
  element_t MK2, MK3;
  element_init_G2(MK2, pairing);
  element_init_G2(MK3, pairing);
  element_pow_zn(MK2, g_2, D_1);
  element_printf("MK2 = %B\n", MK2);
  element_pow_zn(MK3, g_2, D_2);
  element_printf("MK3 = %B\n", MK3);
  //generate SK_ID
  //generate r, ID first
  element_t r, SK_ID, ID;
  element_init_Zr(r, pairing);
  element_init_G2(SK_ID, pairing);
  element_init_Zr(ID, pairing);
  element_random(ID);
  element_random(r);
  //lets calculate SK_ID
  element_t r_ID, alpha_r_ID, alpha_r_ID_D_1, r_D_2, SK_ID_EXP;
  element_init_Zr(r_ID, pairing);
  element_init_Zr(alpha_r_ID, pairing);
  element_init_Zr(alpha_r_ID_D_1, pairing);
  element_init_Zr(r_D_2, pairing);
  element_init_Zr(SK_ID_EXP, pairing);
  element_mul(r_ID, r, ID);
  element_add(alpha_r_ID, alpha, r_ID);
  element_mul(alpha_r_ID_D_1, alpha_r_ID, D_1);
  element_mul(r_D_2, r, D_2);
  element_sub(SK_ID_EXP, alpha_r_ID_D_1, r_D_2);
  element_pow_zn(SK_ID, g_2, SK_ID_EXP);
  //element_pow_zn(SK_ID, g_2, alpha_r_ID_D_1);
  element_printf("SK_ID = %B\n",SK_ID);
  //lets calculate SK_ID in another way
  element_t SK_ID_prime;
  element_init_G2(SK_ID_prime, pairing);
  element_t i_r;
  element_init_Zr(i_r, pairing);
  element_neg(i_r, r);
  element_pow2_zn(SK_ID_prime, MK2, alpha_r_ID, MK3, i_r);
  element_printf("SK_ID_prime = %B\n",SK_ID_prime);
  if (!element_cmp(SK_ID, SK_ID_prime)) {
    printf("--SK_ID-------same----------\n");}
  else{
    printf("--SK_ID---different--------\n");
  }
  //test something.
  //element_t test1, test2, test3, test4;
  //element_init_G2(test1, pairing);
  //element_init_G2(test2, pairing);
  //element_init_G2(test3, pairing);
  //element_init_Zr(test4, pairing);
  //element_pow_zn(test1, g_2, D_1);
  //element_pow_zn(test2, test1, r_ID);
  //element_printf("test2 = %B\n", test2);
  //element_mul(test4, D_1, r_ID);
  //element_pow_zn(test3, g_2, test4);
  //element_printf("test3 = %B\n", test3);
  //if (!element_cmp(test2, test3)) {
    //printf("---------same----------\n");}
  //else{
    //printf("-----different--------\n");
  //}
  //element_mul(test3, test2, test1);
  //element_printf("test3 = %B\n",test3);

  //time for encryption, lets go
  element_t C_0, C_1, M, s, C_0_1, s_d_1, s_ID, s_ID_d_2, C_1_EXP;
  element_init_GT(M, pairing);
  element_init_Zr(s, pairing);
  element_init_GT(C_0_1, pairing);
  element_init_GT(C_0, pairing);
  //element_from_hash(M, "helloworld", 10);
  element_random(M);
  element_printf("M = %B\n",M);
  element_random(s);
  element_printf("s = %B\n", s);
  element_pow_zn(C_0_1, PP2, s);
  element_printf("C_0 right side: %B\n", C_0_1);
  element_mul(C_0, C_0_1, M);
  element_printf("C_0 = %B\n",C_0);
  element_init_Zr(s_d_1, pairing);
  element_init_Zr(s_ID, pairing);
  element_init_Zr(s_ID_d_2, pairing);
  element_init_Zr(C_1_EXP, pairing);
  element_init_G1(C_1, pairing);
  element_mul(s_d_1, s, d_1);
  element_mul(s_ID, s, ID);
  element_mul(s_ID_d_2, s_ID, d_2);
  element_add(C_1_EXP, s_d_1, s_ID_d_2);
  element_pow_zn(C_1, g_1, C_1_EXP);
  element_printf("C_1 = %B\n",C_1);
  //lets make sure C_1 is correct
  element_t C_1_0, C_1_1, C_1_prime;
  element_init_G1(C_1_0, pairing);
  element_init_G1(C_1_1, pairing);
  element_init_G1(C_1_prime, pairing);
  element_pow_zn(C_1_0, PP3, s);
  element_pow_zn(C_1_1, PP4, s_ID);
  element_mul(C_1_prime, C_1_0, C_1_1);
  if (!element_cmp(C_1, C_1_prime)) {
    printf("--C_1-------same----------\n");}
  else{
    printf("--C_1---different--------\n");
  }

  //do some test on C_0_1, I want to know whether d_1 * D_1 = 1
  element_t alpha_s, C_0_1_prime;
  element_init_Zr(alpha_s, pairing);
  element_init_GT(C_0_1_prime, pairing);
  element_mul(alpha_s, alpha, s);
  element_pow_zn(C_0_1_prime, e_g_1_g_2, alpha_s);
  if (!element_cmp(C_0_1, C_0_1_prime)) {
    printf("---C_0_1------same----------\n");}
  else{
    printf("---C_0_1--different--------\n");
  }
  
  //lets see how decryption works.
  element_t M_prime, M_prime_bot;
  element_init_GT(M_prime, pairing);
  element_init_GT(M_prime_bot, pairing);
  element_pairing(M_prime_bot, C_1, SK_ID);
  element_printf("the bottom: %B\n", M_prime_bot);
  if (!element_cmp(C_0_1, M_prime_bot)) {
    printf("---DEC------same----------\n");}
  else{
    printf("---DEC--different--------\n");
  }
  element_div(M_prime, C_0, M_prime_bot);
  element_printf("the M_prime = %B\n", M_prime); 
  if (!element_cmp(M_prime, M)) {
    printf("---------same----------\n");}
  else{
    printf("-----different--------\n");
  } 

  // some test:
  //element_t rd1, rd2, rd3, rd4;
  //element_init_GT(rd1, pairing);
  //element_init_GT(rd2, pairing);
  //element_init_GT(rd3, pairing);
  //element_init_GT(rd4, pairing);
  //element_random(rd1);
  //element_random(rd2);
  //element_mul(rd3, rd1, rd2);
  //element_div(rd4, rd3, rd2);
  //if (!element_cmp(rd1,rd4)) {
    //printf("--rd-------same----------\n");}
  //else{
    //printf("--rd---different--------\n");
  //} 
  
  
  
  return 0;
}
