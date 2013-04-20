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
"Usage: setup [OPTION ...]\n"
"\n"
"Generate system parameters,  master secret key\n"
"\n"
"Output will be written to the files \"pub_key\" and \"master_key\"\n"
"unless the --output-public-key or --output-master-key options are\n"
"used.\n"
"\n"
" -h, --help                    print this message\n\n"
" -p, --output-public-key FILE  write public key to FILE\n\n"
" -m, --output-master-key FILE  write master secret key to FILE\n\n"
"";

char* pub_file = 0;
char* msk_file = 0;

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
		else if( !strcmp(argv[i], "-p") || !strcmp(argv[i], "--output-public-key") )
		{
			if( ++i >= argc )
				die(usage);
			else
				pub_file = argv[i];
		}
		else if( !strcmp(argv[i], "-m") || !strcmp(argv[i], "--output-master-key") )
		{
			if( ++i >= argc )
				die(usage);
			else
				msk_file = argv[i];
		}
		else
			die(usage);
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

	parse_args(argc, argv);

	w12ibe_setup(&pub, &msk);
	spit_file(pub_file, w12ibe_pub_serialize(pub), 1);
	spit_file(msk_file, w12ibe_msk_serialize(msk), 1);

	return 0;
}

