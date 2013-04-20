/*
benchmark bbibe
I didn't take the Hash functions into consideration.
The assumptions I made in this code are:
1, hash functions' running time is negligible
2, XOR operations's running time is negligible

if the above two assumptions are true, the fullIdent has the same running time as 
the basicIdent

*/
#include <pbc.h>
#include <pbc_test.h>

int main(int argc, char **argv) {
	pairing_t pairing;
	element_t g1;
	element_t s, r;
	element_t id;
	element_t pri_id;
	element_t gID;
	element_t Ppub;
	element_t C1;
	element_t gIDr;
	
	pbc_demo_pairing_init(pairing, argc, argv);
	element_init_G1(g1, pairing);
	element_init_Zr(s, pairing);
	element_init_Zr(r, pairing);
	element_init_G1(id, pairing);
	element_init_G1(pri_id, pairing);
	element_init_GT(gID, pairing);
	element_init_G1(Ppub, pairing);
	element_init_G1(C1, pairing);
	element_init_GT(gIDr, pairing);
	
	double t0, t1, ttotal, ttotalpp;
	element_random(g1);
	element_random(s);
	element_random(r);
	element_random(id);
	element_pow_zn(Ppub, g1, s);
	ttotal = 0.0;
	t0 = pbc_get_time();
	element_pow_zn(pri_id, id, s);
	element_pairing(gID, id, Ppub);
	element_pow_zn(gIDr, gID, r);
	t1 = pbc_get_time();
	
	int i = 0;
	int n = 1000;
	
	for(i; i<n; i++)
	{	
		t0 = pbc_get_time();
		element_pow_zn(C1, g1, r);
		t1 = pbc_get_time();
		ttotal += t1 - t0;
		
	}
	printf("if we only have one user and the user is cached, average enc time = %f\n", ttotal / n);

	i = 0;
	ttotal = 0.0;
	for(i; i<n; i++)
	{	
		element_random(id);
		t0 = pbc_get_time();
		element_pow_zn(pri_id, id, s);
		element_pairing(gID, id, Ppub);
		element_pow_zn(gIDr, gID, r);
		element_pow_zn(C1, g1, r);
		t1 = pbc_get_time();
		ttotal += t1 - t0;
		
	}
	printf("100 diff users, average enc time = %f\n", ttotal / n);
	
	

	//dec
	ttotal = 0.0; 
	element_t gIDr_prime;
	element_init_GT(gIDr_prime, pairing);
		i = 0;
	
	for(i; i<n; i++)
	{	
		element_random(pri_id);
		element_random(C1);
		t0 = pbc_get_time();
		element_pairing(gIDr_prime, pri_id,  C1);
		t1 = pbc_get_time();
		ttotal += t1 - t0;
		
	}
	printf("dec random messages, average dec time = %f\n", ttotal / n);
	

	/*if (!element_cmp(gIDr_prime, gIDr)) {
    	printf("----------Confirm----------------\n");}
  	else{
    	printf("----------damn--------\n");
   	}*/
	
}
