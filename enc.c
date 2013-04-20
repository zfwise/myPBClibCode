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
"Usage: enc [OPTION ...] PUB_KEY FILE \n"
"\n"
"Encrypt FILE given receiver's ID and pub parameters\n"
"enc -o ct.txt pub_para.txt message.txt receiver_id\n"
"\n"
" -h, --help               print this message\n\n"
" -k, --keep-input-file    don't delete original file\n\n"
" -o, --output FILE        write resulting key to FILE\n\n"
"";

char* pub_file = 0;
char* in_file  = 0;
char* rec_id  = 0;
char* out_file = 0;
int   keep     = 0;


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
		else if( !in_file )
		{
			in_file = argv[i];
		}
		else if( !rec_id )
		{
			rec_id = argv[i];
		}
		else
			die(usage);

	if( !pub_file || !in_file )
		die(usage);

	if( !out_file )
		die(usage);

}

int
main( int argc, char** argv )
{
	w12ibe_pub_params* pub;
	w12ibe_ct* ct;
	int file_len;
	GByteArray* plt;
	GByteArray* ct_buf;
	GByteArray* aes_buf;
	element_t m;

	parse_args(argc, argv);

	pub = w12ibe_pub_unserialize(suck_file(pub_file), 1);
	
	element_t rec_ID;
	element_init_Zr(rec_ID, pub->pairing);
	element_from_hash(rec_ID, rec_id, strlen(rec_id)); //this function deterministically generate an element given a input.
  	if( !(ct = w12ibe_enc(pub, m, rec_ID)) )
		die("%s", w12ibe_error());

	ct_buf = w12ibe_ct_serialize(ct);
	w12ibe_ct_free(ct);

	plt = suck_file(in_file);
	file_len = plt->len;
	aes_buf = aes_128_cbc_encrypt(plt, m);
	g_byte_array_free(plt, 1);
	// clear the memory.
	element_clear(m);

	write_w12ibe_file(out_file, ct_buf, file_len, aes_buf);

	g_byte_array_free(ct_buf, 1);
	g_byte_array_free(aes_buf, 1);

	if( !keep )
		unlink(in_file);

	return 0;
}

