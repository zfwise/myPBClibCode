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

char* usage =
"Usage: w12ibe_benchmark [OPTION ...] \n"
"\n"
"w12ibe_benchmark -pub pub_para -prv private_file(if necessary) -sig sig_file -msk msk_file -mode mode\n"
"Examples:\n"
"w12ibe_benchmark -pub examples/pub.txt -mode enc\n"
"w12ibe_benchmark -pub examples/pub.txt -prv examples/zfwise.txt -mode dec\n"
"w12ibe_benchmark -pub examples/pub.txt -msk examples/msk.txt -mode sign\n"
"w12ibe_benchmark -pub examples/pub.txt -sig examples/signature.txt -mode verify\n"
"\n"
"Mandatory arguments to long options are mandatory for short options too.\n\n"
" -h, --help               print this message\n\n"
" -pub, --public FILE \n\n"
" -prv, --private FILE      \n\n"
" -msk, --msk FILE      \n\n"
" -sig, --sig FILE      \n\n"
" -mode, enc, dec, sign, verify      \n\n"
"";


char* pub_file   = 0;
char* prv_file   = 0;
char* msk_file	 = 0;
char* mode	 = 0;
char* sig_file   = 0;
int n = 1000;


void
parse_args( int argc, char** argv )
{
	int i;

	for( i = 1; i < argc; i++ )
		if(      !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") )
		{
			printf("%s", usage);
			exit(0);
		}
		else if( !strcmp(argv[i], "-pub"))
		{
			if( ++i >= argc )
				die(usage);
			else
				pub_file = argv[i];
		}
		else if( !strcmp(argv[i], "-prv"))
		{
			if( ++i >= argc )
				die(usage);
			else
				prv_file = argv[i];
		}
		else if( !strcmp(argv[i], "-msk"))
		{
			if( ++i >= argc )
				die(usage);
			else
				msk_file = argv[i];
		}
		else if( !strcmp(argv[i], "-mode"))
		{
			if( ++i >= argc )
				die(usage);
			else
				mode = argv[i];
		}
		else if( !strcmp(argv[i], "-sig"))
		{
			if( ++i >= argc )
				die(usage);
			else
				sig_file = argv[i];
		}
		else
			die(usage);

}

int
main( int argc, char** argv )
{
	w12ibe_pub_params* pub;
	w12ibe_sk_id* sk;
	w12ibe_msk* msk;
	w12ibe_ct* ct;
	w12ibe_sig* sign;
	double t0, t1, ttotal, ttotalpp;
	element_t m;
	element_t user_id;
	
	parse_args(argc, argv);
	pub = w12ibe_pub_unserialize(suck_file(pub_file), 1);
	element_init_Zr(user_id, pub->pairing);
	
	if(prv_file)
		sk = w12ibe_sk_id_unserialize(pub, suck_file(prv_file), 1);
	if(msk_file)
		msk = w12ibe_msk_unserialize(pub, suck_file(msk_file), 1);
	if(sig_file)
		sign = w12ibe_sig_unserialize(pub, suck_file(sig_file), 1);
	ttotal = 0.0;
	if(!strcmp(mode, "enc"))
	{
		printf("Encryption %d times\n", n);
		int k = 0;
		for(k; k<n; k++){
			t0 = pbc_get_time();
	    		ct = w12ibe_enc(pub, m, user_id);
			t1 = pbc_get_time();
			ttotal += t1 - t0;
		}
		printf("average encryption time = %f\n", ttotal / n);
	}
	else if(!strcmp(mode, "dec"))
	{
		printf("Decryption %d times\n", n);
		int k = 0;
		ct = malloc(sizeof(w12ibe_ct));
		element_init_GT(ct->C0, pub->pairing);
		element_init_G1(ct->C11, pub->pairing);
		element_init_G1(ct->C12, pub->pairing);
		element_init_G1(ct->C13, pub->pairing);
		element_init_G1(ct->C14, pub->pairing);
		for(k; k<n; k++){

			element_random(ct->C0);
			element_random(ct->C11);
			element_random(ct->C12);
			element_random(ct->C13);
			element_random(ct->C14);

			t0 = pbc_get_time();
	    		w12ibe_dec(pub, ct, sk, m);
			t1 = pbc_get_time();
			ttotal += t1 - t0;
		}
		printf("average decryption time = %f\n", ttotal / n);
	
	}
	else if(!strcmp(mode, "sign"))
	{
		printf("Sign %d times\n", n);
		int k = 0;
		element_t message;
		element_init_Zr(message, pub->pairing);
		for(k; k<n; k++){
			element_random(message);
			t0 = pbc_get_time();
	    		sign = w12ibe_sign(pub, msk, message);
			t1 = pbc_get_time();
			ttotal += t1 - t0;
		}
		printf("average sign time = %f\n", ttotal / n);
		
	}
	else if(!strcmp(mode, "verify"))
	{
		printf("Verify %d times\n", n);
		int k = 0;
		element_t message;
		element_init_Zr(message, pub->pairing);
		for(k; k<n; k++){
			element_random(message);
			t0 = pbc_get_time();
	    		w12ibe_verify(pub, sign, message);
			t1 = pbc_get_time();
			ttotal += t1 - t0;
		}
		printf("average verify time = %f\n", ttotal / n);
	}
	else
		die(usage);

	return 0;
}


