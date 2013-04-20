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
"Usage: sign [OPTION ...] PUB_KEY MSK_KEY IN_FILE\n"
"\n"
"verify  pub_para.txt sign.txt message.txt\n"
"behavior.\n"
"\n"
"Mandatory arguments to long options are mandatory for short options too.\n\n"
" -h, --help               print this message\n\n"
" -k, --keep-input-file    don't delete original file\n\n"
"";


char* pub_file   = 0;
char* sig_file	 = 0;
char* in_file    = 0;
int   keep       = 0;



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
		else if( !strcmp(argv[i], "-k") || !strcmp(argv[i], "--keep-input-file") )
		{
			keep = 1;
		}
		else if( !pub_file )
		{
			pub_file = argv[i];
		}
		else if( !sig_file )
		{
			sig_file = argv[i];
		}
		else if( !in_file )
		{
			in_file = argv[i];
		}
		else
			die(usage);

	if( !pub_file || !sig_file || !in_file )
		die(usage);
}

int
main( int argc, char** argv )
{
	
	w12ibe_pub_params* pub;
	w12ibe_sig* sig;
	int file_len;
	element_t message;
	
	parse_args(argc, argv);

	pub = w12ibe_pub_unserialize(suck_file(pub_file), 1);
	sig = w12ibe_sig_unserialize(pub, suck_file(sig_file), 1);

	element_init_Zr(message, pub->pairing);
	element_from_hash(message, in_file, strlen(in_file));
	if( !w12ibe_verify(pub, sig, message))
		printf("\nFail to verify signature\n");
	else
		printf("\nSignature Verified\n");

	return 0;
}

