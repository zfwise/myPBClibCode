/*
functions are defined here.
*/

w12ibe_DPVS* w12ibe_DPVS_gen(w12ibe_pub_params* pub);

void w12ibe_setup( w12ibe_pub_params** pub, w12ibe_msk** msk);

w12ibe_sk_id* w12ibe_keygen( w12ibe_pub_params* pub, w12ibe_msk* msk, element_t ID);

w12ibe_ct* w12ibe_enc( w12ibe_pub_params* pub, element_t m, element_t ID);

int w12ibe_dec( w12ibe_pub_params* pub, w12ibe_ct* CT, w12ibe_sk_id* sk, element_t m);

w12ibe_sig* w12ibe_sign( w12ibe_pub_params* pub, w12ibe_msk* sk, element_t mHash);

int w12ibe_verify( w12ibe_pub_params* pub, w12ibe_sig* sign, element_t mHash);


GByteArray* w12ibe_pub_serialize( w12ibe_pub_params* pub );
GByteArray* w12ibe_msk_serialize( w12ibe_msk* msk );
GByteArray* w12ibe_sk_id_serialize( w12ibe_sk_id* sk );
GByteArray* w12ibe_ct_serialize( w12ibe_ct* ct );
GByteArray* w12ibe_sig_serialize( w12ibe_sig* sig );


w12ibe_pub_params* w12ibe_pub_unserialize( GByteArray* b, int free );
w12ibe_msk* w12ibe_msk_unserialize( w12ibe_pub_params* pub, GByteArray* b, int free );
w12ibe_sk_id* w12ibe_sk_id_unserialize( w12ibe_pub_params* pub, GByteArray* b, int free );
w12ibe_ct* w12ibe_ct_unserialize( w12ibe_pub_params* pub, GByteArray* b, int free );
w12ibe_sig* w12ibe_sig_unserialize( w12ibe_pub_params* pub, GByteArray* b, int free );

void w12ibe_pub_free( w12ibe_pub_params* pub );
void w12ibe_msk_free( w12ibe_msk* msk );
void w12ibe_prv_free( w12ibe_sk_id* prv );
void w12ibe_cph_free( w12ibe_ct* ct );
void w12ibe_sig_free( w12ibe_sig* sig );

void        spit_file( char* file, GByteArray* b, int free );

void die(char* fmt, ...);

void read_w12ibe_file( char* file,    GByteArray** cph_buf, int* file_len, GByteArray** aes_buf );

void write_w12ibe_file( char* file,   GByteArray* cph_buf, int file_len, GByteArray* aes_buf );

