#include <pbc.h>
#include <glib.h>
#include "w12ibe_core.c"


int
main( int argc, char** argv )
{
	w12ibe_pub_params* pub;
	w12ibe_msk* msk;
	w12ibe_sk_id* sk;
	w12ibe_ct* ct;
	element_t m, m_prime;
	//parse_args(argc, argv);

	//pub = bswabe_pub_unserialize(suck_file(pub_file), 1);
	//msk = bswabe_msk_unserialize(pub, suck_file(msk_file), 1);
	
	w12ibe_setup(&pub, &msk);
	element_t ID;
	element_init_Zr(ID, pub->pairing);
	element_random(ID);
	element_t g1, g2, g3;
	element_init_G1(g1, pub->pairing);
	element_init_G1(g2, pub->pairing);
	element_init_GT(g3, pub->pairing);
	element_random(g1);
	element_random(g2);
	element_printf("g1=%B\n", g1);
	element_printf("g2=%B\n", g2);
	element_pairing(g3, g1, g2);
	sk = w12ibe_keygen( pub, msk, ID);
	
	ct = w12ibe_enc( pub, m, sk->ID);
	w12ibe_dec( pub, ct, sk, m_prime);

	if (!element_cmp(m, m_prime)) {
    		printf("Dec Success\n");}
  	else{
   		printf("Dec Fail\n");
  	} 
	//spit_file(out_file, bswabe_prv_serialize(prv), 1);

	return 0;
}


