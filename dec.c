#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>

#include "w12ibe_core.c"
#include "file_operation.c"
#include "aes.c"

char* usage =
"Usage: dec [OPTION ...] PUB_KEY PRIV_KEY FILE\n"
"\n"
"dec -o message.txt pub_para.txt sk_id.txt ct.txt\n"
"behavior.\n"
"\n"
"Mandatory arguments to long options are mandatory for short options too.\n\n"
" -h, --help               print this message\n\n"
" -k, --keep-input-file    don't delete original file\n\n"
" -o, --output FILE        write output to FILE\n\n"
"";


char* pub_file   = 0;
char* prv_file   = 0;
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
		else if( !prv_file )
		{
			prv_file = argv[i];
		}
		else if( !in_file )
		{
			in_file = argv[i];
		}
		else
			die(usage);

	if( !pub_file || !prv_file || !in_file )
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
	w12ibe_sk_id* sk;
	int file_len;
	GByteArray* aes_buf;
	GByteArray* plt;
	GByteArray* ct_buf;
	w12ibe_ct* ct;
	element_t m;
	
	parse_args(argc, argv);
	pub = w12ibe_pub_unserialize(suck_file(pub_file), 1);
	sk = w12ibe_sk_id_unserialize(pub, suck_file(prv_file), 1);
	read_w12ibe_file(in_file, &ct_buf, &file_len, &aes_buf);

	ct = w12ibe_ct_unserialize(pub, ct_buf, 1);
	if( !w12ibe_dec(pub, ct, sk, m) )
		die("%s", w12ibe_error());
	w12ibe_ct_free(ct);

	plt = aes_128_cbc_decrypt(aes_buf, m);
	g_byte_array_set_size(plt, file_len);
	g_byte_array_free(aes_buf, 1);
	//maybe I should free the sk too. However, it should be automatically been free when execution ends.
	spit_file(out_file, plt, 1);

	if( !keep )
		unlink(in_file);

	return 0;
}

