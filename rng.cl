__kernel void rng(__global int* data) {
   uint global_addr;
   
   int z = 0;
   for(z = 0; z<101;z++)
	   data[0] = (data[0] + 1) % 2;

}
