// Boneh-Lynn-Shacham short signatures demo.
//
// See the PBC_sig library for a practical implementation.
//
// Ben Lynn
#include <pbc.h>
#include <pbc_test.h>

int main(int argc, char **argv) {
  pairing_t pairing; //initialize the pairing, which is a GDH group.
  element_t g, h; // g is the generator, h is the message
  element_t public_key, sig; //public key = g^x, sig = h^x
  element_t secret_key; //secret key = x
  element_t temp1, temp2;

  pbc_demo_pairing_init(pairing, argc, argv);// the parameters determine the type of pairing we using here, which is a GDH group with pairing. I don't know which pairing is in use.

  element_init_G2(g, pairing); //initialize g, which is the generator, in G2. it should look like (..., ...), which is a point.
  element_init_G2(public_key, pairing); // initialize the public key in G2, a point.
  element_init_G1(h, pairing);// message in G1. it should look like (..., ...), which is a point.
  element_init_G1(sig, pairing);// signature should look be a point.
  element_init_GT(temp1, pairing);
  element_init_GT(temp2, pairing);
  element_init_Zr(secret_key, pairing);// initialize the x. which is an integer.

  printf("Short signature test\n");

  //generate system parameters
  element_random(g);
  element_printf("system parameter g = %B\n", g);

  //generate private key
  element_random(secret_key);
  element_printf("private key = %B\n", secret_key);

  //compute corresponding public key
  element_pow_zn(public_key, g, secret_key); //public key = g^x. 
  element_printf("public key = %B\n", public_key);

	double t0, t1, ttotal, ttotalpp;
  //generate element from a hash
  //for toy pairings, should check that pairing(g, h) != 1
	int i = 0;
	int n = 1000;
	ttotal = 0.0;
	for(i; i<n; i++)
	{	
		element_random(h);
		t0 = pbc_get_time();
	    	element_pow_zn(sig, h, secret_key);
		t1 = pbc_get_time();
		ttotal += t1 - t0;
		
	}
	printf("average sign time = %f\n", ttotal / n);
  //element_from_hash(h, "hashofmessage", 13);
  //element_printf("message hash = %B\n", h);

  //h^secret_key is the signature
  //in real life: only output the first coordinate
  //element_pow_zn(sig, h, secret_key);
  //element_printf("signature = %B\n", sig);

  {
    int n = pairing_length_in_bytes_compressed_G1(pairing);
    //int n = element_length_in_bytes_compressed(sig);
    int i;
    unsigned char *data = pbc_malloc(n);

    element_to_bytes_compressed(data, sig);
    printf("compressed = ");
    for (i = 0; i < n; i++) {
      printf("%02X", data[i]);
    }
    printf("\n");

    element_from_bytes_compressed(sig, data);
    element_printf("decompressed = %B\n", sig);

    pbc_free(data);
  }

  //verification part 1
	i = 0;
	ttotal = 0.0;
	for(i; i<n; i++)
	{	
		element_random(h);
		t0 = pbc_get_time();
	    	element_pairing(temp1, sig, g);
  		element_pairing(temp2, h, public_key);
		element_cmp(temp1, temp2);
		t1 = pbc_get_time();
		ttotal += t1 - t0;
		
	}
	printf("average verify time = %f\n", ttotal / n);
  //element_pairing(temp1, sig, g);
  //element_printf("f(sig, g) = %B\n", temp1);

  //verification part 2
  //should match above
  //element_pairing(temp2, h, public_key);
  //element_printf("f(message hash, public_key) = %B\n", temp2);

  /*if (!element_cmp(temp1, temp2)) {
    printf("signature verifies\n");
  } else {
    printf("*BUG* signature does not verify *BUG*\n");
  }*/

  {
    int n = pairing_length_in_bytes_x_only_G1(pairing);
    //int n = element_length_in_bytes_x_only(sig);
    int i;
    unsigned char *data = pbc_malloc(n);

    element_to_bytes_x_only(data, sig);
    printf("x-coord = ");
    for (i = 0; i < n; i++) {
      printf("%02X", data[i]);
    }
    printf("\n");

    element_from_bytes_x_only(sig, data);
    element_printf("de-x-ed = %B\n", sig);

    element_pairing(temp1, sig, g);
    if (!element_cmp(temp1, temp2)) {
      printf("signature verifies on first guess\n");
    } else {
      element_invert(temp1, temp1);
      if (!element_cmp(temp1, temp2)) {
        printf("signature verifies on second guess\n");
      } else {
        printf("*BUG* signature does not verify *BUG*\n");
      }
    }

    pbc_free(data);
  }

  //a random signature shouldn't verify
  element_random(sig);
  element_pairing(temp1, sig, g);
  if (element_cmp(temp1, temp2)) {
    printf("random signature doesn't verify\n");
  } else {
    printf("*BUG* random signature verifies *BUG*\n");
  }

  element_clear(sig);
  element_clear(public_key);
  element_clear(secret_key);
  element_clear(g);
  element_clear(h);
  element_clear(temp1);
  element_clear(temp2);
  pairing_clear(pairing);
  return 0;
}
