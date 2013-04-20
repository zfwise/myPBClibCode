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
"sign -o signature.txt pub_para.txt msk.txt message.txt\n"
"behavior.\n"
"\n"
"Mandatory arguments to long options are mandatory for short options too.\n\n"
" -h, --help               print this message\n\n"
" -k, --keep-input-file    don't delete original file\n\n"
" -o, --output FILE        write output to FILE\n\n"
"";


char* pub_file   = 0;
char* msk_file   = 0;
char* in_file    = 0;
char* out_file   = 0;
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
		else if( !in_file )
		{
			in_file = argv[i];
		}
		else
			die(usage);

	if( !pub_file || !msk_file || !in_file )
		die(usage);

	if( !out_file )
	{
		if(  strlen(in_file) > 6 &&
				!strcmp(in_file + strlen(in_file) - 6, ".cpabe") )
			out_file = g_strndup(in_file, strlen(in_file) - 6);
		else
			out_file = strdup(in_file);
	}
	
	if( keep && !strcmp(in_file, out_file) )
		die("cannot keep input file when decrypting file in place (try -o)\n");
}

int
main( int argc, char** argv )
{
	
	w12ibe_pub_params* pub;
	w12ibe_msk* msk;
	w12ibe_sig* sig;
	int file_len;
	GByteArray* message_file;
	element_t message;
	
	
	parse_args(argc, argv);

	pub = w12ibe_pub_unserialize(suck_file(pub_file), 1);
	msk = w12ibe_msk_unserialize(pub, suck_file(msk_file), 1);

	element_init_Zr(message, pub->pairing);
	element_from_hash(message, in_file, strlen(in_file));

	sig = w12ibe_sign(pub, msk, message);
	//spit_file(strcat(out_file,"_sk.txt"), w12ibe_sk_id_serialize(sk), 1);
	spit_file(out_file, w12ibe_sig_serialize(sig), 1);
	//maybe I should free the msk. However, it should be automatically been free when execution ends.

	return 0;
}

