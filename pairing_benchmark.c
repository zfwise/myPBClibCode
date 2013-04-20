#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include "pbc_test.h"
#include <pbc_random.h>

#include "w12ibe_core.c"
#include "file_operation.c"
#include "aes.c"

int main(int argc, char **argv) {
  pairing_t pairing; //initialize the pairing, d159 MNT curve
  pbc_demo_pairing_init(pairing, argc, argv); 
  element_t g1, g2, r, gt, g1r, g2r;
  element_init_G1(g1, pairing);
  element_init_G2(g2, pairing);
  element_init_G1(g1r, pairing);
  element_init_G2(g2r, pairing);
  element_init_Zr(r, pairing);
  element_init_GT(gt, pairing);
  element_random(g1);
  element_printf("g1: %B\n",g1);
  element_random(g2);
  element_printf("g2: %B\n",g2);
  element_random(r);
  element_pp_t g_pp;
  pairing_pp_t g_pp2;

  int n = 100;
  double t0, t1, ttotal, pttotal, ppttotal;
  ttotal = 0.00000000;
  pttotal = 0.00000000;
  ppttotal = 0.00000000;
		int k = 0;
		for(k; k<n; k++){
			element_random(g1);
			element_random(r);
			t0 = pbc_get_time();
	    		element_pow_zn(g1r, g1, r);
			t1 = pbc_get_time();
			ttotal += t1 - t0;

			t0 = pbc_get_time();
			element_pp_init(g_pp, g1);
			t1 = pbc_get_time();
			ppttotal += t1 - t0;
	
			t0 = pbc_get_time();
	    		element_pp_pow_zn(g1r, r, g_pp);
			t1 = pbc_get_time();
			pttotal += t1 - t0;
		}
		printf("average G1 exp time = %f ms\n", ttotal/n*1000);
		printf("After pre-processing average G1 exp time = %f ms\n", pttotal/n*1000);
		printf("pre-processing itself takes %f ms\n\n", ppttotal/n*1000);
  ttotal = 0.00000000;
  pttotal = 0.00000000;
  ppttotal = 0.00000000;
		k = 0;
		for(k; k<n; k++){
			element_random(g2);
			element_random(r);
			t0 = pbc_get_time();
	    		element_pow_zn(g2r, g2, r);
			t1 = pbc_get_time();
			ttotal += t1 - t0;

			t0 = pbc_get_time();
			element_pp_init(g_pp, g2);
			t1 = pbc_get_time();
			ppttotal += t1 - t0;

			t0 = pbc_get_time();
	    		element_pp_pow_zn(g2r, r, g_pp);
			t1 = pbc_get_time();
			pttotal += t1 - t0;
		}
		printf("average G2 exp time = %f ms\n", ttotal/n*1000);
		printf("pre-processing average G2 exp time = %f ms\n", pttotal/n*1000);
		printf("pre-processing itself takes %f ms\n\n", ppttotal/n*1000);
  ttotal = 0.00000000;
  pttotal = 0.00000000;
  ppttotal = 0.00000000;
		k = 0;
		for(k; k<n; k++){
			element_random(g1);
			element_random(g2);
			t0 = pbc_get_time();
	    		element_pairing(gt, g1, g2);
			t1 = pbc_get_time();
			ttotal += t1 - t0;
			
			t0 = pbc_get_time();
			pairing_pp_init(g_pp2, g1, pairing);
			t1 = pbc_get_time();
			ppttotal += t1 - t0;
		
			
			//pairing_pp_init(g_pp2, g2, pairing);
			t0 = pbc_get_time();
	    		pairing_pp_apply(gt, g2, g_pp2);
			t1 = pbc_get_time();
			pttotal += t1 - t0;
		}
		printf("average pairing time = %f ms\n", ttotal/n*1000);
		printf("pre-processing average pairing time = %f ms\n", pttotal/n*1000);
		printf("pre-processing itself takes %f ms\n\n", ppttotal/n*1000);
  
  
  return 0;
}


