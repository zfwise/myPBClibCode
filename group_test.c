// Boneh-Lynn-Shacham short signatures demo.
//
// See the PBC_sig library for a practical implementation.
//
// Ben Lynn
#include <pbc.h>
#include <pbc_test.h>

int main(int argc, char **argv) {
  pairing_t pairing; //initialize the pairing, which is a GDH group.
  element_t g_1, g_2, g_3;
  pbc_demo_pairing_init(pairing, argc, argv);
  element_init_G1(g_1, pairing);
  element_init_G2(g_2, pairing);
  element_init_GT(g_3, pairing);

  element_random(g_1);
  element_printf("g_1 = %B\n",g_1);
  element_random(g_2);
  element_printf("g_2 = %B\n",g_2);
  element_random(g_3);
  element_printf("g_3 = %B\n",g_3);

  return 0;
}
