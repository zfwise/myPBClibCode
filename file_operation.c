/*
	include w12ibe_core.c before include this file.
	w12ibe_core.c linked to the data type and function declarations
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>


void
serialize_uint32( GByteArray* b, uint32_t k )
{
	int i;
	guint8 byte;

	for( i = 3; i >= 0; i-- )
	{
		byte = (k & 0xff<<(i*8))>>(i*8);
		g_byte_array_append(b, &byte, 1);
	}
}

uint32_t
unserialize_uint32( GByteArray* b, int* offset )
{
	int i;
	uint32_t r;

	r = 0;
	for( i = 3; i >= 0; i-- )
		r |= (b->data[(*offset)++])<<(i*8);

	return r;
}

void
serialize_element( GByteArray* b, element_t e )
{
	uint32_t len;
	unsigned char* buf;

	len = element_length_in_bytes(e);
	serialize_uint32(b, len);

	buf = (unsigned char*) malloc(len);
	element_to_bytes(buf, e);
	g_byte_array_append(b, buf, len);
	free(buf);
}

void
unserialize_element( GByteArray* b, int* offset, element_t e )
{
	uint32_t len;
	unsigned char* buf;

	len = unserialize_uint32(b, offset);

	buf = (unsigned char*) malloc(len);
	memcpy(buf, b->data + *offset, len);
	*offset += len;

	element_from_bytes(e, buf);
	free(buf);
}

void
serialize_string( GByteArray* b, char* s )
{
	g_byte_array_append(b, (unsigned char*) s, strlen(s) + 1);
}

char*
unserialize_string( GByteArray* b, int* offset )
{
	GString* s;
	char* r;
	char c;

	s = g_string_sized_new(32);
	while( 1 )
	{
		c = b->data[(*offset)++];
		if( c && c != EOF )
			g_string_append_c(s, c);
		else
			break;
	}

	r = s->str;
	g_string_free(s, 0);

	return r;
}

GByteArray*
w12ibe_pub_serialize( w12ibe_pub_params* pub )
{
	GByteArray* b;

	b = g_byte_array_new();
	serialize_string(b,  pub->pairing_desc);
	serialize_element(b, pub->g1);
	serialize_element(b, pub->g2);
	serialize_element(b, pub->PP2);
	serialize_element(b, pub->gd11);
	serialize_element(b, pub->gd12);
	serialize_element(b, pub->gd13);
	serialize_element(b, pub->gd14);
	serialize_element(b, pub->gd21);
	serialize_element(b, pub->gd22);
	serialize_element(b, pub->gd23);
	serialize_element(b, pub->gd24);

	return b;
}
/*
	those unserialize functions will free the input if you set the variable free=1
*/
w12ibe_pub_params*
w12ibe_pub_unserialize( GByteArray* b, int free )
{
	w12ibe_pub_params* pub;
	int offset;

	pub = (w12ibe_pub_params*) malloc(sizeof(w12ibe_pub_params));
	offset = 0;

	pub->pairing_desc = unserialize_string(b, &offset);
	pairing_init_set_buf(pub->pairing, pub->pairing_desc, strlen(pub->pairing_desc));

	element_init_G1(pub->g1,        pub->pairing);
	element_init_G2(pub->g2,        pub->pairing);
	element_init_GT(pub->PP2,       pub->pairing);
	element_init_G1(pub->gd11, 	pub->pairing);
	element_init_G1(pub->gd12,      pub->pairing);
	element_init_G1(pub->gd13,      pub->pairing);
	element_init_G1(pub->gd14,      pub->pairing);
	element_init_G1(pub->gd21, 	pub->pairing);
	element_init_G1(pub->gd22,      pub->pairing);
	element_init_G1(pub->gd23,      pub->pairing);
	element_init_G1(pub->gd24,      pub->pairing);
	
	unserialize_element(b, &offset, pub->g1);
	unserialize_element(b, &offset, pub->g2);
	unserialize_element(b, &offset, pub->PP2);
	unserialize_element(b, &offset, pub->gd11);
	unserialize_element(b, &offset, pub->gd12);
	unserialize_element(b, &offset, pub->gd13);
	unserialize_element(b, &offset, pub->gd14);
	unserialize_element(b, &offset, pub->gd21);
	unserialize_element(b, &offset, pub->gd22);
	unserialize_element(b, &offset, pub->gd23);
	unserialize_element(b, &offset, pub->gd24);

	if( free )
		g_byte_array_free(b, 1);

	return pub;
}

GByteArray*
w12ibe_msk_serialize( w12ibe_msk* msk )
{
	GByteArray* b;

	b = g_byte_array_new();
	serialize_element(b, msk->alpha);
	serialize_element(b, msk->gD11);
	serialize_element(b, msk->gD12);
	serialize_element(b, msk->gD13);
	serialize_element(b, msk->gD14);
	serialize_element(b, msk->gD21);
	serialize_element(b, msk->gD22);
	serialize_element(b, msk->gD23);
	serialize_element(b, msk->gD24);

	return b;
}

w12ibe_msk*
w12ibe_msk_unserialize( w12ibe_pub_params* pub, GByteArray* b, int free )
{
	w12ibe_msk* msk;
	int offset;

	msk = (w12ibe_msk*) malloc(sizeof(w12ibe_msk));
	offset = 0;

	element_init_Zr(msk->alpha, pub->pairing);
	element_init_G2(msk->gD11, pub->pairing);
	element_init_G2(msk->gD12, pub->pairing);
	element_init_G2(msk->gD13, pub->pairing);
	element_init_G2(msk->gD14, pub->pairing);
	element_init_G2(msk->gD21, pub->pairing);
	element_init_G2(msk->gD22, pub->pairing);
	element_init_G2(msk->gD23, pub->pairing);
	element_init_G2(msk->gD24, pub->pairing);

	unserialize_element(b, &offset, msk->alpha);
	unserialize_element(b, &offset, msk->gD11);
	unserialize_element(b, &offset, msk->gD12);
	unserialize_element(b, &offset, msk->gD13);
	unserialize_element(b, &offset, msk->gD14);
	unserialize_element(b, &offset, msk->gD21);
	unserialize_element(b, &offset, msk->gD22);
	unserialize_element(b, &offset, msk->gD23);
	unserialize_element(b, &offset, msk->gD24);
	if( free )
		g_byte_array_free(b, 1);

	return msk;
}

GByteArray*
w12ibe_sk_id_serialize( w12ibe_sk_id* sk )
{
	GByteArray* b;

	b = g_byte_array_new();

	serialize_element(b, sk->ID);
	serialize_element(b, sk->SK_ID1);
	serialize_element(b, sk->SK_ID2);
	serialize_element(b, sk->SK_ID3);
	serialize_element(b, sk->SK_ID4);

	return b;
}

w12ibe_sk_id*
w12ibe_sk_id_unserialize( w12ibe_pub_params* pub, GByteArray* b, int free )
{
	w12ibe_sk_id* sk;
	int offset;

	sk = (w12ibe_sk_id*) malloc(sizeof(w12ibe_sk_id));
	offset = 0;

	element_init_Zr(sk->ID, pub->pairing);
	element_init_G2(sk->SK_ID1, pub->pairing);
	element_init_G2(sk->SK_ID2, pub->pairing);
	element_init_G2(sk->SK_ID3, pub->pairing);
	element_init_G2(sk->SK_ID4, pub->pairing);

	unserialize_element(b, &offset, sk->ID);
	unserialize_element(b, &offset, sk->SK_ID1);
	unserialize_element(b, &offset, sk->SK_ID2);
	unserialize_element(b, &offset, sk->SK_ID3);
	unserialize_element(b, &offset, sk->SK_ID4);

	if( free )
		g_byte_array_free(b, 1);

	return sk;
}


GByteArray*
w12ibe_ct_serialize( w12ibe_ct* ct )
{
	GByteArray* b;

	b = g_byte_array_new();
	serialize_element(b, ct->C0);
	serialize_element(b, ct->C11);
	serialize_element(b, ct->C12);
	serialize_element(b, ct->C13);
	serialize_element(b, ct->C14);

	return b;
}

w12ibe_ct*
w12ibe_ct_unserialize( w12ibe_pub_params* pub, GByteArray* b, int free )
{
	w12ibe_ct* ct;
	int offset;

	ct = (w12ibe_ct*) malloc(sizeof(w12ibe_ct));
	offset = 0;

	element_init_GT(ct->C0, pub->pairing);
	element_init_G1(ct->C11, pub->pairing);
	element_init_G1(ct->C12, pub->pairing);
	element_init_G1(ct->C13, pub->pairing);
	element_init_G1(ct->C14, pub->pairing);
	
	unserialize_element(b, &offset, ct->C0);
	unserialize_element(b, &offset, ct->C11);
	unserialize_element(b, &offset, ct->C12);
	unserialize_element(b, &offset, ct->C13);
	unserialize_element(b, &offset, ct->C14);

	if( free )
		g_byte_array_free(b, 1);

	return ct;
}

GByteArray*
w12ibe_sig_serialize( w12ibe_sig* sig )
{
	GByteArray* b;

	b = g_byte_array_new();
	serialize_element(b, sig->sign1);
	serialize_element(b, sig->sign2);
	serialize_element(b, sig->sign3);
	serialize_element(b, sig->sign4);

	return b;
}

w12ibe_sig*
w12ibe_sig_unserialize( w12ibe_pub_params* pub, GByteArray* b, int free )
{
	w12ibe_sig* sig;
	int offset;

	sig = (w12ibe_sig*) malloc(sizeof(w12ibe_sig));
	offset = 0;

	element_init_G2(sig->sign1, pub->pairing);
	element_init_G2(sig->sign2, pub->pairing);
	element_init_G2(sig->sign3, pub->pairing);
	element_init_G2(sig->sign4, pub->pairing);
	
	unserialize_element(b, &offset, sig->sign1);
	unserialize_element(b, &offset, sig->sign2);
	unserialize_element(b, &offset, sig->sign3);
	unserialize_element(b, &offset, sig->sign4);

	if( free )
		g_byte_array_free(b, 1);

	return sig;
}

void
w12ibe_pub_free( w12ibe_pub_params* pub )
{
	element_clear(pub->g1);
	element_clear(pub->g2);
	element_clear(pub->PP2);
	element_clear(pub->gd11);
	element_clear(pub->gd12);
	element_clear(pub->gd13);
	element_clear(pub->gd14);
	element_clear(pub->gd21);
	element_clear(pub->gd22);
	element_clear(pub->gd23);
	element_clear(pub->gd24);
	pairing_clear(pub->pairing);
	free(pub->pairing_desc);
	free(pub);
}

void
w12ibe_msk_free( w12ibe_msk* msk )
{
	element_clear(msk->alpha);
	element_clear(msk->gD11);
	element_clear(msk->gD12);
	element_clear(msk->gD13);
	element_clear(msk->gD14);
	element_clear(msk->gD21);
	element_clear(msk->gD22);
	element_clear(msk->gD23);
	element_clear(msk->gD24);
	free(msk);
}

void
w12ibe_sk_id_free( w12ibe_sk_id* sk )
{
	element_clear(sk->ID);
	element_clear(sk->SK_ID1);
	element_clear(sk->SK_ID2);
	element_clear(sk->SK_ID3);
	element_clear(sk->SK_ID4);
	free(sk);
}

void
w12ibe_ct_free( w12ibe_ct* ct )
{
	element_clear(ct->C0);
	element_clear(ct->C11);
	element_clear(ct->C12);
	element_clear(ct->C13);
	element_clear(ct->C14);
}

void
w12ibe_sig_free( w12ibe_sig* sig )
{
	element_clear(sig->sign1);
	element_clear(sig->sign2);
	element_clear(sig->sign3);
	element_clear(sig->sign4);
}

FILE*
fopen_read_or_die( char* file )
{
	FILE* f;

	if( !(f = fopen(file, "r")) )
		die("can't read file: %s\n", file);

	return f;
}

FILE*
fopen_write_or_die( char* file )
{
	FILE* f;

	if( !(f = fopen(file, "w")) )
		die("can't write file: %s\n", file);

	return f;
}

GByteArray*
suck_file( char* file )
{
	FILE* f;
	GByteArray* a;
	struct stat s;

	a = g_byte_array_new();
	stat(file, &s);
	g_byte_array_set_size(a, s.st_size);

	f = fopen_read_or_die(file);
	fread(a->data, 1, s.st_size, f);
	fclose(f);

	return a;
}

void
spit_file( char* file, GByteArray* b, int free )
{
	FILE* f;

	f = fopen_write_or_die(file);
	fwrite(b->data, 1, b->len, f);
	fclose(f);

	if( free )
		g_byte_array_free(b, 1);
}

void read_w12ibe_file( char* file, GByteArray** cph_buf, int* file_len, GByteArray** aes_buf )
{
	FILE* f;
	int i;
	int len;

	*cph_buf = g_byte_array_new();
	*aes_buf = g_byte_array_new();

	f = fopen_read_or_die(file);

	/* read real file len as 32-bit big endian int */
	*file_len = 0;
	for( i = 3; i >= 0; i-- )
		*file_len |= fgetc(f)<<(i*8);

	/* read aes buf */
	len = 0;
	for( i = 3; i >= 0; i-- )
		len |= fgetc(f)<<(i*8);
	g_byte_array_set_size(*aes_buf, len);
	fread((*aes_buf)->data, 1, len, f);

	/* read cph buf */
	len = 0;
	for( i = 3; i >= 0; i-- )
		len |= fgetc(f)<<(i*8);
	g_byte_array_set_size(*cph_buf, len);
	fread((*cph_buf)->data, 1, len, f);
	
	fclose(f);
}

void
write_w12ibe_file( char* file,   GByteArray* cph_buf, int file_len, GByteArray* aes_buf )
{
	FILE* f;
	int i;

	f = fopen_write_or_die(file);

	/* write real file len as 32-bit big endian int */
	for( i = 3; i >= 0; i-- )
		fputc((file_len & 0xff<<(i*8))>>(i*8), f);

	/* write aes_buf */
	for( i = 3; i >= 0; i-- )
		fputc((aes_buf->len & 0xff<<(i*8))>>(i*8), f);
	fwrite(aes_buf->data, 1, aes_buf->len, f);

	/* write cph_buf */
	for( i = 3; i >= 0; i-- )
		fputc((cph_buf->len & 0xff<<(i*8))>>(i*8), f);
	fwrite(cph_buf->data, 1, cph_buf->len, f);

	fclose(f);
}

void
die(char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
}

