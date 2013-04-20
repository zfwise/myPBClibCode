#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>

#include "w12ibe_core.c"
#include "file_operation.c"


char* usage =
"Usage: keygen [OPTION ...]\n"
"\n"
"Generate private key given an ID\n"
"\n"
"Output will be written to the files \"sk_id\"\n"
"kengen -o sk_id_FILE pub_params.txt msk.txt user_id\n"
"\n"
" -h, --help                    print this message\n\n"
" -o, --output-user's secret-key FILE  write public key to FILE\n\n"
"";

char*  pub_file = 0;
char*  msk_file = 0;
char*  user_id = 0;

char*  out_file = 0;

parse_args( int argc, char** argv )
{
	int i;

	for( i = 1; i < argc; i++ )
		if(      !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") )
		{
			printf("%s", usage);
			exit(0);
		}
		else if( !strcmp(argv[i], "-o") || !strcmp(argv[i], "--output") )
		{
			if( ++i >= argc )
				die(usage);
			else
				out_file = argv[i];
		}
		else if( !pub_file )
		{
			pub_file = argv[i];
		}
		else if( !msk_file )
		{
			msk_file = argv[i];
		}
		else if( !user_id )
		{
			user_id = argv[i];
		}
		else
		{
			die(usage);
		}

	if( !pub_file || !msk_file  )
	{
		die(usage);
	}	

}

int
main( int argc, char** argv )
{
	w12ibe_pub_params* pub;
	w12ibe_msk* msk;
	w12ibe_sk_id* sk;

	parse_args(argc, argv);

	pub = w12ibe_pub_unserialize(suck_file(pub_file), 1);
	msk = w12ibe_msk_unserialize(pub, suck_file(msk_file), 1);

	/*
	given a user_id, we hash it into Zr
	*/ 	
	element_t ID;
	element_init_Zr(ID, pub->pairing);
	element_from_hash(ID, user_id, strlen(user_id));
	
	sk = w12ibe_keygen(pub, msk, ID);
	//spit_file(strcat(out_file,"_sk.txt"), w12ibe_sk_id_serialize(sk), 1);
	spit_file(out_file, w12ibe_sk_id_serialize(sk), 1);
	return 0;
}
